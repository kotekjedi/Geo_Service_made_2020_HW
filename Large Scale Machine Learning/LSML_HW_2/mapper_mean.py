import pandas as pd


def mapper_mean(chunk: pd.DataFrame):
    counts = chunk.shape[0]
    mean = chunk.price.mean()

    return counts, mean
