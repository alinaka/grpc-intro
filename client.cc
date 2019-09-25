#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "test.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using test::Letter;
using test::Test;
using test::TestRequest;
using test::TestReply;
using test::Word;

class TestClient {
 public:
  TestClient(std::shared_ptr<Channel> channel)
      : stub_(Test::NewStub(channel)) {}

  void GetLetters(const std::string& body) {
	  Word word;
	  Letter letter;
	  ClientContext context;
	  word.set_body(body);
	  std::unique_ptr<ClientReader<Letter> > reader(
			  stub_->GetLetters(&context, word));
	  while (reader->Read(&letter)) {
		  std::cout << "Got new letter "
					<< letter.character() << std::endl;
	  }
	  Status status = reader->Finish();
	  if (status.ok()) {
		std::cout << "GetLetters rpc succeeded." << std::endl;
	  } else {
		std::cout << "GetLetters rpc failed." << std::endl;
	  }
  }

  std::string SendMessage(const std::string& user, std::int32_t& age) {
    // Data we are sending to the server.
	TestRequest request;
    request.set_name(user);
    request.set_age(age);

    // Container for the data we expect from the server.
    TestReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SendMessage(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Test::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  TestClient greeter(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::int32_t age(88);
  std::string reply = greeter.SendMessage(user, age);
  std::cout << "Test received: " << reply << std::endl;

  std::string word("grpc");
  greeter.GetLetters(word); 

  return 0;
}

