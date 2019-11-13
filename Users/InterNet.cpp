#include "InterNet.h"
#include "Msg.h"
bool InterNet::empty()
{
    return buf.empty();
}

IMsg* InterNet::read()
{
    IMsg* msg = buf.front();
    buf.pop_front();
    return msg;
}

