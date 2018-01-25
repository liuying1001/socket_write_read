# socket write read
socket client write/read pos on the server file.

# usage 
## 1.build
```
# make clean & make 
```
## 2.run server
open console 1 to perform cmd below.
```
#./server
```
## 3.run client

### 3.1 write data
open console 2 to perform cmd, write data to server.
```
#./client
Please Input cmd: write 3 4 ssssssdddddd
```
write data "ssssssdddddd" to the server, server will write data to file from 3*LBA_SIZE.

### 3.2 read data
read data from server
```
#./client
Please Input cmd: read 3 4 
```
This command will read data from server start with the POS 3*LBA_SIZE.
