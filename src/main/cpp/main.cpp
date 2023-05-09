#include <iostream>
#include <fstream>
#include <numeric>

#include <json/json.h>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include "proto/locations.pb.h"

using namespace std;
using boost::asio::ip::tcp;


void processProtobuf(tcp::iostream& stream){
    // Read message size
    int messageSize;// your implementation
    if (!stream.read(reinterpret_cast<char*>(&messageSize), sizeof(messageSize))) {
        // Read error
        cout << "Reading message size error" << endl;
        return;
    }
    messageSize = ntohl(messageSize);

    // Read message
    std::vector<char> buffer(messageSize);
    if (!stream.read(buffer.data(), messageSize)) {
        // Read Error
        cout << "Reading data error" << endl;
        return;
    }


    // Parse data
    Request req;
    if (!req.ParseFromArray(buffer.data(), messageSize)) {
        // Parse error
        cout << "Parsing Data error" << endl;
        return;
    }
		cout << "Req size: "<<req.ByteSizeLong();
    /*esw::PResults results;
    for (auto ds=datasets.datasets().begin(); ds!=datasets.datasets().end(); ++ds){
        esw::PResult* result = results.add_results();

        // Get averages
        for (auto it=(*ds).records().begin(); it!=(*ds).records().end(); ++it){
            double average = 0;
            for (const double rec : (*it).second.record()){
                average += rec;
            }
            // Add average
            (*result->mutable_averages())[(*it).first] = average/(*it).second.record().size();
        }

        // Send the result back
        (*result->mutable_info()) = (*ds).info();
    }

    string serialized_results = results.SerializeAsString();
    stream.write(serialized_results.c_str(), serialized_results.length());
    //stream << results.SerializeAsString().c_str();
    cout << results.ShortDebugString() << endl;*/
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Error: two arguments required - ./server  <port>" << endl;
        return 1;
    }

    // unsigned short int port = 12345;
    unsigned short int port = atoi(argv[1]);

    try {
        boost::asio::io_service io_service;

        tcp::endpoint endpoint(tcp::v4(), port);
        tcp::acceptor acceptor(io_service, endpoint);

        while (true) {
            cout << "Waiting for message in ProtoBuf format..." << endl;
            tcp::iostream stream;
            boost::system::error_code ec;
            acceptor.accept(*stream.rdbuf(), ec);

            processProtobuf(stream);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
