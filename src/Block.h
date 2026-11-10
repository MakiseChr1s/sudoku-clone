#pragma once
#include <utility>
#include <iostream>
#include <vector>
#include <random>
class Block
{
public:
    void generator();
    std::vector<std::pair<int, int>> emptyGridFinder(const std::vector<std::vector<int>>& board);
    std::vector<int> shuffle_unit();

public:
    std::vector<std::vector<int>> matrix{ 9,std::vector<int>(9,0) };
    int max_grid = 9;
};