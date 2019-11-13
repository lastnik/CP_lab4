#pragma once
#include <string>
#include <functional>
#include "InterNet.h"
#include "EllipticCurve.h"

struct UserInit
{
    std::string          name;
    InterNet&            net;
};

struct IMsg;

class User
{
public:
    explicit User(UserInit&&);
    IMsg* receive();
    template<class T>
    void transmit(T);
    void connect(InterNet*);
    std::string getName();
private:
    UserInit cfg;
    InterNet* link;
};

template<class T>
void User::transmit(T msg)
{
    link->write<T>(msg);
}