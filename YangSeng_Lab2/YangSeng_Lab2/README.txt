Seng Xai Yang

How to compile client and server:
(Option 1)
g++ client.cc -o client
g++ server.cc -o server

(Option 2) Recommended
'make'

____________________________________
How to run program:
First run server then after that you can run the client

1. How to run server:
1) './server'
2) then it will prompt you to enter ip address: <enter ip address>
3) then it will prompt you to enter Port #: <enter port #>
4) then it will prompt you to enter Save file to: <enter filename or directory and filename '/tmp/a.out'>
5) then it will prompt you to enter in encryption key: <enter encryption key>

2. How to run client:
1) './client' 
2) prompt you to enter ip address: <enter ip address>
3) prompt you to enter Port #: <enter port #>
4) prompt you to enter File to be sent: <enter filename or directory and filename ex: '/tmp/1G/'>
5) prompt you to enter packet size: <enter packet size up to 64>
6) prompt you to enter in encryption key: <enter encryption key>

__________________________________________________

Problems:
- I havent implemented where a file name is not specified in the 'server' it automatically goes stdout.
- When reading in the buffer from the server that is sent over the server it reads in 
  extra bytes so when decrypting the file is not decrypted but it should of been able to decrypt it.
- Extra bytes are being written.
- Different md5sum due to reading extra bytes
- Unable to implement conversion of encrypted file into hexadecimal to be displayed
- When sending 1G file I get errors.

What works:
- Im able to connect the client to the server.
- Able to send text & binary file from client to server.
- Able to encrypt and decrypt text & binary file. (just that extra bytes are being written).
- Able to send big files from /tmp to another /tmp without crashing


