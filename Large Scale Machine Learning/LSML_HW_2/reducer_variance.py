def reducer_variance(variances):
    counts, var, mean = 0, 0, 0
    for c, v, m in variances:
        var = (c*v + var*counts)/(c+counts) + c*counts*((m - mean)/(c+counts))**2
        mean = (mean*counts + c*m)/(counts+c)
        counts += c
    return var