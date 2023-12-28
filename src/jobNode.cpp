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

int idThisNode = 0;


/*
 * Главный поток принимает сообщение от предыдущего узла-родителя через сокет SUB, в этом же потоке смотрит, является ли он исполнителем запроса:
 * - если он исполнитель, то он направляет выполнение на поток для работы(возможно с испльзованием сокета PAIR)
 * - если он не исполнитель, то передает сообщение дальше через сокет PUB
 *
 * Также у меня есть третий процесс, который будет отвечать за прием сообщений от узлов-детей, он будет PULL, его задача слушать детей и отправлять результаты родителю, по идее этот поток можно убрать и делать те же действия в основном потоке, так как основной поток не выполняет долгую работу, нужно будет подумать над этим
*/

// void heartbit(zmq::context_t& ctx){

//     zmq::socket_t socketHeartbit(ctx, ZMQ_PAIR);

//     socketHeartbit.connect("inproc://heartbit");

//     zmq::message_t request;
    
//     bool flag = false;

//     int value = -1;

//     auto start = std::chrono::high_resolution_clock::now();

//     auto stop = start;

//     std::string answer = "heartbit " + std::to_string(idThisNode);

//     while(true){

//         auto result = socketHeartbit.recv(request, zmq::recv_flags::dontwait);

//         if(result.has_value() && result.value() > 0){

//             value = *request.data<int>();

//             if(value == -1){

//                 flag = false;

//                 continue;
//             }

//             flag = true;
//         }

//         if(flag){

//            stop = std::chrono::high_resolution_clock::now();

//            if((std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)).count() >= value){

//                 zmq::message_t msg(&answer[0], answer.size());

//                 socketHeartbit.send(std::move(msg), zmq::send_flags::none);

//                 start = std::chrono::high_resolution_clock::now();
//            }

            
//         }
//     }
// }

// void calculate(zmq::context_t& ctx){

//     zmq::socket_t socketCalculate(ctx, ZMQ_PAIR);

//     socketCalculate.connect("inproc://calculate");

//     zmq::message_t request;
    

//     auto start = std::chrono::high_resolution_clock::now();
//     auto stop = start;

//     while (true) {
        
//         auto result = socketCalculate.recv(request, zmq::recv_flags::dontwait);
        
//         if(result.has_value() && result.value() > 0){

//             std::string subcommand = request.to_string();

//             std::string answer;

//             if(subcommand == "start"){

//                 start = std::chrono::high_resolution_clock::now();
//                 answer = "Ok:" + std::to_string(idThisNode);
//             }
//             else if(subcommand == "stop"){
                
//                 stop = std::chrono::high_resolution_clock::now();
//                 answer = "Ok:"+  std::to_string(idThisNode);
//             }
//             else if(subcommand == "time"){

//                 answer = "Ok:" + std::to_string(idThisNode) + ": " + std::to_string((std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)).count());

                
//             }

//             zmq::message_t msg(&answer[0], answer.size());
//             socketCalculate.send(std::move(msg), zmq::send_flags::none);
//         }

        
//     }
// }

int main(int argc, char* argv[]){

    const int PUB = std::stoi(argv[0]);
    const int SUB = std::stoi(argv[1]);
    int GPORT = std::stoi(argv[2]);
    idThisNode = std::stoi(argv[3]);

    const int thisPUB = GPORT;
    const int thisPULL = GPORT+1;

    GPORT += 2;

    zmq::context_t ctx;

    //сокет для приёма сообщений от родителя
    zmq::socket_t respondSub(ctx, ZMQ_SUB);
    const std::string addrSub = makeTCP(PUB);
    respondSub.connect(addrSub);
    respondSub.set(zmq::sockopt::subscribe, "");

    //сокет для отправки результата родиxтелю
    zmq::socket_t respondPush(ctx, ZMQ_PUSH); 
    const std::string addrPush = makeTCP(SUB);    
    respondPush.connect(addrPush);

    //сокет для отправки сообщений детям
    zmq::socket_t respondPub(ctx, ZMQ_PUB);  
    const std::string addrPub = makeTCP(thisPUB);
    respondPub.bind(addrPub);

    //сокет для принятия сообщений от детей
    zmq::socket_t respondPull(ctx, ZMQ_PULL);
    const std::string addrPull = makeTCP(thisPULL);
    respondPull.bind(addrPull);

    // //сокет для передачи команды о выполнении потоку исполнения
    // zmq::socket_t socketCalculate(ctx, ZMQ_PAIR);
    // socketCalculate.bind("inproc://calculate");
    // std::thread threadCalculate(calculate, std::ref(ctx));
   

    // //сокет для передачи того, чтобы общаться с потоком, котоырй занимается heartbit
    // zmq::socket_t socketHeartbit(ctx, ZMQ_PAIR);
    // socketHeartbit.bind("inproc://heartbit");
    // std::thread threadHeartbit(heartbit, std::ref(ctx));

    zmq::message_t command;

    zmq::message_t answerFromChild;

    // zmq::message_t answerFromCalculate;

    // zmq::message_t answerFromHeartbit;

    while(true){

      
        auto result = respondSub.recv(command, zmq::recv_flags::dontwait);

        if(result.has_value() && result.value() > 0){
            
            std::string str = command.to_string();

            std::vector<std::string> words = split_string(str);

            int idNode = std::stoi(words[1]);

            std::string type_command = words[0];

            
            if(type_command == "create"){
            
                int idParent = std::stoi(words[2]);

                cout<<idParent<<" "<<idThisNode<<endl;
                
                if(idParent == idThisNode){
                    
                    pid_t pidId = 0;

                    pidId = create_processe();

                    if(!pidId){
                        
                        execl("../build/jobNode", &(std::to_string(thisPUB)[0]), &(std::to_string(thisPULL)[0]), &(std::to_string(GPORT)[0]), &(std::to_string(idNode)[0]), NULL);
                        perror("Execl in child");
                        return -1;
                    }

                    std::string str = "Ok1: " + std::to_string(pidId);

                    zmq::message_t answer(&str[0], str.size());

                    respondPush.send(std::move(answer), zmq::send_flags::none);

                    
                }
                
                GPORT += 2;
                zmq::message_t command(&str[0], str.size());
                respondPub.send(std::move(command), zmq::send_flags::none);

            }

            // else if(type_command == "exec"){

            //     if(idNode == idThisNode){

            //         zmq::message_t msg(&((words[2])[0]), words[2].size());

            //         socketCalculate.send(std::move(msg), zmq::send_flags::none);
            //     }

            //     else{

            //         zmq::message_t command(&str[0], str.size());
            //         respondPub.send(std::move(command), zmq::send_flags::none);
            //     }
                
            // }

            // else if(type_command == "heartbit"){

            //     zmq::message_t msg(&((words[1])[0]), words[1].size());

            //     socketHeartbit.send(std::move(msg), zmq::send_flags::none);
            // }
 
        }


        auto result2 = respondPull.recv(answerFromChild, zmq::recv_flags::dontwait);

        if(result2.has_value() && result2.value() > 0){

            respondPush.send(answerFromChild, zmq::send_flags::none);
        }


        // auto result3 = socketCalculate.recv(answerFromCalculate, zmq::recv_flags::dontwait);

        // if(result3.has_value() && result3.value() > 0){

        //     respondPush.send(answerFromCalculate, zmq::send_flags::none);
        // }
           
            
        // auto result4 = socketHeartbit.recv(answerFromHeartbit, zmq::recv_flags::dontwait);

        // if(result4.has_value() && result4.value() > 0){

        //     respondPush.send(answerFromHeartbit, zmq::send_flags::none);
        // }
        
    }

}
