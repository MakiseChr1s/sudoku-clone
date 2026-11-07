#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <Windows.h>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <fstream>

#define main1

int lambda = []() { SetConsoleOutputCP(65001);return 0; }();

#ifdef main1

std::vector<std::vector<int>> board = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

std::vector<std::vector<int>> testboard = {
    { 5, 3, 4, 6, 7, 8 ,9, 1, 2},
    {6, 7, 2, 1 ,9 ,5 ,3 ,4 ,8 },
    { 1, 9, 8 ,3 ,4 ,2, 5, 6,7 },
    {8, 5, 9 ,7 ,6 ,1 ,4 ,2 ,3},
    {4, 2, 6 ,8 ,5 ,3 ,7 ,9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1 ,9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

std::vector<std::vector<int>> given(9, std::vector<int>(9, 0));

void init_given(bool isloaded) {
    if (!isloaded)
    {
        for (int row = 0;row < 9;row++)
            for (int col = 0;col < 9;col++)
                given[row][col] = (board[row][col] != 0);
    }
}

void printBoard() {
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

bool canplace(const int& row,const int& col,const int& value) 
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

bool isWin(const std::vector<std::vector<int>>& b)
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

bool save(const std::string& path)
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

bool load(const std::string& path)
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

void play()
{
    int returnCode = -1;
    //if load == true,init_given(!load())
    init_given(load("save.txt"));
    Sleep(1000);
    while (true)
    {
        int row = 0, col = 0, value;

        system("cls");
        printBoard();
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
            save("save.txt");
            break;
        }

        if (row < 1 || row > 9 || col < 1 || col > 9 ||
            value < 0 || value > 9) {
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

            if ((canplace(row, col, value)))
            {
                board[r][c] = value;
                if (isWin(board))
                {
                    system("cls");
                    printBoard();
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
            board[r][c] = value;
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



