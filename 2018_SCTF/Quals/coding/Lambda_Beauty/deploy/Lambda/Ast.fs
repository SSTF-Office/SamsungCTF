module Lambda.AST

type ast =
  | App of ast * ast
  | Abs of string * ast
  | Var of string

let (<<<) x y = App (x, y)
