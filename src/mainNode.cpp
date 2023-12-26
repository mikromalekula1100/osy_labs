#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>
#include <vector>
#include <map>

using std::endl;
using std::cout;
using std::cin;


//один поток будет считывать с консоли и отправлять на обработку, а второй - принимать результат и выводить его в консоль

void reading(){
    zmq::context_t ctx;
    zmq::socket_t reqPull(ctx, ZMQ_PULL);
    const std::string addrPull = "tcp://127.0.0.1:4041";
    reqPull.bind(addrPull);

    zmq::message_t answer;

    while(true){
       
        auto result = reqPull.recv(answer, zmq::recv_flags::dontwait);
    
        if(result.has_value() && result.value() > 0){
            cout<<"полученный ответ от сервера: "<<*(answer.data<int>())<<endl;        
        } 
        

    }
}

int main (){
    std::map<int, pid_t> nodes;
    std::vector
    zmq::context_t ctx;
    zmq::socket_t reqPush(ctx, ZMQ_PUSH);
    const std::string addrPush = "tcp://127.0.0.1:4040";
    reqPush.bind(addrPush); 

    std::thread newThread(reading);

    
    while(true){
        std::string input;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::vector<std::string> words;
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        
        
        if(words[0] == "create"){
            int idNode = std::stoi(words[1]);
            int idParent = std::stoi(words[2]);
            if(!nodes.find(idNode)){

            }
            cout<<"Error: Already exists"<<endl;
        }
        // else if(){

        // }
        // else{

        // }
        zmq::message_t msg(&value, sizeof(value));
        reqPush.send(msg, zmq::send_flags::none);
        cout<<"я клиент и я отправил число "<<value<<endl;  
 
            
    }
}
