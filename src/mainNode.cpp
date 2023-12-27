#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>
#include <vector>
#include <set>

#include "../include/makeTCP.hpp"
#include "../include/create_processe.hpp"
#include "../include/split_string.hpp"


using std::endl;
using std::cout;
using std::cin;

int PORT = 6040;
//один поток будет считывать с консоли и отправлять на обработку, а второй - принимать результат и выводить его в консоль

//в управляющем узле будет один сокет PUB чтобы отсылать всем узлам запросы, а также сокет типа PULL в отдельном процессе для приёма срезультатов от выполняющих узлов

void reading(){

    zmq::context_t ctx;

    zmq::socket_t reqPull(ctx, ZMQ_PULL);

    const std::string addrPull = makeTCP(PORT+1);

    reqPull.bind(addrPull);

    zmq::message_t answer;

    while(true){
       
        auto result = reqPull.recv(answer, zmq::recv_flags::dontwait);
    
        if(result.has_value() && result.value() > 0){

            cout<< answer.to_string()<<endl;        
        } 
        

    }
}

int main (){

    std::set<int> nodes;

    zmq::context_t ctx;
    zmq::socket_t reqPub(ctx, ZMQ_PUB);
    
    const std::string addrPub = makeTCP(PORT);
    reqPub.bind(addrPub); 

    std::thread newThread(reading);

    
    while(true){
        
        std::string str;
        std::getline(std::cin, str);
        std::vector<std::string> words = split_string(str);
        
        int idNode = std::stoi(words[1]);
        int idParent = std::stoi(words[2]);

        
        if(words[0] == "create"){
            
            if(!nodes.count(idNode)){

                if(idParent == -1){

                    pid_t pidId = 0;

                    pidId = create_processe();

                    if(!pidId){
                        
                        execl("../build/jobNode", &(std::to_string(PORT)[0]), &(std::to_string(PORT+1)[0]), &(std::to_string(idNode)[0]), NULL);
                        perror("Execl in child");
                        return -1;
                    }

                    
                    
                    cout<<"Ok: "<<pidId<<endl;

                }

                else{
                    
                    zmq::message_t command(&str[0], str.size());
                    reqPub.send(std::move(command), zmq::send_flags::none);
                }

                nodes.insert(idNode);
                PORT += 4;
                

            }

            else{

                cout<<"Error: Already exists"<<endl;
            }
            
        }

        else if(words[0] == "exec"){

        }
            
    }
}
