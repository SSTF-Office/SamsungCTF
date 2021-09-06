package SimpleDSL

import scala.math.BigInt
import scala.util.Random

object SDSLGenerator {

  val rd = new Random()
  val delta = 65536
  val limit: Long = 2147483647
  val lowLimit: Long = 10
  val len = 8
  val forLimit = 2
  val forCountLimit = 200
  val forProbability: Double = 0.3


  def randRange(a: Long, b: Long): Long = {
      if (a == b) a else
    (rd.nextLong & (~(1L << 63))) % (b - a) + a
  }

  def randInt(a: Int, b: Int): Int = {
      if (a == b) a else
    (rd.nextInt & (~(1 << 31))) % (b - a) + a
  }

  def randVarname(): String = {
    "x_" + rd.alphanumeric.take(len).mkString
  }

  def getConstant(a: Int): Tuple2[SDSLAST, String] = {
    val iname = randVarname()
    (SetVar(iname, IntLiteral(a)), iname)
  }

  def getInput(a: Int): Tuple2[SDSLAST, String] = {
    var forwardRange : Int = if (delta < limit - a) delta else 2147483647 - a
    var backwardRange : Int = if (delta < a) delta else a
    val iname = randVarname()
    val nextFilter = randInt(0, 2) match {
        case 0 =>
        {
        IFElseStmt(
            GreatThan(
                GetVar(iname),
                IntLiteral(a)
            ), SetVar(iname, IntLiteral(
                randInt( a - backwardRange, a))), None
            )
        }
        case _ => {
            val c = randInt( a - backwardRange, a)
            val d = randInt( if(delta < c) c - delta else 0, c )
            val ndelta = a - c
            AndThen(
                IFElseStmt(
                    GreatThan(
                        GetVar(iname),
                        IntLiteral(c)
                    ), SetVar(iname, IntLiteral(d)), None),
                IFElseStmt(
                    LessEqual(
                        GetVar(iname),
                        IntLiteral( c + randInt(0, delta))),
                    SetVar(iname, Add(GetVar(iname), IntLiteral(ndelta))),
                    None)
            )
        }
    }
    (AndThen(
        SetVar(iname, FunctionCall("input",Nil)),
        nextFilter),
    iname)
  }

  def createConstantOrInput(a: Int): Tuple2[SDSLAST, String] = {
    if (rd.nextBoolean) getConstant(a) else getInput(a)
  }
  def createSafe(a: Long, n: Int, forCount: Int, safety: Boolean): Tuple2[SDSLAST, String] = {
    if ((n == 0 && a < limit) || a < lowLimit) {
      createConstantOrInput(a.toInt)
    } else {
      val d = rd.nextDouble
      (if (forCount > forLimit) 1.0 else d) match {
        case p if p > forProbability=> {
          val a1 = randRange(0, a)
          val a2 = a - a1
          val n1 = if (n > 0) randInt(0, n) else 0
          val n2 = n - n1
          val (x, varx) = createSafe(a1, n1, forCount, safety)
          val (y, vary) = createSafe(a2, n2, forCount, safety)
          val zname = randVarname()
          if (rd.nextBoolean)
            (AndThen(x, AndThen(y, SetVar(zname, Add(GetVar(varx), GetVar(vary))))), zname)
          else
            (AndThen(y, AndThen(x, SetVar(zname, Add(GetVar(vary), GetVar(varx))))), zname)
        }
        case _ => {
          val mthres = if (a > forCountLimit) forCountLimit else a
          val p = randRange(1, mthres)
          val q = if (safety) (a / p) else (a / p) + 1
          val (x, varx) = createSafe(p, 0, forCount, safety)
          val (y, vary) = createSafe(q, if (n > 0) n - 1 else 0, forCount + 1, safety)
          val iname = randVarname()
          val zname = randVarname()
          (
            AndThen(
              x,
              AndThen(
                SetVar(zname, IntLiteral(0)),
                ForLoop(iname, IntLiteral(0), iname, Add(GetVar(iname), IntLiteral(1)),
                  LessThan(GetVar(iname), GetVar(varx)), AndThen(y, SetVar(zname, Add(GetVar(zname), GetVar(vary))))))),
              zname)
        }
      }
    }
  }

}
