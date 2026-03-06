# 1. 空间点索引

## 1.1 算法概述

本项目实现了基于**网格索引（Grid Index）**的空间点索引系统，用于高效管理和查询空间点数据。网格索引是一种简单而有效的空间索引算法，特别适合处理均匀分布的点数据。

## 1.2 核心算法原理

### 1.2.1 网格划分

网格索引的核心思想是将空间划分为固定大小的网格单元：

- **空间范围**：通过指定最小/最大经度和纬度来定义整个空间范围
- **网格大小**：设置每个网格单元的大小（`cellSize`）
- **网格索引计算**：对于每个点，根据其经纬度计算所属的网格单元索引

```cpp
// 计算网格单元索引的核心代码
std::pair<int, int> GridIndex::getCellIndex(double longitude, double latitude) const {
  int xIndex = static_cast<int>((longitude - minLon) / cellSize);
  int yIndex = static_cast<int>((latitude - minLat) / cellSize);
  return {xIndex, yIndex};
}
```

### 1.2.2 数据存储

- 使用嵌套的哈希表（`std::unordered_map`）存储网格单元：
  - 外层哈希表以 x 坐标索引为键
  - 内层哈希表以 y 坐标索引为键
  - 每个网格单元存储一个点的向量（`std::vector<Point>`）

### 1.2.3 核心操作

1. 插入操作
  - 计算点所属的网格单元索引
  - 将点添加到对应网格单元的点列表中

2. 删除操作
  - 计算点所属的网格单元索引
  - 从对应网格单元的点列表中移除该点

3. 查找操作
  - 计算点所属的网格单元索引
  - 在对应网格单元的点列表中查找该点

4. 范围查询
  - 计算查询范围覆盖的网格单元范围
  - 遍历所有覆盖的网格单元
  - 检查每个网格单元中的点是否在查询范围内
  - 收集符合条件的点

## 1.3 数据结构

### 1.3.1 点（Point）

- 存储经纬度信息
- 提供比较运算符重载

### 1.3.2 网格单元（GridCell）

- 存储该网格单元中的所有点

### 1.3.3 网格索引（GridIndex）

- 管理整个网格系统
- 提供点的插入、删除、查找和范围查询功能

### 1.3.4 点管理器（PointManager）

- 封装网格索引
- 提供文件读写功能
- 统一管理点数据

## 1.4 性能分析

- **时间复杂度**：
  - 插入、删除、查找：O(1) 平均情况下，因为哈希表操作平均时间复杂度为 O(1)
  - 范围查询：O(k + n)，其中 k 是查询范围内的网格单元数量，n 是这些网格单元中的点数量

- **空间复杂度**：O(n)，其中 n 是点的数量

## 1.5 适用场景

- 点数据均匀分布的场景
- 需要频繁进行范围查询的场景
- 数据量适中的场景

## 1.6 使用示例

```cpp
// 创建网格索引（范围和网格大小根据实际情况设置）
PointManager manager(110.0, 120.0, 30.0, 40.0, 0.1);

// 从文件加载点数据
manager.loadFromFile("data/points.txt");

// 插入点
Point point(116.3, 39.9);
manager.insertPoint(point);

// 范围查询
std::vector<Point> results = manager.rangeQuery(116.0, 117.0, 39.5, 40.5);

// 保存点数据到文件
manager.saveToFile("data/output.txt");
```

## 1.7 优势与局限性

**优势**：
- 实现简单，易于理解和维护
- 对于均匀分布的数据，查询效率较高
- 内存使用合理

**局限性**：
- 对于点数据分布不均匀的场景，可能存在网格单元负载不均衡的问题
- 网格大小的选择对性能影响较大，需要根据实际数据情况进行调优
- 不适合处理动态变化的空间范围

---

# 2. 项目结构

```bash
.
├── CMakeLists.txt
├── README.md
├── cmake_build_debug.sh
├── data
│   └── points.txt
├── example
│   └── CMakeLists.txt
├── format_count.sh
├── include
│   ├── GridIndex.h
│   ├── Point.h
│   └── PointManager.h
├── lib
│   ├── CMakeLists.txt
│   ├── GridIndex.cpp
│   ├── Point.cpp
│   └── PointManager.cpp
├── src
│   ├── CMakeLists.txt
│   └── main.cpp
└── test
    ├── CMakeLists.txt
    └── test01.cpp
```

---

# 3. CMake 优化指南

## 3.1 最小化 GLOB 操作

~~`file(GLOB ...)`~~

推荐：
```cmake
set(SOURCES
    main.cpp
    utiles/utiles.cpp
    ...
)

add_executable(myApp ${SOURCES})
```

>> - **CMake 文档明确指出：**
>> "We do not recommend using GLOB to collect a list of source files from your source tree. If no CMakeLists.txt file changes when a source is added or removed then the generated build system cannot know when to ask CMake to regenerate."

## 3.2 使用现代 CMake

~~`include_directories(${YoUR_DIRECToRY})`~~

~~`link_directories(${YOUR_DIRECTORY})`~~

推荐：
```cmake
target_include_directories(myLib PRIVATE include/)
target_link_libraries(myApp PRIVATE myLib)
```

## 3.3 使用 Ninja

> `Ninja`的构建速度很快。

如果采用的是 LLVM：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

如果采用的是 GNU：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
```

如果采用的是 MSVC：

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
```

## 3.4 指定目标编译

如果只想编译 `src` 目录下的代码文件：

```bash
cmake --build build --target my_program -j12
```

> 这里的 `my_program` 与 `src/CMakeLists.txt` 下的 `add_executable(my_program ${ALL_SRCS})` 相对应。

---

# 4. 一些问题

> 下面是可能在编写代码时遇到的问题，以及对应的解决办法。

## 4.1 Windows 下编码问题

> Windows 的控制台编码一般采用的是 GB2312 编码的，但源代码编码采用的是 UTF-8 编码（不建议把源代码文件编码改为 GB2312 格式！），所以当有中文在控制台输出时，会产生乱码。解决方法如下：

- 方式一：如果使用的是 **cmd**，在每次从终端执行可编译好的程序之前，执行：

```shell
chcp 65001
```

执行完后，再执行程序就不会乱码。

- 方式二：如果使用的是 **PowerShell**，可以修改配置文件：

打开配置文件（如果没有的话，会创建）：

```shell
notepad $PROFILE
```

在配置文件中写入：

```shell
# 设置默认编码为 UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
[Console]::InputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# 设置环境变量，让子进程也使用 UTF-8
$env:PYTHONIOENCODING = "utf-8"
```

修改完毕后，重启 PowerShell 即可。

如果是在 VS 中运行程序，因为默认打开的是 cmd，所以建议在代码 `main` 函数开头添加：

```cpp
int main(){
  // 设置控制台为 UTF-8
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  
  // 其它代码......
}
```

如果采用 MSVC 编译，需要在 `CMakeLists.txt` 中给目标程序添加编译选项：

```cmake
# MSVC UTF-8 支持
if(MSVC)
  target_compile_options(my_app PRIVATE /utf-8)
endif()
```

## 4.2 无法找到头文件

> 在 **VSCode + clangd** 配置下，可能会出现无法找到标准库头文件的情况，导致 clangd 报错，但是编译是可以通过的。一般单独分别安装 llvm 和 mingw 时，会出现这个错误。这个错误是因为 **clangd 无法找到 MinGW 的标准库头文件路径**。虽然你生成了 `compile_commands.json`，但里面的编译器是 MinGW 的 `g++.exe`，而 clangd 需要知道这些头文件在哪里。

其问题根源是 clangd 和 MinGW 是两个不同的工具链：
- **clangd** 基于 LLVM/Clang，默认在 MSVC 或自身 libc++ 环境下查找头文件
- **MinGW** 使用 GCC 的 libstdc++，头文件路径不同

### 4.2.1 方案 1：使用集成有 llvm 的 mingw

下载：[winlibs](https://winlibs.com/)

选择 **UCRT runtime** 版并且包含有 **LLVM/Clang/LLD/LLDB** 的下载。

### 4.2.2 方案 2：配置 clangd 参数

> 比如我这里使用的是 CLion 集成的 mingw。

在 VS Code 的 `settings.json` 中添加：

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build", // 指定 compile_commands.json 所在目录
        "--query-driver=C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe", // 查询指定编译器的系统 include 路径
        "--background-index", // 后台索引整个项目，加速代码跳转
        "--header-insertion=iwyu", // 自动插入头文件时遵循 "Include What You Use" 原则
        "--completion-style=bundled", // 代码补全风格：打包式
        "--pch-storage=memory", // 预编译头文件存储在内存中
        "--cross-file-rename", // 启用跨文件重命名功能
        "--enable-config", // 允许从 .clangd 文件读取配置
    ],
}
```

**关键参数**：
- `--query-driver`：告诉 clangd 查询指定编译器的系统 include 路径

再在项目根目录下创建 `.clangd` 文件，在里面手动指定 MinGW 的 C++ 标准库路径：

```yaml
CompileFlags:
  Add:
    - -std=c++20
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include/c++/x86_64-w64-mingw32
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/lib/gcc/x86_64-w64-mingw32/13.1.0/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/include
    - -IC:/Apps/CLion 2025.3.2/bin/mingw/x86_64-w64-mingw32/include
  Compiler: C:/Apps/CLion 2025.3.2/bin/mingw/bin/g++.exe
```

> 把上面的路径改成你自己的。

### 4.2.3 方案 3：改用 Clang 编译器

可以尝试直接用 Clang 编译，避免工具链混用：

```bash
# 在 CMake 配置时指定 Clang
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
```

确保 `clang++` 能找到 MinGW 的 libstdc++，或使用 Clang 自带的 libc++。

## 4.3 使用 MSVC 编译器

> 有时候不可避免得要用到 MSVC 编译器，因为网上有不少预编译好的 Windows 第三方库，都是用的 MSVC 编译器。

下载后 VS 时（比如下载的是 VS 2022 Community），会自动安装 **Developer Command Prompt for VS 2022** 和 **Developer PowerShell for VS 2022**，这两个终端集成了 MSVC 编译器的环境变量。在该终端中使用指令 `where.exe cl` 会有如下输出：

```shell
PS C:\Apps\Microsoft Visual Studio\2022\Community> where.exe cl
C:\Apps\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe
```

打开 **Developer PowerShell for VS 2022**，然后用 `cd` 命令进入到项目根目录，输入：

```shell
# 打开 VSCode
code .

# 或者打开 CLion
clion64.exe .
```

这样打开的 IDE 就会继承  **Developer PowerShell for VS 2022** 中的环境。

> 前提是要把 VSCode 和 CLion 添加到环境变量中。这里建议使用  **Developer PowerShell for VS 2022** 而不是 **Developer Command Prompt for VS 2022**，因为前者的命令行功能更强大。

在编译时指定编译器：`-DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl`。

如果使用 MSVC 生成器（Visual Studio Generator），是不会生成 `compile_commands.json` 文件的：

```shell
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
```

因此，除非要使用 VS 进行开发，其他情况生成器请选择 Ninja 或者 Unix Makefiles。
