## compile option
g++ main.cpp user.cpp -o user_binary

## execute
user_binary < sample_input.txt

## base64 encoding
base64 -w 0 user_binary > encoded_user_binary

## upload your 'encoded_user_binary'
(cat encoded_user_binary; echo "") | nc codingmachine.sstf.site 9000
