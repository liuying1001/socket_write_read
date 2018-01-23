# socket write read
socket client write/read pos on the server file.

# usage 
1.build
```
# make clean & make 
```
2. console1
```
#./server
```
3. console2
write data to server.
```
#./client
Please Input cmd: write 3 4 ssssssdddddd
```
write data "ssssssdddddd" to the server, server will write data to file from 3*LBA_SIZE.

read data from server
```
#./client
Please Input cmd: read 3 4 
```
This command will read data from server start with the POS 3*LBA_SIZE
