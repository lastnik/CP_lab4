#pragma once
#include <string>
struct IMsg
{
    explicit IMsg(std::string&& _name) : name(_name){};
    std::string name;
};

struct InitMsg : public IMsg
{
    InitMsg() : IMsg("InitMsg"){};
    std::string openKeyX;
    std::string openKeyY;
};


