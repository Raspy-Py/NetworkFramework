
## Installation
Install the project via:
```bash
git clone --recurse-submodules https://github.com/Raspy-Py/NetworkFramework.git
```
Build by running following command in the project root:
```bash
mkdir build 
cd build 
cmake ..
cmake --build .
```
## Usage

To test, run the 'server' and 'client' either in different processes on the same device or on different devices in the same local network.
<br>
### Run client
 - Windows
```bash
.\client.exe
```
 - Linux/MacOS
 ```bash
 ./client
 ```
 ### Run server
  - Windows
 ```bash
 .\server.exe <port>
 ```
  -Linux/MacOS
```bash
./server <port>
```
<mark>port - arbitrary number between 1024 and 65536</mark>
