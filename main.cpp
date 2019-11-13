#include <iostream>
#include <cstdint>
#include "Logger.h"
#include "BigInt.h"
#include <chrono>
#include <thread>
#include "Field.h"
#include "IntegerMod.h"
#include "EllipticCurve.h"
#include "User.h"
#include "Msg.h"
#include "Rand.h"
constexpr char const* file = "config.json";
struct config
{
    std::string prime;
    std::string A;
    std::string B;
    std::string X;
    std::string Y;
    std::string n;
    std::string h;
    std::string LogLevel;
};
std::string find(std::string const& str, std::string const& key)
{
    auto k = str.find(key);
    if(k == std::string::npos)
    {
        throw error::ExeptionBase<error::ErrorList::InputError>("Lose mandatory field " + key);
    }
    k = str.find(':', k + 1);
    if(k == std::string::npos)
    {
        throw error::ExeptionBase<error::ErrorList::InputError>("Lose mandatory value of field " + key);
    }
    auto start = str.find("\"",k + 1);
    auto end   = str.find( "\"", start + 1);
    if(end == str.size())
        end  = str.find( "\n", start + 1);

    return str.substr(start + 1, end - start - 1);
};
config parse(std::string const& str)
{
    config cfg;
    cfg.prime = find(str,"prime");
    cfg.A = find(str,"A");
    cfg.B = find(str,"B");
    cfg.X = find(str,"X");
    cfg.Y = find(str,"Y");
    cfg.n = find(str,"n");
    cfg.h = find(str,"h");

    cfg.LogLevel = find(str,"LogLevel");
    return cfg;
}

void ECDH(User user, EllipticCurve curve, Point G)
{
    std::cout << "user " + user.getName()<< " started" << std::endl;
    InitMsg msg;
    auto privateKey = Rand::rand(Field::IntegerMod::getIntegerMod().getBitSize());
    std::cout << "user " + user.getName() << " generated private key: "<< privateKey.toString() << std::endl;
    auto [x, y] = curve.mul(G, privateKey);
    msg.openKeyX = x.toString();
    msg.openKeyY = y.toString();
    user.transmit<InitMsg>(msg);
    std::cout << "user " + user.getName() << " transmit public key: (" << x.toString() <<","<<y.toString() << ")" << std::endl;
    InitMsg* init = nullptr;
    init = static_cast<InitMsg *>(user.receive());
    Field::BigInt rX, rY;
    rX.setByString(init->openKeyX);
    rY.setByString(init->openKeyY);
    delete init;
    std::cout << "user " + user.getName()<< " received another public key: ("<< rX.toString() <<","<<rY.toString() << ")" << std::endl;
    auto [X, Y] = curve.mul({rX, rY}, privateKey);

    std::cout << user.getName() << ": Connect establishment common key: ("<< X.toString() <<","<< Y.toString() << ")" << std::endl;

    std::cout << "user " + user.getName() << " finished";
}


InterNet A2B, B2A;

int main() {
    BigInteger::BigInt a, b, prime;
    Field::BigInt A, B;
    Field::BigInt X, Y, N;
    Logger::setLevel(Log::Level::debug);
    bool crash = false;

    try
    {
        Logger::start();
    }
    catch(error::Exeption& exp)
    {
        std::cout << exp.what() << std::endl;
        return -1;
    }

    std::fstream in(file, std::ios_base::in);

    if(!in.is_open())
    {
        Logger::print<Log::Level::fatal>((std::string("Can't open file: ") + file).c_str());
        crash = true;
    }

    std::stringstream buf;
    buf << in.rdbuf();
    std::string json = buf.str();
    EllipticCurve curve;
    try
    {
        auto cfg = parse(json);
        Logger::setLevel(cfg.LogLevel);
        a.setByString(cfg.A);
        b.setByString(cfg.B);
        prime.setByString(cfg.prime);
        Field::IntegerMod::setIntegerMod(prime);
        A = Field::BigInt(a);
        B = Field::BigInt(b);
        curve.set(A, B, N);
        X.setByString(cfg.X);
        Y.setByString(cfg.Y);
        if(Y != curve.getY(X))
        {
            Logger::print<Log::Level::info>("(%s,%s) - point of curve Y=%s", X.toString().c_str(),
                                            curve.getY(X).toString().c_str(), Y.toString().c_str());
            crash = true;
        }
    }
    catch(error::Exeption& exp)
    {
        Logger::print<Log::Level::fatal>(exp.what().c_str());
        std::cout << exp.what() << std::endl;
        crash = true;
    }

    Logger::setLevel(Log::Level::debug);

    if(crash)
    {
        Logger::stop();
        return -1;
    }

    User userA({"Alice", B2A}), userB({"BOB", A2B});
    userA.connect(&A2B); userB.connect(&B2A);
    //ECDH(userA, curve, Point{X, Y});
    std::thread t1(ECDH, userA, curve, Point({X, Y}));
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::thread t2(ECDH, userB, curve, Point({X, Y}));
    t1.join(); t2.join();
    Logger::print<Log::Level::debug>((std::string("Json config input: \n") + json).c_str());
    Logger::stop();
    return 0;
}