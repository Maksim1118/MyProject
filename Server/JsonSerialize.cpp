#include "JsonSerialize.h"

nlohmann::json Server::JsonSerialize::oneTimeDataInJson(Thorn& thorn) const
{
    return nlohmann::json();
}

nlohmann::json Server::JsonSerialize::staticDataInJson(Thorn& thorn) const
{
    return nlohmann::json();
}

nlohmann::json Server::JsonSerialize::persistentDataInJson(Thorn& thorn) const
{
    return nlohmann::json();
}

nlohmann::json Server::JsonSerialize::staticDataInJson(Bot& bot) const
{
    return nlohmann::json();
}
