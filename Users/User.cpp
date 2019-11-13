#include "User.h"
#include <chrono>
#include <thread>
#include <iostream>
User::User(UserInit&& init) : cfg(std::move(init))
{}

IMsg* User::receive()
{
    using namespace std::chrono_literals;
    while (cfg.net.empty())
    {
        std::this_thread::sleep_for(1s);
        std::cout << "user " + getName()<< " wait" << std::endl;
    }
    return cfg.net.read();
}

void User::connect(InterNet* linked)
{
    link = linked;
}

std::string User::getName() {
    return cfg.name;
}
