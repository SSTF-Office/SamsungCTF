Main goal: Get admin privileges and read flag from /hive/secret server endpoint.
Web application is written in python Flask framework with enabled debugging WSGI.

Step by step:
1. Register new account
2. Log in
3. Look at headers in server response, there is cookie in strange format, unusually long and encoded in base64, play with it
4. Modify cookie or set it to empty string and resend request to the server on / path.
5. Server will respond with 500 Internal Server Error and reveal Flask application stacktrace. Analyze it.
6. Flask stacktrace reveals piece of code that causes server error through uncaught exception.
   In the check_cookie function, a comment will appear showing the format in which the cookie is created and information about the additional unsecured
   server endpoint /cookietest/ available only after successfully logging in. It decodes cookie and display its content. 
   Sending an empty cookie to the server will result in exception in another place that reveal information about the mechanism used to encrypt cookie (AES in CBC mode).
7. Application session management logic is vulnerable to bit flipping attack that can increase the privileges of ordinary user to administrator.
8. Analyze how cookie is crafted by the server and register new user with proper field alignment that will allow to exploit this vulnerability.
Example:
    AES block size = 16 bytes
    Cookie format = '{ "username": "", "email": "", "isAdmin" : 1/0, "sessionId" : "" }'
    Divide cookie into 16 bytes blocks. Use email field in registration form to align rest of fields. Json format must be preserved so that it can be decoded correctly
    Cookie with correct field alignment:
        { "username": "test", "email": "test@test123.com", "isAdmin": 0, "sessionId": <sessionId> }
    You can now divide this cookie into 16 bytes blocks

        1.    { "username": "t
        2.    est", "email": "
        3.    test@test123.com    <=== block that will be used to perform bit flipping attack
        4.    ", "isAdmin": 0,
        and so on...

By modification of the selected block we are able to influence the content of the next block.
If we want to flip a single bit in the plaintext we must flip the bit at the same position in the previous ciphertext block.
Our goal is to change isAdmin value from 0 to 1.
We can use /cookietest/ endpoint to check if we generate proper cookie.
After crafting valid admin cookie we can read the flag from /hive/secret/ endpoint.
