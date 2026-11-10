#include <algorithm>
#include "Block.h"

bool isComplete(const std::vector<std::vector<int>>& b);
bool canplace(const int& row, const int& col, const int& value, const std::vector<std::vector<int>>& board);

std::vector<int> Block::shuffle_unit()
{
    std::vector<int> unit = { 1,2,3,4,5,6,7,8,9 };
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(unit.begin(), unit.end(), gen);
    return unit;
}

std::vector<std::pair<int, int>> Block::emptyGridFinder(const std::vector<std::vector<int>>& board)
{
    std::vector<std::pair<int,int>> empty;
    for (int r = 0;r < 9;r++)
    {
        for (int c = 0;c < 9;c++)
            if (board[r][c] == 0) { empty.push_back(std::make_pair(r, c)); }
    }

    return empty;
}

void Block::generator() 
{
    //对角线宫填充
    // 2 6 5 | 0 0 0 | 0 0 0
    // 3 4 1 | 0 0 0 | 0 0 0
    // 8 9 7 | 0 0 0 | 0 0 0
    // ---------------------
    // 0 0 0 | 1 9 4 | 0 0 0
    // 0 0 0 | 8 3 6 | 0 0 0
    // 0 0 0 | 5 2 7 | 0 0 0
    // ---------------------
    // 0 0 0 | 0 0 0 | 3 4 5
    // 0 0 0 | 0 0 0 | 9 6 2
    // 0 0 0 | 0 0 0 | 7 8 1
    for (int br = 0;br < max_grid / 3;br++)
    {
        int k = 0;
        auto unit = shuffle_unit();
        for (int r = br * 3;r < br * 3 + 3;r++)
            for (int c = br * 3;c < br * 3 + 3;c++)
            {
                matrix[r][c] = unit[k];
                k++;
            }
    }
    
    auto empty = emptyGridFinder(matrix);
        
    std::vector<bool> isfilled(54, false);

    int full = 0;

    std::vector<std::vector<int>> savedCache(empty.size(), std::vector<int>());
    while (full<empty.size())
    {
        int r = empty[full].first,
            c = empty[full].second;
        std::vector<int> cache;
        
        //生成候选集
        if (savedCache[full].empty() && !isfilled[full])
        {
            auto unit = shuffle_unit();
            while (!unit.empty())
            {
                if (canplace(r + 1, c + 1, unit.back(), matrix))
                {
                    cache.reserve(1);
                    cache.emplace_back(unit.back());
                    unit.pop_back();
                }
                else unit.pop_back();
            }
        }
        else
        {
            cache = savedCache[full];
        }
        
        //填数
        if (!cache.empty())
        {
            matrix[r][c] = cache.back();
            cache.pop_back();
            savedCache[full] = cache;
            isfilled[full] = true;
            full++;
        }
        else
        {
            isfilled[full] = false;
            matrix[r][c] = 0;
            if (full == 0) break;
            full--;
        }
                
            
        
    }
}
