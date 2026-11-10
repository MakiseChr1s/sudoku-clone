# sudoku-clone

一个 7 天内写出来的 Windows 控制台数独仿版，用来练习 C++、递归和回溯。

原项目：https://github.com/mayerui/sudoku/

## 怎么跑

### 方式 A：Visual Studio

用 Visual Studio 打开：

```text
src/sudoku-clone.sln
```

选择 x64 Debug，生成并运行。

### 方式 B：g++

需要 Windows + MinGW g++，在仓库根目录执行：

```powershell
g++ -std=c++17 -Wall -Wextra src/main.cpp src/Block.cpp -o sudoku.exe
.\sudoku.exe
```

`Block.cpp` 不能漏，否则 `Block::generator()` 等函数会链接失败。

## 操作

启动时先选择：

```text
是否读档（Y/N）
```

选 `N` 会生成新题；选 `Y` 读取 `save.txt`，没有存档时回退到新题。

然后输入三个整数：

```text
行 列 值
```

- 行、列：1～9
- 值：1～9 填数，0 清空
- `0 0 0` 退出并自动存档

题面数字不能修改；与行/列/宫冲突的数字会被拒绝。

## 随机题面与唯一解

- `Block::generator()` 先填三个对角线宫，再用回溯生成完整解。
- `randomErase()` 从完整解随机挖空。
- 每挖掉一个空格，都用 MRV 求解器（`MRV()` 内部调用 `solve(..., 2)`）数解：
  - 返回 `1`：保留这个空格；
  - 返回 `2`：放回去，避免多解；
  - 返回 `0`：放回去。

难度 1～3 对应**最多** 20 / 40 / 60 次挖空尝试。因为多解时会放回，实际提示数会变化，不是固定值。

20 局样本测试结果：

```text
unique=20  multi=0  none=0  mismatch=0
```

## 存档

- 退出（`0 0 0`）时自动写 `save.txt`
- 下次启动选 `Y` 读取
- 想重新开始：删除 `save.txt`，或在启动时选 `N`

## 我改了什么

相对原项目，保留并自己实现了：

- 9×9 控制台棋盘
- 固定题面输入、题面锁定
- 行/列/宫冲突检测
- 完整解生成 + 随机挖空
- MRV 求解器与唯一解检查
- 难度选择
- 单文件存档 / 读档

砍掉了：

- 光标移动 / 撤销
- 颜色 / 边框 / 中英文
- 多存档

## 卡在哪

- 难度是按“挖空尝试次数”估的，不是严格按解题技巧分级。
- 完整解来自回溯生成器，随机性有限；不是均匀随机数独。
- 唯一性靠每次挖空后数解，生成比固定题慢。
- 只支持 Windows 控制台。

## 截图

![screenshot](docs/1.png)
![screenshot](docs/2.png)
![screenshot](docs/3.png)
