open Lambda.AST
open System

let sleep (t: int) =
  System.Threading.Thread.Sleep t

let esc = string (char 0x1B)

let consoleColorToChar = function
  | ConsoleColor.Blue -> esc + "[1;34m"
  | ConsoleColor.Green -> esc + "[1;32m"
  | ConsoleColor.Red -> esc + "[1;31m"
  | ConsoleColor.White -> esc + "[1m"
  | _ -> failwith "unhandled"

let consoleColor (fc : ConsoleColor) =
    let current = Console.ForegroundColor
    consoleColorToChar fc |> System.Console.Write
    { new IDisposable with
          member x.Dispose () = esc + "[0m" |> System.Console.Write }


let print (color: ConsoleColor option) (word: string) =
  match color with
  | Some (x) ->
      use c = consoleColor x in
      System.Console.Write word
  | None ->
      use c = consoleColor ConsoleColor.White in
      System.Console.Write word

let printn (color: ConsoleColor option) (word: string) =
  print color (word + "\n")


let flag = "SCTF{N0w_Y0u_C4n_C0de_W17h_7h3_F0und47i0n_0f_7h3_Func710n4l_L4ngu4g3:)}"
let rand = new System.Random()

let gentc len =
  let swap (a: _[]) x y =
    let tmp = a.[x]
    a.[x] <- a.[y]
    a.[y] <- tmp
  let shuffle len a =
    Array.iteri (fun i _ -> swap a i (rand.Next(i, Array.length a))) a
    a.[0..len]
  Array.append [| 1 .. 35 |] [| 1 .. 35 |] |> shuffle len

let unchurchList = function
  | Abs (a, b) ->
      match b with
      | App (App(Var(x), hd), tl) when a=x -> (hd, tl) |> Some
      | _ -> None
  | _ -> None

let check tc result =
  let answer = Array.sort tc |> Array.rev
  let rec checker (ans: int[]) res =
    if Array.length ans = 0 then
      match Lambda.Church.toInt res with
      | Some (x) when x=0 -> true
      | _ -> false
    else
      match unchurchList res with
      | Some (head, lt) ->
          match Lambda.Church.toInt head with
          | Some (x) when ans.[0] = x -> checker ans.[1..] lt
          | _ -> false
      | None -> false
  checker answer result

let rec launch prog tc =
  if Array.length tc = 0 then
    Lambda.Eval.evalWithTimeout (60 * 1000) (prog <<< (Lambda.Church.ofInt 0))
  else
    launch (prog <<< tc.[0]) tc.[1..]

let doTests maxiter prog =
  let rec unitTest iter =
    if iter = maxiter then
      true
    else
      let tc = gentc (11 + 3 * iter + rand.Next(-2, 3))
      sprintf "Test %d/%d" (iter+1) maxiter |> print (Some ConsoleColor.Blue)
      match Array.map Lambda.Church.ofInt tc |> launch prog with
      | Some (x) ->
          if check tc x then
            printn (Some ConsoleColor.Green) "  Pass"
            unitTest (iter + 1)
          else
            printn (Some ConsoleColor.Red) "  Fail"
            false
      | None ->
          printn (Some ConsoleColor.Red) "  Timeout!"
          false
  unitTest 0


[<EntryPoint>]
let main argv =
  printn None "Welcome to the λ: Power."
  sleep 1000
  printn None "\n[+] Your aim is to implement sorting algorithm with λ-calculus which satisfy following conditions.\n"
  printn (Some ConsoleColor.Green) "[-] Your program should accept non-fixed-length of inputs."
  printn (Some ConsoleColor.Green) "[-] End of input is marked with zero as church numeral."
  printn (Some ConsoleColor.Green) "[-] The result should be decreasing order."
  printn (Some ConsoleColor.Green) "[-] Your output should be list of church numeral formated like below."
  printn (Some ConsoleColor.Green) "[-] λz.(z (λf.λx.(f (f (f x)))) (λz.(z (λf.λx.(f (f x))) (λz.(z (λf.λx.(f x)) (λx.λy.y))))))"
  printn (Some ConsoleColor.Red) "[-] Timeout: 60s per test"
  sleep 1000
  print None "[λ]-> "

  try
    let result = System.Console.ReadLine ()
               |> Lambda.Util.parse
               |> doTests 3
    if result then
      printn None "Wow! Your equation SORT the number!"
      sleep 1000
      print None "This is reward for your awesome job: "
      sleep 1000
      printn (Some ConsoleColor.Green) flag
    else
      printn None "Your equation CANNOT SORT numbers!"
      printn None "Bye~"
  with
  | _ -> printn (Some ConsoleColor.Red) "Fail to parse"
  0
