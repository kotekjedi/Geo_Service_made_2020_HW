import sys

import csv
import json
import xml.etree.ElementTree as ET

import getopt
import logging
import re

from datetime import datetime

LOGGER_NAME = "task"


def parse_questions(file):
    questions = []
    keys = ['PostTypeId', 'CreationDate', 'Score', 'Title']

    lines = file.readlines()

    for line in lines:
        row = ET.fromstring(line)
        question = {}

        for key in keys:
            if key in row.attrib:
                question[key] = row.attrib[key]
            else:
                question[key] = None

        questions.append(question)
    return questions


def parse_stop_words(file):
    words = set(file.read().split('\n'))
    if '' in words:
        words.remove('')
    return words


def parse_queries(file):
    reader = csv.reader(file)
    data = [[int(word) for word in row] for row in reader]
    return data


def parse_command(argv):
    # logger = logging.getLogger(LOGGER_NAME)
    opts, _ = getopt.getopt(argv, "", longopts=["questions=", "stop-words=", "queries="])
    questions_path = stop_words_path = queries_path = ""

    # logger.info('Received command:')
    for opt, arg in opts:
        # logger.info('"%s": %s', opt, arg)
        if opt == '--questions':
            questions_path = arg
            # logger.debug('Parsed questions path: %s', questions_path)

        elif opt == '--stop-words':
            stop_words_path = arg
            # logger.debug('Parsed stop words path: %s', stop_words_path)

        elif opt == "--queries":
            queries_path = arg
            # logger.debug('Parsed queries path: %s', queries_path)

    return {
        'questions_path': questions_path,
        'stop_words_path': stop_words_path,
        'queries_path': queries_path
    }


def results_to_json(results, queries):
    logger = logging.getLogger(LOGGER_NAME)
    # logger.info("Converting results to json")
    res_json_lines = ""
    for res, query in zip(results, queries):
        json_line_dict = {
            "start": int(query[0]),
            "end": int(query[1]),
            "top": res
        }
        res = json.dumps(json_line_dict)
        res_json_lines += res + '\n'

    res_json_lines = res_json_lines[:-1]
    return res_json_lines


class Post(object):
    def __init__(self, title, type_id, score, creation_date):
        self.title = title
        self.type_id = type_id
        self.score = score
        self.creation_date = creation_date
        self.special_words = None
        self.title_words = None

    def transform_self(self):
        if self.title is not None:
            self.title_words = set(re.findall('\\w+', self.title.lower()))
        else:
            self.title_words = None
        self.type_id = int(self.type_id)
        self.score = int(self.score)
        self.creation_date = datetime.strptime(self.creation_date, '%Y-%m-%dT%H:%M:%S.%f')

    def remove_stop_words(self, stop_words):
        if self.title_words is not None:
            self.special_words = self.title_words - stop_words
        else:
            self.special_words = None
        return self.special_words


def post_filter(post, start_year, end_year, post_type):
    if int(post.creation_date.year) < start_year:
        return False
    if int(post.creation_date.year) > end_year:
        return False
    if post.type_id != post_type:
        return False
    return True


def filter_posts(posts, start_year, end_year):
    # logger = logging.getLogger('task')
    # logger.debug('Filtering posts from %d to %d', start_year, end_year)
    return list(filter(lambda post: post_filter(post, start_year, end_year, 1), posts))


def calc_words_scores(posts):
    # logger = logging.getLogger('task')
    # logger.debug('Calculating word scores')
    scores = {}
    for post in posts:
        for word in post.special_words:
            if word in scores:
                scores[word] += post.score
            else:
                scores[word] = post.score
    return scores


def process_query(query, posts):
    logger = logging.getLogger(LOGGER_NAME)
    start = int(query[0])
    end = int(query[1])
    top_n = int(query[2])
    logger.debug('got query "%d,%d,%d"', start, end, top_n)
    filtered_posts = filter_posts(posts, start, end)
    scores = calc_words_scores(filtered_posts)
    # logger.debug('Choosing %d most popular words', top_n)
    res = sorted(scores.items(), key=lambda kv: (-kv[1], kv[0]))
    res_len = len(res)
    if res_len < top_n:
        logger.warning('not enough data to answer, found %d words out of %d for period '
                       '"%d,%d"', res_len, top_n, start, end)
        top_n = res_len
    res = res[:top_n]
    # logger.debug('Finished processing query')
    return res


def analysis(questions, stop_words, queries, log_filenames=True):
    logger = logging.getLogger(LOGGER_NAME)

    # if log_filenames:
    #     logger.debug("Reading XML dataset from file: %s", questions.name)
    questions = parse_questions(questions)
    if log_filenames:
        logger.info("process XML dataset, ready to serve queries")

    # if log_filenames:
    #     logger.debug("Reading stop words from file: %s", stop_words.name)
    stop_words = parse_stop_words(stop_words)

    # if log_filenames:
    #     logger.debug('Reading queries from file: %s', queries.name)
    queries = parse_queries(queries)

    # logger.info("Processing dataset")
    posts = []
    for question in questions:
        post = Post(question["Title"], question["PostTypeId"], question["Score"],
                    question["CreationDate"])
        post.transform_self()
        post.remove_stop_words(stop_words)
        posts.append(post)

    # logger.info("Serving queries")
    results = []
    for query in queries:
        results.append(process_query(query, posts))
    logger.info("finish processing queries")

    return results_to_json(results, queries)


def main(argv):
    logger = logging.getLogger(LOGGER_NAME)

    logger.setLevel(logging.DEBUG)

    all_handler = logging.FileHandler(filename='analyze_stackoverflow.log', mode='w')
    warn_handler = logging.FileHandler(filename='analyze_stackoverflow.warn', mode='w')

    all_handler.setLevel(logging.DEBUG)
    warn_handler.setLevel(logging.WARNING)

    all_handler.setFormatter(logging
                             .Formatter('%(asctime)s-%(name)s-%(levelname)s: %(message)s'))
    warn_handler.setFormatter(logging
                              .Formatter('%(asctime)s-%(name)s-%(levelname)s: %(message)s'))

    logger.addHandler(all_handler)
    logger.addHandler(warn_handler)

    path_dict = parse_command(argv)
    with open(path_dict['questions_path'], encoding='utf-8') as questions, \
            open(path_dict['stop_words_path'], encoding='koi8-r') as stop_words, \
            open(path_dict['queries_path']) as queries:
        results_json_lines = analysis(questions, stop_words, queries)
        # logger.info("Writing results to stdout")
        print(results_json_lines, flush=True)


if __name__ == "__main__":
    main(sys.argv[1:])
