#include "makeTCP.hpp"

std::string makeTCP(int port)
{
    return "tcp://127.0.0.1:" + std::to_string(port);
}