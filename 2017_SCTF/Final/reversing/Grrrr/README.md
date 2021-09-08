# Grrrr (Difficulty: Medium)

## Description

Grrrr.......

## Requirements

1. Python (>= 2.7)
2. Go (>= 1.7 ..?)
3. Grumpy (You can make executable binary by this repository https://github.com/google/grumpy/tree/dba35db9b866c3f0f18d2744218cf35a6023e72e)

## Compile

```
python grrrr_generator.py [flag] > grrrr.py
build/bin/grumpc grrrr.py > grrrr.go (in grumpy folder)
go build  -ldflags "-s -w" -o grrrr grrrr.go
```

## Comment

Reversing a Go binary compiled by Grumpy.

## Hint

1. Grumpy
2. https://github.com/google/grumpy/tree/dba35db9b866c3f0f18d2744218cf35a6023e72e
3. no stripped binary

## How to Solve.

1. 먼저 해당 go 파일이 Grumpy로 컴파일된 파일인지를 눈치 채어야함 (이건 strings로 grumpy 문자열이 보이기 때문에 유추가 쉬움, 심지어 실행시 Usage grrrr.py [flag]로 친절하게 알켜줌)
2. Grumpy가 어떠한 방식으로 동작하는지 파악 (Grumpy는 python module -> go module 로 wrapping하기 때문에 wrapping 된 함수가 무엇인지 찾을 필요 있음)
3. 여러 함수를 포함한 python 파일을 만든뒤 Grumpy로 go 바이너리 만들어서 ida에서 해당 함수가 어떻게 구성되어있는지를 파악하면 Grrrr바이너리도 어떻게 동작하는지 유추가 가능 (sig 파일 만들면 좀 더 쉬움)

## Useful Resources

* https://rednaga.io/2016/09/21/reversing_go_binaries_like_a_pro/

## Write Up

* https://github.com/Jinmo/ctfs/tree/master/2017/sctf2017finals/reversing/grrr (Korean)

## Flag
SCTF{6rumpy_c47_m30w5_6rrr}

