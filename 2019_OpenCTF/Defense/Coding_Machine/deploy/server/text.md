최신 가이드

ssh ubuntu@codingmachine.sstf.site 접속후

sudo docker run -p 9000:9000 openctf 실행시켜주세요.


===================================================================================================================

유저가 업로드한 바이너리의 경로를 입력받습니다. 업로드시 바이너리에 인코딩이 되었다면 디코딩을 해주셔야 합니다.

loader로 유저의 바이너리를 실행시킵니다. seccomp가 적용되었습니다.

## ./loader ./example_user_binary ./test_input1.txt ./user_output0.txt

./example_user_binary : 유저의 바이너리 경로

./test_input1.txt : 채점에 사용될 파일. 주어진 파일을 그대로 사용하시면 됩니다.

./user_output0.txt : 결과물을 저장할 경로. 덮어쓰거나 새로 생성됩니다.

## ./check.sh ./test_input1.txt ./user_output0.txt

./test_input1.txt : 채점에 사용될 파일. 주어진 파일을 그대로 사용하시면 됩니다.

./user_output0.txt : 위 파일과 함께 채점에 사용됩니다.

[output] : true 또는 false를 stdout에 출력합니다. true일시에 flag를 유저에게 돌려주시면 됩니다.


