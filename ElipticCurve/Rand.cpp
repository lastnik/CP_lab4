#include "Rand.h"
#include <cstdlib>
#include <ctime>
namespace Rand
{
Field::BigInt rand(uint64_t bitSize)
{
    using namespace Field;
    Field::BigInt res;
    std::srand(unsigned(std::time(0)));
    if(bitSize == 0)
    {
        return  Field::BigInt();
    }else
    {

        for(size_t i = 0; i < bitSize; i++)
        {
            res += (std::rand() % 2) ? "1"_BigIntMod : "0"_BigIntMod;
            res <<= 1;
        }
    }
    return res;
}
};