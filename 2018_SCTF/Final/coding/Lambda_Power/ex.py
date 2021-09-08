# -*- coding: utf-8 -*-
def gen():
    FIRST = '(λp.p (λx.λy.x))'
    SECOND = '(λp.p (λx.λy.y))'
    PAIR = '(λx.λy.λz.z x y)'
    TRUE = '(λx.λy.x)'
    FALSE = '(λx.λy.y)'
    CONS = PAIR
    HEAD = FIRST
    TAIL = SECOND
    NIL = FALSE
    ISNIL = '(λl.l (λh.λt.λd.{FALSE}) {TRUE})'.format(**locals())
    ADD = '(λm.λn.λf.λx.m f (n f x))'
    Y = '(λf.(λx.f (x x)) (λx.f (x x)))'
    ONE = '(λf.λx.f x)'
    TWO = '(λf.λx.f f x)'
    SUCC = '(λn.λf.λx.f (n f x))'
    INLOOP = '({Y} {SUCC})'.format(**locals())
    PRED = '(λn.λf.λx.n (λg.λh.h (g f)) (λu.x) (λu.u))'
    MINUS = '(λm.λn.n {PRED} m)'.format(**locals())
    ISZERO = '(λn.n (λx.{FALSE}) {TRUE})'.format(**locals())
    LEQ = '(λm.λn.{ISZERO} ({MINUS} m n))'.format(**locals())
    IF = '(λp.λa.λb p a b)'
    for k, v in locals().items():
        globals()[k] = v


gen()


def get_nArg_and_Do(d):
    H = '(Y (λf.λn.λk.(ISZERO k) (n %s) (f (CONS k n))) NIL)' % d
    return H


def popMINinList():
    H = '(λh.λt.λcont.λret.(Y (λf.λn.λl.λr.(ISNIL l) (r (cont (CONS n ret))) (l (λh.λt.((LEQ h n) (f h t (CONS n r)) (f n t (CONS h r))))))) h t NIL)'
    return H


def sort():
    H = '(λh.λt.(Y (λf.λr.λh.λt.(ISNIL t) (CONS h r) (POP h t f r))) NIL h t)'
    return H


def pack(X):
    return  '((λISZERO.λCONS.λNIL.λY.λISNIL.λLEQ.λFIRST.λSECOND.((λPOP.((λSORT.%s) %s)) %s)) %s %s %s %s %s %s %s %s)' % \
            (X, sort(), popMINinList(), ISZERO, CONS, NIL, Y, ISNIL, LEQ, FIRST, SECOND)

print pack(get_nArg_and_Do('SORT'))
