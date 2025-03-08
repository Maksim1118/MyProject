#pragma once
#include <mutex>
#include <SFML/Network.hpp>
#include <string.h>
#include <iostream>

#include "nlohmann\json.hpp"
using namespace std;

struct MessageServer
{
public:
    MessageServer();
    ~MessageServer();
    nlohmann::json request;
    nlohmann::json response;
    bool isResponse = true;
};
extern MessageServer messageServer;
extern mutex m;