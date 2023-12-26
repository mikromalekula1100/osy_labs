#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>
#include <vector>
#include <map>

#include "../include/create_processe.h"

using std::endl;
using std::cout;
using std::cin;


//один поток будет считывать с консоли и отправлять на обработку, а второй - принимать результат и выводить его в консоль

//в управляющем узле будет один сокет PUB чтобы отсылать всем узлам запросы, а также сокет типа PULL в отдельном процессе для приёма срезультатов от выполняющих узлов
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

    zmq::context_t ctx;
    zmq::socket_t reqPub(ctx, ZMQ_PUB);
    const std::string addrPush = "tcp://127.0.0.1:4040";
    reqPub.bind(addrPush); 

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
            if(idParent == -1){
                if(!nodes.find(idNode)){
                    pid_t pidId = create_processe();
                    if(!pidId){
                        
                    }
                    nodes[idNode] = 
                }
            }
            // int idNode = std::stoi(words[1]);
            // int idParent = std::stoi(words[2]);
            // if(!nodes.find(idNode)){
            //     for(auto i : nodes){
            //         i.second
            //     }
            // }
            // cout<<"Error: Already exists"<<endl;
        }
        // else if(){

        // }
        // else{

        // }
        zmq::message_t msg(&value, sizeof(value));
        reqPub.send(msg, zmq::send_flags::none);
        cout<<"я клиент и я отправил число "<<value<<endl;  
 
            
    }
}
