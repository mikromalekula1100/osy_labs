#include <iostream>
#include <string>
#include "zmq.hpp"
#include <unistd.h>
#include <thread>
#include <zmq.h>

using std::endl;
using std::cout;
/*
 * Главный поток принимает сообщение от предыдущего узла-родителя через сокет SUB, в этом же потоке смотрит, является ли он исполнителем запроса:
 * - если он исполнитель, то он направляет выполнение на поток для работы(возможно с испльзованием сокета PAIR)
 * - если он не исполнитель, то передает сообщение дальше через сокет PUB
 *
 * Также у меня есть третий процесс, который будет отвечать за прием сообщений от узлов-детей, он будет PULL, его задача слушать детей и отправлять, по идее этот поток можно убрать и делать те же действия в основном потоке, так как основной поток не выполняет долгую работу, нужно будет подумать над этим
*/
void calculate(zmq::context_t& ctx){
    zmq::socket_t socket(ctx, ZMQ_PAIR);
    socket.connect("inproc://calculate");

    zmq::message_t request;
    while (true) {
        
        auto result = socket.recv(request, zmq::recv_flags::dontwait);
        if(result.has_value() && result.value() > 0){
            
            int value = *(request.data<int>()); 
            value *= 2;
            cout<<"я вычислительный поток сервера и я принял число"<<value<<endl;
            sleep(2);
            zmq::message_t reply(&value, sizeof(value));
            socket.send(reply, zmq::send_flags::none);
            cout<<"я вычислительный поток и я отправил число"<< value<<endl;
        }

        
    }
}

int main(){
    zmq::context_t ctx;
    zmq::socket_t respondPush(ctx, ZMQ_PUSH);
    zmq::socket_t respondPull(ctx, ZMQ_PULL);
    const std::string addrPush = "tcp://127.0.0.1:4041";
    const std::string addrPull = "tcp://127.0.0.1:4040";
    respondPush.connect(addrPush);
    respondPull.connect(addrPull);
    
    zmq::socket_t socket(ctx, ZMQ_PAIR);
    socket.bind("inproc://calculate");
    std::thread newThread(calculate, std::ref(ctx));

    zmq::message_t req;
    zmq::message_t answer;  
    while(true){
      
        auto result = respondPull.recv(req, zmq::recv_flags::dontwait);
        if(result.has_value() && result.value() > 0){
            int receivedValue = *req.data<int>();
            cout << "получено число " << receivedValue << endl;
            zmq::message_t to_send(&receivedValue, sizeof(receivedValue));
            socket.send(to_send, zmq::send_flags::none); 
        }

        
                      
        auto result2 = socket.recv(answer, zmq::recv_flags::dontwait);

        if(result2.has_value() && result2.value() > 0){

            respondPush.send(std::move(answer), zmq::send_flags::none);
        }

           
            
        
    }

}
