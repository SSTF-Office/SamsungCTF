module repl

let showResult res =
  match Lambda.Church.toInt res with
  | Some x ->
      sprintf "%d: %s" x (Lambda.Util.pp res)
      |> System.Console.WriteLine
  | None ->
      sprintf "Object: %s" (Lambda.Util.pp res)
      |> System.Console.WriteLine

let rec repl () =
  System.Console.Write ("[λ]-> ")
  match System.Console.ReadLine () with
  | "exit" | "Exit" -> 0
  | _ as e ->
      try
        let res = Lambda.Util.parse e |> Lambda.Eval.evalWithTimeout 5000
        match res with
        | Some x -> showResult x
        | None -> System.Console.WriteLine "Timeout"
      with
        | _ -> System.Console.WriteLine "Illegal input"
      repl()


[<EntryPoint>]
let main argv =
  repl ()
