#pragma once
#include <mutex>
#include <SFML/Network.hpp>
#include <string.h>
#include <iostream>

#include "nlohmann\json.hpp"
using namespace std;

struct Message
{
public:
    Message();
    ~Message();
    nlohmann::json process(nlohmann::json request);
};
extern Message message;
