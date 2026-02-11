# 1.空间点索引

---

# 2.说明

```bash
.
├── cmake_build_debug.sh
├── CMakeLists.txt
├── example  # 代码示例存放处
│   └── CMakeLists.txt
├── format_count.sh
├── include  # 头文件存放处
│   └── math_utils.h
├── lib  # 动态库/静态库存放处
│   ├── CMakeLists.txt
│   └── math_utils.cpp
├── README.md
├── src  # 源文件存放处
│   ├── CMakeLists.txt
│   └── main.cpp
└── test  # 测试案例存放处
    ├── CMakeLists.txt
    ├── sub_foo
    │   └── sub_test_1.cpp
    ├── test_1.cpp
    └── test_2.cpp

7 directories, 14 files
```

---

# 3.CMake 优化指南

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

## 3.2 使用 Ninja

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

`Ninja`的构建速度很快。

## 3.3 使用现代 CMake

~~`include_directories(${YOUR_DIRECTORY})`~~

~~`link_directories(${YOUR_DIRECTORY})`~~

推荐：
```cmake
target_include_directories(myLib PRIVATE include/)
target_link_libraries(myApp PRIVATE myLib)
```

## 3.4 指定目标编译

如果只想编译`src`目录下的代码文件：
```bash
cmake --build build --target my_program -j12
```

这里的`my_program`与`src/CMakeLists.txt`下的`add_executable(my_program ${ALL_SRCS})`相对应。