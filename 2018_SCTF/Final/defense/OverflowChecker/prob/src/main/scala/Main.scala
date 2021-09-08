package SimpleDSL

import scopt._
import scala.io.Source
import scala.util.Random
import java.io.FileNotFoundException
import scala.io.StdIn

case class Config(file: Option[String] = None, license: Boolean = false)

object Main {
  def main(args: Array[String]) {
    val rd = new Random()
    val thres = 2147483647L
    val total = 100
    var count = 0
    val cmax = 5
    val flag = "SCTF{W0RLD_T0P_CL4S5_ANALYZ3R}"
    println("Welcome to OverflowChecker!!")
    println(f"You have to analyze $total%d programs which may have integer overflow")
    println(f"A program is considered as harmful when it can make integer overflow with specific input.")
    println(f"You must analyze all programs correctly in 300 seconds except $cmax%d failure.")
    for(i <- List.range(0, total))
    {
        println(f"$i%d/$total%d try")
        val target = rd.nextBoolean
        val delta = (rd.nextInt & (~(1 << 31))) % 100000000
        val sign = if (target) delta else -delta

        val (ast, name) = SDSLGenerator.createSafe(thres + sign, i / 10, 0, sign <= 0)
        println(SDSLCompiler.pp(ast, 0))
        println("(y : safe, n : overflow) > ")
        val answer = StdIn.readLine
        if(answer.length == 0) System.exit(0) else Unit
        if((answer(0) == 'y') != (sign <= 0)) 
        {
            count = count + 1
            println(f"fail (fail count : $count%d)")
            if (count >= cmax)
            {
                println(f"$count%d times failed!!")
                System.exit(0)
            }
        } else {
            println("good!")
        }
    }
    println(s"Finish! flag is $flag")
  }

  def executeFile(code: String) {
    SDSLCompiler(code) match {
        case Left(err) => println(err)
        case Right(ast) => SDSLInterpreter(ast)
    }
  }
}
