package SimpleDSL

import org.scalatest.FunSuite
import scala.collection.immutable.HashMap

class Test1 extends FunSuite {

    def check(src: String, cond: HashMap[String, Int] => Boolean) {
    val flag = SDSLCompiler(src) match {
      case Left(err) => false
      case Right(ast) => cond(SDSLInterpreter.interp(ast, HashMap()))
    }
    assert(flag)
  }

  test("basic assign"){
    val src = """
        x <- 1;
        """
    val cond = (i: HashMap[String, Int]) => i("x") == 1
    check(src, cond)
  }

  test("add 1"){
      val src = """
      x <- 10;
      y <- 100;
      x <- ( x + y );
      """
      val cond = (i: HashMap[String, Int]) => i("x") == 110
      check(src, cond)
  }


}
