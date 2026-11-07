#pragma once
#include <iostream>
#include <vector>
#include <random>
class Block
{
private:
    int max_grid = 9;
private:
    void board_init();
    void generator();
    std::vector<int> shuffle_unit();
public:
    std::vector<std::vector<int>> matrix;
    
};