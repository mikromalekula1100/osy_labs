#include <iostream>
#include <string>
#include "zmq.hpp"
#include <unistd.h>
#include <thread>
#include <zmq.h>
#include <set>


#include "../include/makeTCP.hpp"
#include "../include/create_processe.hpp"
#include "../include/split_string.hpp"

using std::endl;
using std::cout;
/*
 * Главный поток принимает сообщение от предыдущего узла-родителя через сокет SUB, в этом же потоке смотрит, является ли он исполнителем запроса:
 * - если он исполнитель, то он направляет выполнение на поток для работы(возможно с испльзованием сокета PAIR)
 * - если он не исполнитель, то передает сообщение дальше через сокет PUB
 *
 * Также у меня есть третий процесс, который будет отвечать за прием сообщений от узлов-детей, он будет PULL, его задача слушать детей и отправлять результаты родителю, по идее этот поток можно убрать и делать те же действия в основном потоке, так как основной поток не выполняет долгую работу, нужно будет подумать над этим
*/

// void calculate(zmq::context_t& ctx){
//     zmq::socket_t socket(ctx, ZMQ_PAIR);
//     socket.connect("inproc://calculate");

//     zmq::message_t request;
//     while (true) {
        
//         auto result = socket.recv(request, zmq::recv_flags::dontwait);
//         if(result.has_value() && result.value() > 0){
            
//             int value = *(request.data<int>()); 
//             value *= 2;
//             cout<<"я вычислительный поток сервера и я принял число"<<value<<endl;
//             sleep(2);
//             zmq::message_t reply(&value, sizeof(value));
//             socket.send(reply, zmq::send_flags::none);
//             cout<<"я вычислительный поток и я отправил число"<< value<<endl;
//         }

        
//     }
// }

int main(int argc, char* argv[]){
    ште
    const int portIn = std::stoi(argv[0]);
    const int portOut = std::stoi(argv[1]);
    const int idThisNode = std::stoi(argv[2]);
    cout<<idThisNode<<endl;

    zmq::context_t ctx;

    //сокет для приёма сообщений от родителя
    zmq::socket_t respondSub(ctx, ZMQ_SUB);
    const std::string addrSub = makeTCP(portIn);
    respondSub.connect(addrSub);
    respondSub.set(zmq::sockopt::subscribe, "");

    //сокет для отправки сообщений детям
    zmq::socket_t respondPub(ctx, ZMQ_PUB);  
    const std::string addrPub = makeTCP(portOut + 1);
    respondPub.bind(addrPub);

    //сокет для принятия сообщений от детей
    zmq::socket_t respondPull(ctx, ZMQ_PULL);
    const std::string addrPull = makeTCP(portOut + 2);
    respondPull.bind(addrPull);

    //сокет для отправки результата родиxтелю
    zmq::socket_t respondPush(ctx, ZMQ_PUSH); 
    const std::string addrPush = makeTCP(portOut);    
    respondPush.connect(addrPush);

    //сокет для передачи команды о выполнении потоку исполнения
    // zmq::socket_t socket(ctx, ZMQ_PAIR);
    // socket.bind("inproc://calculate");
    // std::thread newThread(calculate, std::ref(ctx));

    zmq::message_t command;
    zmq::message_t answerFromChild;
    while(true){
      
        auto result = respondSub.recv(command, zmq::recv_flags::dontwait);

        if(result.has_value() && result.value() > 0){
            
            std::string str = command.to_string();

            std::vector<std::string> words = split_string(str);

            int idNode = std::stoi(words[1]);
            int idParent = std::stoi(words[2]);

            if(words[0] == "create"){

                if(idParent == idThisNode){
                    
                    pid_t pidId = 0;

                    pidId = create_processe();

                    if(!pidId){
                        
                        execl("../build/jobNode", &(std::to_string(portOut + 1)[0]), &(std::to_string(portOut + 2)[0]), &(std::to_string(idNode)[0]), NULL);
                        perror("Execl in child");
                        return -1;
                    }

                    

                    std::string str = "Ok: " + std::to_string(pidId);

                    zmq::message_t answer(&str[0], str.size());

                    respondPush.send(std::move(answer), zmq::send_flags::none);

                }

                else{
                    
                    zmq::message_t command(&str[0], str.size());
                    respondPub.send(std::move(command), zmq::send_flags::none);
                }

            }
 
        }


        auto result2 = respondPull.recv(answerFromChild, zmq::recv_flags::dontwait);

        if(result2.has_value() && result2.value() > 0){

            respondPush.send(answerFromChild, zmq::send_flags::none);
        }

           
            
        
    }

}
