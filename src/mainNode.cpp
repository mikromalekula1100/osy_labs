#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>

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
    zmq::context_t ctx;
    zmq::socket_t reqPush(ctx, ZMQ_PUSH);
    const std::string addrPush = "tcp://127.0.0.1:4040";
    reqPush.bind(addrPush); 

    std::thread newThread(reading);

    
    while(true){
        int value;
        cin>>value;
        zmq::message_t msg(&value, sizeof(value));
        reqPush.send(msg, zmq::send_flags::none);
        cout<<"я клиент и я отправил число "<<value<<endl;  
 
            
    }
}