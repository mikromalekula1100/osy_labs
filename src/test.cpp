void reading(){

    zmq::context_t ctx;

    zmq::socket_t reqPull(ctx, ZMQ_PULL);

    const std::string addrPull = makeTCP(PULL);

    reqPull.bind(addrPull);

    zmq::message_t answer;

    while(true){
       
        auto result = reqPull.recv(answer, zmq::recv_flags::dontwait);
    
        if(result.has_value() && result.value() > 0){

            cout<< answer.to_string()<<endl;        
        } 
        

    }
}