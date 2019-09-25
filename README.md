## Compile

`python3 grpc_tools.protoc -I. --python_out=. --grpc_python_out=. test.proto`

`make test.grpc.pb.cc test.pb.cc`

`make`

## Run server

`python3 server.py`

## Run client

`./client`

