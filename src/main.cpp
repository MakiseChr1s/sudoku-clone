#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <Windows.h>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <fstream>

#include "Block.h"

#define main1

int lambda = []() { SetConsoleOutputCP(65001);return 0; }();

#ifdef main1

struct Empty
{
    int r, c;
    std::vector<int> candidates;
};

std::vector<std::vector<int>> testboard = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

std::vector<std::vector<int>> given(9, std::vector<int>(9, 0));

void init_given(bool isloaded, const std::vector<std::vector<int>>& board) {
    if (!isloaded)
    {
        for (int row = 0;row < 9;row++)
            for (int col = 0;col < 9;col++)
                given[row][col] = (board[row][col] != 0);
    }
}

void printBoard(const std::vector<std::vector<int>>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0)
                std::cout << ". ";
            else
                std::cout << board[r][c] << " ";
        }
        std::cout << "\n";
    }
}

bool canplace(const int& row,const int& col,const int& value,const std::vector<std::vector<int>>& board) 
{
    int r = row - 1, c = col - 1;
    for (int i = 0;i < 9;i++)
    {
        if (i != c && board[r][i] == value) return false;
        if (i != r && board[i][c] == value) return false;
    }
    int blockrow = r / 3 * 3, blockcol = c / 3 * 3;
    for (int i = blockrow;i < blockrow + 3;i++)
        for (int j = blockcol;j < blockcol + 3;j++)
        {
            if ((i != r || j != c) && board[i][j] == value) return false;
        }
    return true;
}

void printText(int& returnCode)
{
    switch (returnCode)
    {
    case 0: std::cout << "请输入三个整数" << std::endl;break;
    case 1: std::cout << "数字范围应在1~9之间" << std::endl;break;
    case 2: std::cout << "题面数字不能更改" << std::endl;break;
    case 3: std::cout << "不要与行/列/宫重复" << std::endl;break;
    case 4: std::cout << "成功清除" << std::endl;break;
    case -1: break;
    default: std::cout << "empty/unexpected return code" << std::endl;break;
    }
    returnCode = -1;
}

bool isComplete(const std::vector<std::vector<int>>& b)
{
    //非零检测
    for(const auto& row:b)
        for(const auto& value:row)
            if (value == 0||value<0||value>9) { return false; }

    //行检查
    for (int r = 0;r < 9;r++)
    {
        std::unordered_set<int> unit;
        for (int c = 0;c < 9;c++)
        {
            if (unit.find(b[r][c]) != unit.end()) return false;
            unit.insert(b[r][c]);
        }
    }

    //列检查
    for (int c = 0;c < 9;c++)
    {
        std::unordered_set<int> unit;
        for (int r = 0;r < 9;r++)
        {
            if (unit.find(b[r][c]) != unit.end()) return false;
            unit.insert(b[r][c]);
        }
    }

    //宫检查
        for (int br = 0;br < 3;br++)
        {
            for (int bc = 0;bc < 3;bc++)
            {
                std::unordered_set<int> unit;
                for (int r = br * 3;r < br * 3 + 3;r++)
                    for (int c = bc * 3;c < bc * 3 + 3;c++)
                    {
                        if (unit.find(b[r][c]) != unit.end()) return false;
                        unit.insert(b[r][c]);
                    }
            }
        }
        return true;
}

bool save(const std::string& path,const std::vector<std::vector<int>>& board)
{
    //打开文件，若文件不存在则创建新文件
    std::ofstream outfile(path);
    if (!outfile)
    {
        std::cerr << "创建文件失败" << std::endl;
        return false;
    }
    for(int r = 0;r<9;r++)
        for (int c = 0;c < 9;c++)
        {
            outfile << board[r][c] << ' ' <<std::endl;
        }
    for (int r = 0;r < 9;r++)
        for (int c = 0;c < 9;c++)
        {
            outfile << given[r][c] << ' ' << std::endl;
        }
    outfile.close();
    if(!outfile)
    {
        std::cerr << "打开或写入文件失败" << std::endl;
        return false;
    }
    std::cerr << "已存档" << std::endl;
    return true;
}

bool load(const std::string& path, std::vector<std::vector<int>>& board)
{
    
    std::ifstream infile(path);
    if (!infile)
    {
        std::cerr << "打开存档文件失败" << std::endl;
        return false;
    }
    std::vector<std::vector<int>> 
        temp_board(9, std::vector<int>(9)),
        temp_given(9, std::vector<int>(9));
    //temp_board.reserve(81);//error，reserve预约的是外层内存，需搭配emplace_back，temp_board[0][0]仍不存在
    //temp_given.reserve(81);
    for (int r = 0;r < 9;r++)
        for (int c = 0;c < 9;c++)
        {
            infile >> temp_board[r][c];
        }
    for (int r = 0;r < 9;r++)
        for (int c = 0;c < 9;c++)
        {
            infile >> temp_given[r][c];
        }
    if (!infile)
    {
        std::cerr << "读取存档失败" << std::endl;
        return false;
    }
    std::cerr << "读取存档成功" << std::endl;
    board = temp_board, given = temp_given;
    return true;
}

int solve(std::vector<std::vector<int>>& b,const std::vector<Empty>& emp ,int idx ,int lmt)
{
    
    if (emp.size() == idx) return 1;

    const Empty& e = emp[idx];
    int total = 0;

   
    for (int v : e.candidates)
    {
        if (!canplace(e.r + 1, e.c + 1, v, b)) continue;

        b[e.r][e.c] = v;
        total += solve(b, emp, idx + 1, lmt - total);
        b[e.r][e.c] = 0;

        if (total >= lmt) break;
    }
        return total;
}

bool MRV(std::vector<std::vector<int>> board)
{

    Block b;
    auto empty = b.emptyGridFinder(board);
    if (empty.size() > 1)
    {
        //获取每个空格的候选集
        int candidateSetCount = 0;
        std::vector<bool> isfilled(empty.size(), false);
        std::vector<Empty> emp;

        while (candidateSetCount < empty.size())
        {
            std::vector<int> cache;
            int r = empty[candidateSetCount].first,
                c = empty[candidateSetCount].second;
            std::vector<int> unit = { 1,2,3,4,5,6,7,8,9 };
            for (int v : unit)
            {
                if (canplace(r + 1, c + 1, v, board))
                {
                    cache.reserve(1);
                    cache.emplace_back(v);
                }
            }
                emp.push_back({ r,c,cache });
                candidateSetCount++;
            
        }

        //按候选集长度从小到大排序
        sort(emp.begin(), emp.end(), [](const Empty& e1, const Empty& e2) {return e1.candidates.size() < e2.candidates.size();});

        //2 9 5 6 3 1 8 4 7
        //6 8 7 2 9 4 3 1 5
        //3 1 4 8 5 . 6 9 2
        //5 2 . 1 . 6 9 7 4
        //4 7 1 9 2 3 5 8 6
        //9 6 8 4 . 5 2 3 1
        //1 3 2 7 6 8 4 5 9
        //8 4 9 5 . 2 7 6 3
        //7 5 6 3 4 9 1 2 8
        //递归求解
        int solves = solve(board, emp, 0, 2);
        if (solves == 1) return true;
        else if(solves == 0 || solves == 2) return false;
    }

    return true;
}

void randomErase(std::vector<std::vector<int>>& b,const int& level)
{
    std::vector<std::pair<int, int>> position;
    for (int r = 0;r < 9;r++)
    {
        for (int c = 0;c < 9;c++)
        {
            position.push_back(std::make_pair(r, c));
        }
    }
    static std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(position.begin(), position.end(), gen);

    int empty = 0;
    while (empty < level * 20)
    {
        int r = position[empty].first,
            c = position[empty].second;

        int value = b[r][c];
        b[r][c] = 0;
        if (!MRV(b)) b[r][c] = value;
        
        empty++;

    }
}

void play()
{
    Block block;
    int returnCode = -1;
    int level;
    char istoload;
    bool loaded;
    while(true)
    {
        system("cls");
        std::cout << "是否读档（Y/N）" << std::endl;
        

        if(!(std::cin >> istoload))        
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        else if (istoload == 'y' || istoload == 'Y')
        {
            loaded = load("save.txt",block.matrix);
            break;
        }
        else if (istoload == 'n' || istoload == 'N')
        {
            loaded = false;
            break;
        }
    }
    Sleep(500);
    
    if (!loaded)
    {
        block.generator();
        while (true)
        {
            system("cls");
            std::cout << "请输入难度（1~3）" << std::endl;
            if (!(std::cin >> level))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
            else if (level > 0 && level < 4)
            {
                randomErase(block.matrix, level);
                break;
            }
        }
    
    }
    init_given(loaded,block.matrix);
    while (true)
    {
        int row = 0, col = 0, value;

        system("cls");
        printBoard(block.matrix);
        std::cout << "请输入行，列，值，输入0 0 0退出。";
        printText(returnCode);

        if (!(std::cin >> row >> col >> value))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            returnCode = 0;
            
            continue;
        }
        if (row == 0 && col == 0 && value == 0)
        {
            std::cout << "游戏退出" << std::endl;
            save("save.txt",block.matrix);
            break;
        }

        if ((row < 1 || row > 9 || col < 1 || col > 9 ||
            value < 0 || value > 9)) {
            returnCode = 1;
            continue;
        }

        int r = row - 1, c = col - 1;

        if (given[r][c])
        {
            returnCode = 2;
            continue;
        }


        if (value != 0)
        {

            if ((canplace(row, col, value,block.matrix)))
            {
                block.matrix[r][c] = value;
                if (isComplete(block.matrix))
                {
                    system("cls");
                    printBoard(block.matrix);
                    std::cout << "完成！" << std::endl;
                    break;
                }
            }
            else
            {
                returnCode = 3;
                continue;

            }

        }

        if (value == 0)
        {
            block.matrix[r][c] = value;
                returnCode = 4;
                continue;
        }

    }
}

int main()
{
    play();
}

#endif



