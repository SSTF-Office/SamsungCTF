# arm_arm write-up

## Attack vector
saveNote 쪽에서 sprintf로 인한 stack-overflow가 발생한다.

User 와 link는 각각 길이체크를 검수 하지만 sprintf 로 두 변수가 한번에 buffer로 복사하면서 정해진 사이즈를 넘기게 된다.

## Attack payload
다양한 attack payload 가 나올 수 있다.

방법중 하나로 글로벌로 선언된 ID 값은 사용자 input으로 만들 수 있다.

공격자는 해당 ID에 `/flag` 라는 문자열을 만들어 줄 수 있다. 

이후 memcpy 코드를 통하여 `note.db`의 문자열을 `ID` 값인 flag 로 바꾸어 줄 수 있다.'

해당 id address를 src 인자로, note.db address를 dst 인자로 맞춘 뒤 saveNote의 memcpy 주소로 분기 한다.

memcpy가 호출되면 note.db의 경로가 바뀌게 되고 뒤 이은 printNote가 호출 되면서 바뀐 경로의 flag를 읽어 올 수 있다.



