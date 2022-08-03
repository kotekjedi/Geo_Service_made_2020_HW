import breeze.linalg.{DenseMatrix, DenseVector, pinv, norm}

class LinearRegression(var weights: Option[DenseVector[Double]] = None) {
  def fit(X: DenseMatrix[Double], y: DenseVector[Double]): Unit = {
//    analytical solution for linear regression eq
//    pseudo-inverse mat @ target vec
    weights = Option(pinv(X) * y)
  }

  def predict(X: DenseMatrix[Double]): DenseVector[Double] = {
    try {
      X * weights.get
    } catch {
      case e: Exception =>
        print("Weights are not defined! Empty output!")
        DenseVector(0)
    }
  }

  def mse(y_true: DenseVector[Double], y_pred: DenseVector[Double]): Double = {
    norm(y_true - y_pred) / y_true.size
  }
}




