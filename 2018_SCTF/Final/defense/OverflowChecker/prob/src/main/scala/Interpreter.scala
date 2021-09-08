package SimpleDSL
import scala.collection.immutable.HashMap
import scala.annotation.tailrec
import scala.io.StdIn

object SDSLInterpreter
{

    def interp(ast: SDSLAST, env: HashMap[String, Int]) : HashMap[String, Int] = {
        ast match {
            case AndThen(s1, s2) => interp(s2, interp(s1, env))
            case SetVar(x, exp) => env + (x -> interp_intexp(exp, env))
            case ForLoop(x1, v1, x2, v2, cond, body) => {
                val env1 = env + (x1 -> interp_intexp(v1, env))
                interp_forLoop(x2, v2, cond, body, env1)
            }
            case IFElseStmt(cond, ifcase, elsecase) => if (interp_boolexp(cond, env)) interp(ifcase, env) else elsecase.map((i) => interp(i, env)).getOrElse(env)
            case ExpStmt(exp) => interp_intexp(exp, env);env
        }
    }

    def interp_intexp(exp: SDSLIntEXP, env: HashMap[String, Int]) : Int  = {

        exp match {
            case IntLiteral(value) => value
            case Add(v1, v2) =>
            {
                val a1 = interp_intexp(v1, env)
                val a2 = interp_intexp(v2, env)
                val check = a1.toLong + a2.toLong
                if (check > Int.MaxValue) {
                    throw new ArithmeticException("int overflow")
                }
                a1 + a2
            }
            case GetVar(vname) =>
                env(vname)
            case FunctionCall(fname, args) =>
            {
                fname match {
                    case "input" => {
                        val x = StdIn.readLine().toInt
                        if (x < 0) 0 else x
                    }
                    case "print" => args.foreach((i) => println(interp_intexp(i, env)));args.length
                    case _ => 0
                }
            }
        }
    }


    def interp_boolexp(exp: SDSLBoolEXP, env: HashMap[String, Int]) : Boolean  = {
        exp match { 
            case Equal(v1, v2) => interp_intexp(v1, env) == interp_intexp(v2, env)
            case LessThan(v1, v2) => interp_intexp(v1, env) < interp_intexp(v2, env)
            case LessEqual(v1, v2) => interp_intexp(v1, env) <= interp_intexp(v2, env)
            case GreatThan(v1, v2) => interp_intexp(v1, env) > interp_intexp(v2, env)
            case GreatEqual(v1, v2) => interp_intexp(v1, env) >= interp_intexp(v2, env)

        }
    }

    @tailrec
    def interp_forLoop(x: String, v: SDSLIntEXP, cond: SDSLBoolEXP,
        body: SDSLAST,
        env: HashMap[String, Int]): HashMap[String, Int] = {
        if (interp_boolexp(cond, env))
        {
            val bodyenv = interp(body, env)
            interp_forLoop(x, v, cond, body, bodyenv + (x -> interp_intexp(v, bodyenv)))
        }
        else
            env
    }

    def apply(ast: SDSLAST) {
        val env: HashMap[String, Int] = HashMap()
        interp(ast, env)
    }
}
