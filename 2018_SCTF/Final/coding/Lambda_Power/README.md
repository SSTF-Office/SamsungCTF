# λ: Power (Extreme)

## Description

```
λ is beautiful!
λ is powerful!
So I like λ:)

In qualification, you successfully found the beauty of the λ.
It is time to find the power of the λ.

nc poweroflambda.eatpwnnosleep.com 42

You can get interpreter at here.

Hint) Partial leak of judge system:

let rec launch prog tc =
  if Array.length tc = 0 then
    Lambda.Eval.evalWithTimeout (60 * 1000) (prog <<< (Lambda.Church.ofInt 0))
  else
    launch (prog <<< tc.[0]) tc.[1..]

/// Main testing function (2018-08-26 22:18:27)
let doTests maxiter prog =
  ... Some codes here ...
  let tc: int[] = generateTc ()
  match Array.map Lambda.Church.ofInt tc |> launch prog with
   ... Some codes here ...
```

## How to solve

1. Defining basic values.

From [Wiki](https://en.wikipedia.org/wiki/Lambda_calculus)[pedia](https://en.wikipedia.org/wiki/Church_encoding),
you can easily define below values.

- TRUE = '(λx.λy.x)'
- NIL = FALSE = '(λx.λy.y)'
- ISNIL = '(λl.l (λh.λt.λd.{FALSE}) {TRUE})'
- ISZERO = '(λn.n (λx.{FALSE}) {TRUE})'
- PRED = '(λn.λf.λx.n (λg.λh.h (g f)) (λu.x) (λu.u))'
- MINUS = '(λm.λn.n {PRED} m)'
- CONS = '(λx.λy.λz.z x y)'
- LEQ = '(λm.λn.{ISZERO} ({MINUS} m n))'
- Y = '(λf.(λx.f (x x)) (λx.f (x x)))'

Y is the [fixed-point combinator](https://en.wikipedia.org/wiki/Fixed-point_combinator).

2. Read until 0

Using Y combinator, you can read integer until 0: `(Y (λf.λn.λk.(ISZERO k) (cont n) (f (CONS k n))) NIL)`

(cont is the procedure that runs after reading n integers.)

3. Sort

Using operators, you can easily write sorting algorithm with λ Calculus.

popMinValueFromList: `(λh.λt.λcont.λret.(Y (λf.λn.λl.λr.(ISNIL l) (r (cont (CONS n ret))) (l (λh.λt.((LEQ h n) (f h t (CONS n r)) (f n t (CONS h r))))))) h t NIL)`

sort: `(λh.λt.(Y (λf.λr.λh.λt.(ISNIL t) (CONS h r) (popMinValueFromList h t f r))) NIL h t)`

4. Optimization

Sorting algorithm may be timed out due to the lazy-evaluation.

To optimize this, you should transform the formula to [CPS](https://en.wikipedia.org/wiki/Continuation-passing_style) using the fact: `(n cont) <=> (cont n)` when n is list.

In other words, `(Y (λf.λn.λk.(ISZERO k) (cont n) (f (CONS k n))) NIL)` can be optimized to `(Y (λf.λn.λk.(ISZERO k) (n cont) (f (CONS k n))) NIL)`).
