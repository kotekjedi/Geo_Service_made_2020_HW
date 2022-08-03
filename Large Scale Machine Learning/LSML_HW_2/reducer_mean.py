def reducer_mean(means):
    nominator = 0
    denominator = 0
    for key, value in means:
        nominator += key * value
        denominator += key
    return nominator / denominator
