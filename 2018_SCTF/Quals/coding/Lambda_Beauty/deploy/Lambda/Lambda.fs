namespace Lambda
open AST

module Util =
  open Microsoft.FSharp.Text.Lexing

  let parse expr =
    let res = LexBuffer<char>.FromString expr
            |> Parser.start Lexer.read
    match res with
    | None -> failwith "Parse Error"
    | Some x -> x

  let rec pp = function
    | Var (v) -> v
    | Abs (v, body) -> sprintf "λ%s.%s" v (pp body)
    | App (a, b) -> sprintf "(%s) (%s)" (pp a) (pp b)


module Eval =
  open System.Threading.Tasks
  open System

  let rec get_fv = function
    | Var (v) -> [v]
    | Abs (s, t) -> List.filter (fun x-> x <> s) (get_fv t)
    | App (t, u) ->
        let f1 = get_fv t
        let f2 = get_fv u
        set (List.append f1 f2) |> List.ofSeq

  let rec rename n l =
    if List.exists (fun x -> x = n) l then
      rename (n + "'") l
    else
      n

  let rec subst var term = function
    | Var (s) when s = var -> term
    | Var (s) -> Var (s)
    | Abs (s, t) ->
      let f_t = get_fv t
      let f_term = get_fv term
      if (s = var) then
        Abs (s, t)
      elif not (List.exists (fun x -> x = var) f_t) then
        Abs (s, t)
      elif List.exists (fun x -> x = s) f_term then
        let f = rename s (List.append f_t f_term)
        Abs (f, subst var term t)
      else
        Abs (s, subst var term t)
    | App (t, u) -> App (subst var term t, subst var term u)

  let reduxible = function
    | App (Abs(_, _), _) -> true
    | _ -> false


  let rec eval_step t =
    if reduxible t then
      ``β-reduction`` t
    else
      match t with
      | Var (s) -> Var(s)
      | Abs (s, t) -> Abs(s, eval_step t)
      | App (t, u) ->
          let n_t = eval_step t
          if n_t = t then App (t, eval_step u) else App (n_t, u)
  and ``β-reduction`` = function
    | App (Abs(s, t), u) -> subst s u t
    | _ as t -> t

  let rec eval t =
    // When state is stable, return
    let n_t = eval_step t
    if n_t = t then t else eval n_t

  let evalWithTimeout timeout expr =
    let task = Task.Run(fun () -> eval expr)
    if task.Wait (TimeSpan.FromMilliseconds (float (timeout))) then
      task.Result |> Some
    else
      None

module Church =
  let rec private count num v1 v2 = function
    | Var (v) when v=v2 -> num |> Some
    | App (v, body) ->
        match v with
        | Var (v) when v=v1 -> count (num + 1) v1 v2 body
        | _ -> None
    | _ -> None

  let toInt = function
    | Abs (v1, body) ->
        match body with
        | Abs (v2, body) when v1<>v2 -> count 0 v1 v2 body
        | _ -> None
    | _ -> None

  let ofInt (i: int) =
    let rec apply i =
      if i = 0 then (Var "x") else Var "f" <<< (apply (i - 1))
    Abs ("f", Abs ("x", apply (int i)))

  let ofString (s: string) =
    let encoder acc elem =
      Abs("x", Abs("y", Abs("z", Var("z") <<< Var("x") <<< Var("y"))))
      <<< (ofInt elem) <<< acc

    let castBitArr (x: char) =
      let x = int(x)
      Array.init 8 (fun i -> (x >>> i) &&& 1)

    s.ToCharArray ()
    |> Array.map castBitArr
    |> Array.fold (fun acc x -> Array.concat [x; acc]) Array.empty
    |> Array.fold encoder (Abs("x", Abs("y", Var("y"))))
