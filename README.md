# AirBridge server

This project consist of two repos <br>
Another repo of client written in Java is available at
[AirBridge client](https://github.com/rozek1997/air-bridge-client)

## Project aim 

Project aim was to make client - server model using two programming languages. <br>

## Short description
For this purpose two application, client and server, in Java and C respectively have been written and connected via websocket. <br>
Server listing on the port define in application.properties file. Main thread is constantly listening for a new connection request. 
If request appear then server accept new connection, create new thread for this connection and pass socket descriptor to the newly created thread. 
This thread, receive file and save it under uniq name. Then server close connection and kill thread<br>

Limit of simultaneous working thread is 60!

## Compiling 
### Before using downolad library json-c or use command
` $ sudo apt install libjson-c-dev`

### Property file must contain field
` port="chosen port" `

## Compilation

`$ gcc -o "name" -std=gnu99 -Wall -lpthread -ljson-c`