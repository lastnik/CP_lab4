#pragma once
#include <list>

struct IMsg;

class InterNet
{
public:
    InterNet() = default;
    ~InterNet() = default;
    bool empty();
    template <class T>
    void write(T);
    IMsg* read();
private:
    std::list<IMsg*> buf;
};

template <class T>
void InterNet::write(T msg)
{
    T* ptr = new T();
    ptr->name = msg.name;
    ptr->openKeyX = msg.openKeyX;
    ptr->openKeyY = msg.openKeyY;
    buf.push_back(ptr);
}