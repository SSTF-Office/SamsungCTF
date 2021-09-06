open Lambda.AST

let flag = "SCTF{S0_L0ng_4nd_7h4nks_f0r_A11_7h3_L4mbd4}"

[<EntryPoint>]
let main argv =
  System.Console.Write ">>> "
  try
    let r = System.Console.ReadLine () |> Lambda.Util.parse
    let result = r <<< (Lambda.Church.ofString flag)
               |> Lambda.Eval.evalWithTimeout 3000
    match result with
    | Some (x) ->
        System.Console.WriteLine "The result seems to be Object!"
        System.Console.WriteLine "Good Job!"
    | None ->
        System.Console.WriteLine "Timeout!"
  with
    | _ -> System.Console.WriteLine "Illegal input"
  0
