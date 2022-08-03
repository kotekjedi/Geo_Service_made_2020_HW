import pandas as pd


def mapper_variance(chunk: pd.DataFrame):
    counts = chunk.shape[0]
    var = chunk.price.var() * (counts - 1) / counts

    return counts, var
