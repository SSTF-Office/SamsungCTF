package SimpleDSL

import scala.util.parsing.combinator._
import scala.util.parsing.input._

sealed trait SDSLToken

case class IDENTIFIER(str: String) extends SDSLToken
case class LITERAL(int: Int) extends SDSLToken
case object APPLY extends SDSLToken
case object EQ extends SDSLToken
case object GT extends SDSLToken
case object GE extends SDSLToken
case object LT extends SDSLToken
case object LE extends SDSLToken
case object PLUS extends SDSLToken
case object FOR extends SDSLToken
case object SEMICOLON extends SDSLToken
case object IF extends SDSLToken
case object ELSE extends SDSLToken
case object LBRAC extends SDSLToken
case object RBRAC extends SDSLToken
case object LDBRAC extends SDSLToken
case object RDBRAC extends SDSLToken
case object COMMA extends SDSLToken

trait SDSLCompilationError
case class SDSLLexerError(msg: String) extends SDSLCompilationError
case class SDSLParserError(msg: String) extends SDSLCompilationError

object SDSLLexer extends RegexParsers {
  override def skipWhitespace = true
  override val whiteSpace = "[ \t\r\f\n]+".r

  def identifier: Parser[IDENTIFIER] = {
    "[a-zA-Z_][a-zA-Z0-9_]*".r ^^ { str => IDENTIFIER(str) }
  }

  def literal: Parser[LITERAL] = {
    "[0-9]+".r ^^ { str => LITERAL(str.toInt) }
  }

  def apply = "<-" ^^ { _ => APPLY }

  def equal = "=" ^^ { _ => EQ }
  def gt = ">" ^^ { _ => GT }
  def ge = ">=" ^^ { _ => GE }
  def lt = "<" ^^ { _ => LT }
  def le = "<=" ^^ { _ => LE }
  def plus = "+" ^^ { _ => PLUS }
  def for_t = "for" ^^ { _ => FOR }
  def semicolon = ";" ^^ { _ => SEMICOLON }
  def if_t = "if" ^^ { _ => IF }
  def else_t = "else" ^^ { _ => ELSE }
  def lbrac = "(" ^^ { _ => LBRAC }
  def rbrac = ")" ^^ { _ => RBRAC }
  def ldbrac = "{" ^^ { _ => LDBRAC }
  def rdbrac = "}" ^^ { _ => RDBRAC }
  def comma = "," ^^ { _ => COMMA }

  def tokens: Parser[List[SDSLToken]] = {
    phrase(rep1(apply | equal | ge | le | gt | lt 
      | plus | for_t | semicolon | if_t | else_t | lbrac | rbrac
      | ldbrac | rdbrac | comma | identifier | literal))
  }

  def apply(code: String): Either[SDSLLexerError, List[SDSLToken]] = {
    parse(tokens, code) match {
      case NoSuccess(msg, next) => Left(SDSLLexerError(msg))
      case Success(result, next) => Right(result)
    }
  }

}
sealed trait SDSLAST
case class AndThen(step1: SDSLAST, step2: SDSLAST) extends SDSLAST
case class SetVar(varname: String, exp: SDSLIntEXP) extends SDSLAST
case class ForLoop(x1: String, v1: SDSLIntEXP, x2: String, v2: SDSLIntEXP,
    cond: SDSLBoolEXP, loopbody: SDSLAST) extends SDSLAST
case class IFElseStmt(cond: SDSLBoolEXP, ifcase: SDSLAST, elsecase: Option[SDSLAST]) extends SDSLAST
case class ExpStmt(exp: SDSLIntEXP) extends SDSLAST

sealed trait SDSLIntEXP
case class IntLiteral(value: Int) extends SDSLIntEXP
case class Add(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLIntEXP
case class GetVar(varname: String) extends SDSLIntEXP
case class FunctionCall(fname: String, args: List[SDSLIntEXP]) extends SDSLIntEXP

sealed trait SDSLBoolEXP
case class Equal(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLBoolEXP
case class LessThan(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLBoolEXP
case class LessEqual(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLBoolEXP
case class GreatThan(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLBoolEXP
case class GreatEqual(v1: SDSLIntEXP, v2: SDSLIntEXP) extends SDSLBoolEXP


object SDSLParser extends Parsers {
  override type Elem = SDSLToken

  class SDSLTokenReader(tokens: Seq[SDSLToken]) extends Reader[SDSLToken] {
    override def first: SDSLToken = tokens.head
    override def atEnd: Boolean = tokens.isEmpty
    override def pos: Position = NoPosition
    override def rest: Reader[SDSLToken] = new SDSLTokenReader(tokens.tail)
  }

  private def identifier: Parser[IDENTIFIER] = {
      accept("identifier", { case id @ IDENTIFIER(name) => id })
  }

  private def literal: Parser[LITERAL] = {
      accept("Int literal", { case lit @ LITERAL(value) => lit })
  }

  def program: Parser[SDSLAST] = {
    phrase(block)
  }

  def block: Parser[SDSLAST] = {
    rep1(statement) ^^ { case stmtList => stmtList reduceRight AndThen }
  }

  def intexp: Parser[SDSLIntEXP] = {
      val intlit = literal ^^ { case LITERAL(value) => IntLiteral(value) }
      val intadd = LBRAC ~ intexp ~ PLUS ~ intexp ~ RBRAC ^^ { case _~ exp1 ~ _ ~ exp2 ~_ => Add(exp1, exp2) }
      val funcall = identifier ~ LBRAC ~ rep(intexp ~ COMMA) ~ opt(intexp) ~ RBRAC^^
      {
          case IDENTIFIER(fname) ~ _ ~ inputs ~ lastinput ~ _ => 
          {val aList = inputs.map( (i) => i match
              {
                  case iex ~ _ => iex
              })
              FunctionCall(fname,
            if (lastinput.isEmpty) aList else aList :+ lastinput.get
          )
          }
      }
      val intget = identifier ^^ { case IDENTIFIER(name) => GetVar(name) }
      funcall | intget | intadd | intlit
  }

  def boolexp: Parser[SDSLBoolEXP] = {
      val equal = intexp ~ EQ ~ intexp ^^ {
          case v1 ~ _ ~ v2 => Equal(v1, v2)
      }
      val lesst = intexp ~ LT ~ intexp ^^ {
          case v1 ~ _ ~ v2 => LessThan(v1, v2)
      }
      val lesse = intexp ~ LE ~ intexp ^^ {
          case v1 ~ _ ~ v2 => LessEqual(v1, v2)
      }
      val greatt = intexp ~ GT ~ intexp ^^ {
          case v1 ~ _ ~ v2 => GreatThan(v1, v2)
      }
      val greate = intexp ~ GE ~ intexp ^^ {
          case v1 ~ _ ~ v2 => GreatEqual(v1, v2)
      }
      equal | lesst | lesse | greatt | greate
  }
  def statement: Parser[SDSLAST] = {
      val setvar = identifier ~ APPLY ~ intexp ~ SEMICOLON ^^ {
          case IDENTIFIER(vname) ~ _ ~ expression ~ _ => SetVar(vname, expression)
      }
      val forLoop = FOR ~ LBRAC ~ identifier ~ APPLY ~ intexp ~ 
      SEMICOLON ~ boolexp ~ SEMICOLON ~ identifier ~ APPLY ~ intexp ~ 
      RBRAC ~ LDBRAC ~ rep(statement) ~ RDBRAC ^^ {
          case _ ~ _ ~ IDENTIFIER(x1) ~ _ ~ v1 ~ _ ~ cond ~ _ ~ 
          IDENTIFIER(x2) ~ _ ~ v2 ~ _~ _ ~ stmt ~ _ => 
                  ForLoop(x1,v1,x2,v2,cond,(stmt reduceRight AndThen) )
      }
      val iex = intexp ~ SEMICOLON ^^ {
          case exp ~ _ => ExpStmt(exp)
      }
      val ifElse = IF ~ LBRAC ~ boolexp ~ RBRAC ~ LDBRAC ~ rep(statement) ~ 
      RDBRAC ~ opt( ELSE ~ LDBRAC ~ rep(statement) ~ RDBRAC) ^^ {
          case _ ~ _ ~ cond ~ _ ~ _ ~ bodytrue ~ _ ~ elsebody =>
          {
              val bodyfalse = for ( elseformat <- elsebody ) yield (
              elseformat match
              {
                  case _ ~ _ ~ bf ~ _ => bf reduceRight AndThen
              }
              )
              IFElseStmt(cond, bodytrue reduceRight AndThen, bodyfalse)
          }
      }
      setvar | forLoop | iex | ifElse
  }

  def apply(tokens: Seq[SDSLToken]): Either[SDSLParserError, SDSLAST] = {
      val reader = new SDSLTokenReader(tokens)
      program(reader) match {
        case NoSuccess(msg, next) => Left(SDSLParserError(msg))
        case Success(result, next) => Right(result)
      }
  }
}

object SDSLCompiler {
  def apply(code: String): Either[SDSLCompilationError, SDSLAST] = {
    for {
      tokens <- SDSLLexer(code).right
      ast <- SDSLParser(tokens).right
    } yield ast
  }

  def pp(ast: SDSLAST, indent: Int) : String = {
    ast match {
        case AndThen(a, b) => pp(a, indent) + "\n" + pp(b, indent)
        case SetVar(x, exp) => (" " * indent) + x + " <- " + ppexp(exp) + ";"
        case ForLoop(x1, v1, x2, v2, cond, stmts) => (" " * indent) +
         "for( " + x1 + " <- " + ppexp(v1) + "; " + ppbexp(cond) + "; " +
         x2 + " <- " + ppexp(v2) + ") {\n" + pp(stmts, indent + 2) + "\n" +
         (" " * indent) + "}"
        case ExpStmt(exp) => (" " * indent) + ppexp(exp) + ";"
        case IFElseStmt(cond, exptrue, expfalse) => {
            val ifcase = (" " * indent) +
        "if( " + ppbexp(cond) +" ) {\n" + pp(exptrue, indent + 2) + "\n" +
        (" " * indent) + "}"
        val elsecase = expfalse.map((exp) => " else {\n" + pp(exp, indent + 2) + 
            "\n" + (" " * indent) + "}").getOrElse("")
        ifcase + elsecase
        }
    }
  }

  def ppexp(exp: SDSLIntEXP) : String = {
      exp match {
          case IntLiteral(a) => a.toString
          case GetVar(vname) => vname
          case Add(v1, v2) => "( " + ppexp(v1) + " + " + ppexp(v2) + " )"
          case FunctionCall(fname, arglist) => fname + "(" + 
          arglist.map((i) => ppexp(i)).mkString(", ") +")"
      }
  }

  def ppbexp(exp: SDSLBoolEXP) : String = {
      exp match {
          case Equal(v1, v2) => ppexp(v1) + " = " + ppexp(v2)
          case LessThan(v1, v2) => ppexp(v1) + " < " + ppexp(v2)
          case LessEqual(v1, v2) => ppexp(v1) + " <= " + ppexp(v2)
          case GreatThan(v1, v2) => ppexp(v1) + " > " + ppexp(v2)
          case GreatEqual(v1, v2) => ppexp(v1) + " >= " + ppexp(v2)
      }
  }
}

