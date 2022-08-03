import sys
import pandas as pd

import mapper_mean, mapper_variance, reducer_mean, reducer_variance


def solid_estimation(path):
    df = pd.read_csv(path)
    counts = df.shape[0]
    mean = df.price.mean()
    variance = df.price.var() * (counts - 1) / counts

    return mean, variance


def distibuted_estimation(path, chunksize=128):
    chunks = pd.read_csv(path, chunksize=chunksize, iterator=True)

    context_mean = []
    context_var = []

    for chunk in chunks:
        counts_mean, mean = mapper_mean.mapper_mean(chunk)
        context_mean.append((counts_mean, mean))

        counts_var, variance = mapper_variance.mapper_variance(chunk)
        context_var.append((counts_var, variance, mean))

    return reducer_mean.reducer_mean(context_mean), reducer_variance.reducer_variance(context_var)


def main():

    try:
        path = sys.argv[2]

    except BaseException:
        path = "./AB_NYC_2019.csv"

    if sys.argv[1] == "solid":
        mean, variance = solid_estimation(path)

    elif sys.argv[1] == "dist":
        mean, variance = distibuted_estimation(path)

    else:
        print(sys.argv[1], " is unknown")
        mean, variance = "error", "error"

    open("results.txt", "a").write(f"mean {mean}; var {variance}; method {sys.argv[1]}\n")


if __name__ == "__main__":
    main()
