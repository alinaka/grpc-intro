from concurrent import futures
import time
import logging

import grpc

import test_pb2
import test_pb2_grpc

_ONE_DAY_IN_SECONDS = 60 * 60 * 24


class Test(test_pb2_grpc.TestServicer):

    def SendMessage(self, request, context):
        return test_pb2.TestReply(message=f'Hello, {request.name}, {request.age}')    
    
    def GetLetters(self, request, context):
        for letter in request.body:
            l = test_pb2.Letter()
            l.character = letter
            yield l


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    test_pb2_grpc.add_TestServicer_to_server(Test(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)


if __name__ == '__main__':
    logging.basicConfig()
    serve()

