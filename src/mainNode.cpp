#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>
#include <vector>
#include <set>

#include "../include/makeTCP.hpp"
#include "../include/create_processe.hpp"
#include "../include/split_string.hpp"

//один поток будет считывать с консоли и отправлять на обработку, а второй - принимать результат и выводить его в консоль

//в управляющем узле будет один сокет PUB чтобы отсылать всем узлам запросы, а также сокет типа PULL в отдельном процессе для приёма срезультатов от выполняющих узлов

using std::endl;
using std::cout;
using std::cin;

const int PUB = 4740;
const int PULL = 4741;
int GPORT = 4742;


std::set<int> nodes;

int nodeForCheck = 0;

auto start = std::chrono::high_resolution_clock::now();

auto stop = start;

int64_t myTime = 1;

bool flagToHeartbit = false;

void reading(){

    std::set<int> survivingNodes;
    
    zmq::context_t ctx;

    zmq::socket_t reqPull(ctx, ZMQ_PULL);

    const std::string addrPull = makeTCP(PULL);

    reqPull.bind(addrPull);

    zmq::message_t answer;

    while(true){


        auto result = reqPull.recv(answer, zmq::recv_flags::dontwait);
    
        if(result.has_value() && result.value() > 0){
            
            std::string str = answer.to_string();   

            std::vector<std::string> words = split_string(str);

            if(flagToHeartbit){ 

                stop = std::chrono::high_resolution_clock::now();

                if(myTime*4 <= (std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)).count() && words[0] == "heartbit"){
                    
                    bool f = 1;

                    for(int i : nodes){

                            if(survivingNodes.count(i) == 0){

                                cout<<"Heartbit: "<<i<<" is unavailable now"<<endl;
                                f = 0;
                            }
                        }

                        if(f){
                            cout<<"OK"<<endl;
                        }

                        survivingNodes.clear();

                        start = std::chrono::high_resolution_clock::now();
                }

                else if(words[0] == "heartbit"){

                    int node = std::stoi(words[1]);

                    survivingNodes.insert(node);
                    
                }
                
                else{

                    cout<< answer.to_string()<<endl;  
                }

            }

            else if(words[0] == "check"){
        
                nodeForCheck = std::stoi(words[1]);
            }


            else{
                
                cout<<answer.to_string()<<endl;  
            }
                  
        } 
        

    }
}





int main (){


    zmq::context_t ctx;
    zmq::socket_t reqPub(ctx, ZMQ_PUB);
    
    const std::string addrPub = makeTCP(PUB);
    reqPub.bind(addrPub); 

    std::thread newThread(reading);

    
    while(true){
        
        std::string str;

        std::getline(std::cin, str);

        std::vector<std::string> words = split_string(str);
        
        int idNode = std::stoi(words[1]);

        std::string type_command = words[0];

        if(type_command == "create"){

            int idParent = std::stoi(words[2]);

            if(!nodes.count(idNode)){

                std::string check  = "check " + words[2];

                zmq::message_t msgCheck(&check[0], check.size());

                reqPub.send(std::move(msgCheck), zmq::send_flags::none);

                if(idParent == -1){

                    pid_t pidId = 0;

                    pidId = create_processe();

                    if(!pidId){
                        
                        execl("../build/jobNode", &(std::to_string(PUB)[0]), &(std::to_string(PULL)[0]),&(std::to_string(GPORT)[0]), &(std::to_string(idNode)[0]), NULL);
                        perror("Execl in child");
                        return -1;
                    }

                    cout<<"Ok: "<<pidId<<endl;

                }

                else if(!nodes.count(idParent)){
                    
                    cout<<"Error:"<<idParent<<" not found"<<endl;
                    continue;
                
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(250));

                if((idParent == -1) || words[2] == std::to_string(nodeForCheck)){

                    zmq::message_t command(&str[0], str.size());

                    reqPub.send(std::move(command), zmq::send_flags::none);

                    nodes.insert(idNode);
                    
                    GPORT += 2;
                }

                else{

                    cout<<"Error:"<<idParent<<" is unavailable"<<endl;
                }

                
            }

            else{

                cout<<"Error: Already exists"<<endl;
            }
            
        }

        else if(type_command == "exec"){
            
            std::string check  = "check " + words[1];

            zmq::message_t msgCheck(&check[0], check.size());
            
            reqPub.send(std::move(msgCheck), zmq::send_flags::none);

            std::this_thread::sleep_for(std::chrono::milliseconds(250));

            if(nodes.count(idNode)){

                if(words[1] == std::to_string(nodeForCheck)){

                    zmq::message_t command(&str[0], str.size());

                    reqPub.send(std::move(command), zmq::send_flags::none);
                }

                else{

                    cout<<"Error:"<<words[1]<< ": Node is unavailable"<<endl;
                }
                    
            }
            
            else{

                cout<<"Error:"<<words[1]<<": Not found"<<endl;
            }

        }

        else if(type_command == "heartbit"){
            
            if(idNode == -1){

                flagToHeartbit = false;
            }

            else{
            
                myTime = idNode;

                flagToHeartbit = true;
            }

            zmq::message_t command(&str[0], str.size());

            reqPub.send(std::move(command), zmq::send_flags::none);

            start = std::chrono::high_resolution_clock::now();
        }
            
    }
}
