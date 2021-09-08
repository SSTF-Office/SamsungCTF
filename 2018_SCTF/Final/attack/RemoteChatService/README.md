# RemoteChatService

## Description

```
I made a super secure chat service, connects to the chat server by using the remote client.
You need to exploit the chat server to get the flag.

(service address)
(binary download link)
```

## How to run
In deploy folder
```
# ./run.sh
```

## How to solve
1. Make and join the room.
2. In the client, when receiving chat data from the server, the local stack
buffer of the client can be overflowed.
3. In the server, when copying the chat data which was received from the client,
the local stack buffer of the server also can be overflowed.
4. The attacker needs to exploit the remote client first, to trigger the
vulnerability in the chat server.
5. Read the flag which located in /home/chat_server/flag by exploiting the chat
server.
