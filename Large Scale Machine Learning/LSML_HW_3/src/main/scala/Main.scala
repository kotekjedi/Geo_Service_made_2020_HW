import java.io.File

import breeze.linalg.csvwrite

object Main {
  def main(args: Array[String]): Unit = {
//    loading fish splited fish data source: https://www.kaggle.com/aungpyaeap/fish-market
    val trainDataSet = DataLoader.loadData("src/main/scala/Fish_train.csv", train = true)
    val testDataSet = DataLoader.loadData("src/main/scala/Fish_test.csv")

    val linReg = new LinearRegression

    linReg.fit(trainDataSet.features, trainDataSet.target)

    print("Train error:")
    println(linReg.mse(trainDataSet.target, linReg.predict(trainDataSet.features)))

    print("Test error:")
    println(linReg.mse(testDataSet.target, linReg.predict(testDataSet.features)))

    csvwrite(new File("src/main/scala/out.csv"), linReg.predict(testDataSet.features).toDenseMatrix)
  }
}
