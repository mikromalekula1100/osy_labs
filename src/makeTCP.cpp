#include "../include/makeTCP.hpp"

#include <chrono>

std::string makeTCP(int port)
{
    return "tcp://127.0.0.1:" + std::to_string(port);
}