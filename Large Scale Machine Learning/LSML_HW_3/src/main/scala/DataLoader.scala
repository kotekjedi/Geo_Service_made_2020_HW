import java.io.{File, FileReader}

import breeze.io.CSVReader
import breeze.linalg.{Axis, DenseMatrix}

import scala.collection.mutable

object DataLoader {
//  map with possible fish types
  var categoricalDict: mutable.Map[Any, Int] = mutable.Map[Any, Int]()

  def loadData(filePath: String, train: Boolean = false): DataSet = {
//    loading data with one categorical column (first one) and target column (second one)
    var data = CSVReader.read(new FileReader(new File(filePath)), ',', '"', '\\', 1)
    data = data.takeWhile(line => line.nonEmpty && line.head.nonEmpty)

    if (train) data.map(x => categoricalDict.getOrElseUpdate(x.head, categoricalDict.size + 1))

    data = data.map(oneHotCast)

    val dataBreeze = DenseMatrix.tabulate(data.length, data.head.length)((i, j) => data(i)(j).toDouble)
    val y = dataBreeze(0 until dataBreeze.rows, categoricalDict.size)
    val X = dataBreeze.delete(Seq(categoricalDict.size), Axis._1)

    new DataSet(X, y)
  }

  def oneHotCast(line: IndexedSeq[String]): IndexedSeq[String] = {
//    one-hot encoding for the first column (fish type)
    val oneHotPos: Option[Int] = categoricalDict.get(line.head)
    val oneHotEncoding: Seq[Any] =
      Seq.fill(oneHotPos.get - 1)(0) ++ Seq(1) ++ Seq.fill(categoricalDict.size - oneHotPos.get)(0)
    line.tail.prependedAll(oneHotEncoding.map(x => x.toString))
  }
}
