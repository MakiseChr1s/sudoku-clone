#include <algorithm>
#include "Block.h"


std::vector<int> Block::shuffle_unit()
{
    std::vector<int> unit = { 1,2,3,4,5,6,7,8,9 };
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(unit.begin(), unit.end(), gen);
    return unit;
}

void Block::board_init() {}

void Block::generator() {}