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

const int PUB = 5510;
const int PULL = 5511;
int GPORT = 5612;

std::set<int> nodes;


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

                cout<<1111111<<endl;

            }

            else{
                
                cout<<2222222222<< answer.to_string()<<2222222222<<endl;  
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
                    
                    cout<<"Error: Parent not found"<<endl;
                    continue;
                
                }

                zmq::message_t command(&str[0], str.size());

                reqPub.send(std::move(command), zmq::send_flags::none);

                nodes.insert(idNode);
                
                GPORT += 2;
            }

            else{

                cout<<"Error: Already exists"<<endl;
            }
            
        }

        else if(type_command == "exec"){
            
            //TO DO: Subcommand subcommand = Subcommand(idNode, words[2]);

            zmq::message_t command(&str[0], str.size());

            reqPub.send(std::move(command), zmq::send_flags::none);

        }

        else if(type_command == "hearbit"){

            if(idNode == -1){

                flagToHeartbit = false;
            }
            else{

                myTime = idNode;
                flagToHeartbit = true;
            }

            start = std::chrono::high_resolution_clock::now();
        }
            
    }
}
