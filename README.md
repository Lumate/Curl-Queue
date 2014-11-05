Curl-Queue
==========

URL capturing over zmq for scalable external request handling

Requirements
------------
You will need zmq and protobuf and curl libs installed
```
sudo apt-get install libcurl4-gnutls-dev
```

Building
--------
```
git clone --recursive git@github.com:Lumate/Curl-Queue.git
cd Curl-Queue
mkdir build
cd build
cmake ..
make
```
