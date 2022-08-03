import logging
import unittest
import copy
import io

import task_Panfilov_Alexander_analyze_stackoverflow as task


class TestTask(unittest.TestCase):

    stop_words = {'first', 'of', 'last', 'on', 'and',
                  'the', 'in', 'how',
                  'do', 'from', 'a',
                  'to', 'i', 'for', 'why', 'be',
                  'someone', 'must', 'an'}

    queries = '2008,2009,12\n' \
              '2003,2019,20\n' \
              '2005,2020,10\n' \
              '2007,2009,3\n'

    posts = [
        task.Post(
            'Setting setting sTyle style on on first first and'
            ' and last lAst vIsible visible TabItem tabItem',
            '1', '5', '2008-10-15T00:44:56.847'
        ),
        task.Post(
            'Setting on first first and and last lAst'
            ' TabItem tabItem of oF TabControl tabControl',
            '1', '7', '2008-10-15T00:48:10.124'

        ),
        task.Post(
            None,
            '2', '18', '2008-10-15T00:48:10.124'

        )
    ]

    questions = '<row Id="201905" PostTypeId="2" ParentId="201323" ' \
                'CreationDate="2008-10-14T16:35:44.180"' \
                ' Score="38" OwnerDisplayName="adnam" LastActivityDate="2008-10-14T16:35:44.180" ' \
                'CommentCount="3"' \
                ' CommunityOwnedDate="2009-12-16T00:37:16.437" />' \
                '\n<row Id="20358" PostTypeId="1" CreationDate="2008-10-15T00:44:56.847" ' \
                'Score="5" ' \
                'ViewCount="239"' \
                ' OwnerUserId="28074" LastEditorUserId="66708" LastEditorDisplayName="Jexia"' \
                ' LastEditDate="2010-10-15T00:37:22.290" ' \
                'LastActivityDate="2013-02-06T17:45:43.123" ' \
                'Title="Setting setting sTyle style on on first first and and last lAst' \
                ' vIsible visible' \
                ' TabItem tabItem" Tags="&lt;.net&gt;&lt;wpf&gt;&lt;xaml&gt;&lt;' \
                'tabcontrol&gt;&lt;tabitem&gt;"' \
                ' AnswerCount="3" CommentCount="2" FavoriteCount="1" />' \
                '\n<row Id="278123" PostTypeId="1" CreationDate="2008-10-15T00:48:10.124"' \
                ' Score="7" ViewCount="2239"' \
                ' Body="approach this?&lt;/p&gt;&#xA;" OwnerUserId="28074"' \
                ' LastEditorUserId="66708"' \
                ' LastEditorDisplayName="Jexia" LastEditDate="2010-10-15T00:37:22.290"' \
                ' LastActivityDate="2013-02-06T17:45:43.123" Title="Setting on first' \
                ' first and and last lAst' \
                ' TabItem tabItem of oF TabControl tabControl" Tags="&lt;.net&gt;&lt;wpf&gt;' \
                '&lt;xaml&gt;&lt;' \
                'tabcontrol&gt;&lt;tabitem&gt;" CommentCount="2" FavoriteCount="1"/>'

    stop_words_str = 'first\nof\nlast\non\nand\n' \
                     'the\nin\nhow\n' \
                     'do\nfrom\ni\nin\nhow\na\n' \
                     'to\ndo\ni\nin\nhow\na\n' \
                     'for\nwhy\nbe\nsomeone\nmust\nan\nto\n'

    transformed_posts = None

    @classmethod
    def setUp(cls):
        cls.transformed_posts = copy.deepcopy(cls.posts)
        logger = logging.getLogger("task")
        logger.setLevel(logging.ERROR)
        for post in cls.transformed_posts:
            post.transform_self()
            post.remove_stop_words(cls.stop_words)

    def test_parse_questions(self):
        pseudo_file = io.StringIO(self.questions)
        questions = task.parse_questions(pseudo_file)
        zero_question = {
            'PostTypeId': '2',
            'CreationDate': '2008-10-14T16:35:44.180',
            'Score': '38',
            'Title': None
        }
        first_question = {
            'PostTypeId': '1',
            'CreationDate': '2008-10-15T00:44:56.847',
            'Score': '5',
            'Title': 'Setting setting sTyle style on on first '
                     'first and and last lAst vIsible visible TabItem tabItem'
        }

        self.assertDictEqual(questions[0], zero_question)
        self.assertDictEqual(questions[1], first_question)

    def test_parse_queries(self):
        queries_true = [[2008, 2009, 12],
                        [2003, 2019, 20],
                        [2005, 2020, 10],
                        [2007, 2009, 3]]

        pseudo_file = io.StringIO(self.queries)
        queries = task.parse_queries(pseudo_file)
        self.assertListEqual(queries, queries_true)

    def test_parse_stop_words(self):
        pseudo_file = io.StringIO(self.stop_words_str)
        stop_words = task.parse_stop_words(pseudo_file)
        self.assertSetEqual(stop_words, self.stop_words)

    def test_results_to_json(self):
        res_json_lines_true = '{"start": 2007, "end": 2009, "top": [["setting", 12], ' \
                              '["tabitem", 12], ["tabcontrol", 7]]}\n'
        query = [2007, 2009, 3]
        res = [('setting', 12), ('tabitem', 12), ('tabcontrol', 7)]
        res_json_lines = task.results_to_json([res], [query])
        self.assertEqual(res_json_lines, res_json_lines_true)

    def test_parse_command(self):
        test_samples_dir = './sample/test'
        test_questions = f'{test_samples_dir}/questions.xml'
        test_stop_words = f'{test_samples_dir}/stop_words.txt'
        test_queries = f'{test_samples_dir}/queries.csv'

        command = ['--questions', test_questions,
                   '--stop-words', test_stop_words,
                   '--queries', test_queries]
        res_true = {
            'questions_path': test_questions,
            'stop_words_path': test_stop_words,
            'queries_path': test_queries
        }

        res = task.parse_command(command)

        self.assertDictEqual(res, res_true)

    def test_post_transform_self(self):
        res_true = {'setting', 'style', 'on', 'first', 'and',
                    'last', 'visible', 'tabitem'}

        post = copy.deepcopy(self.posts[0])
        post.transform_self()
        self.assertEqual(post.type_id, 1)
        self.assertEqual(post.score, 5)
        self.assertSetEqual(post.title_words, res_true)
        self.assertEqual(post.type_id, 1)

    def test_post_remove_stop_words(self):
        res_true = {'setting', 'style', 'visible', 'tabitem'}
        post = copy.deepcopy(self.posts[0])
        post.transform_self()
        res = post.remove_stop_words(stop_words=self.stop_words)
        self.assertSetEqual(res, res_true)

    def test_post_filter(self):
        post = self.transformed_posts[1]
        self.assertTrue(task.post_filter(post, 2007, 2010, 1))
        self.assertTrue(task.post_filter(post, 2008, 2010, 1))
        self.assertTrue(task.post_filter(post, 2007, 2008, 1))
        self.assertFalse(task.post_filter(post, 2007, 2010, 2))
        self.assertFalse(task.post_filter(post, 2006, 2007, 1))
        self.assertFalse(task.post_filter(post, 2009, 2011, 1))

    def test_calc_words_score(self):
        scores_true = {
            'setting': 12,
            'style': 5,
            'visible': 5,
            'tabitem': 12,
            'tabcontrol': 7
        }
        posts = self.transformed_posts
        posts = task.filter_posts(posts, 2007, 2018)
        scores = task.calc_words_scores(posts)
        self.assertDictEqual(scores_true, scores)

    def test_process_query(self):
        query = [2007, 2009, 3]
        res_true = [
            ('setting', 12),
            ('tabitem', 12),
            ('tabcontrol', 7)
        ]

        res = task.process_query(query, self.transformed_posts)
        self.assertListEqual(res, res_true)

    def test_analysis(self):
        res_true = '{"start": 2008, "end": 2009, "top": [["setting", 12], ' \
                   '["tabitem", 12], ["tabcontrol", 7], ["style", 5], ["visible", 5]]}\n' \
                   '{"start": 2003, "end": 2019, "top": [["setting", 12], ["tabitem", 12], ' \
                   '["tabcontrol", 7], ["style", 5], ["visible", 5]]}\n' \
                   '{"start": 2005, "end": 2020, "top": [["setting", 12], ["tabitem", 12], ' \
                   '["tabcontrol", 7], ["style", 5], ["visible", 5]]}\n' \
                   '{"start": 2007, "end": 2009, "top": [["setting", 12], ' \
                   '["tabitem", 12], ["tabcontrol", 7]]}\n'

        pseudo_questions = io.StringIO(self.questions)
        pseudo_queries = io.StringIO(self.queries)
        pseudo_words = io.StringIO(self.stop_words_str)
        res = task.analysis(pseudo_questions, pseudo_words, pseudo_queries, log_filenames=False)
        self.assertEqual(res, res_true)


if __name__ == '__main__':
    unittest.main()
