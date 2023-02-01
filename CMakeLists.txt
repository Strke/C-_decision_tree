#CMake 最低版本号要求
cmake_minimum_required(VERSION 3.25)

#指定工程名
project(decision_tree)

#指定C/C++编译器
set(CMAKE_C_COMPILER "D:/MinGW/bin/gcc")
set(CMAKE_CXX_COMPILER "D:/MinGW/bin/g++")

#指定C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#指定头文件所在的路径
# include_directories(${PROJECT_SOURCE_DIR}/include)
#在 ${PROJECT_SOURCE_DIR}/src 路径下搜索.cpp源文件
# aux_source_directory(${PROJECT_SOURCE_DIR}/src ALL_SRCS)
#查找当前目录下的所有源文件
#并将名称保存到 DIR_SRCS 变量
#aux_source_directory(. DIR_SRCS)
#指定生成的可执行文件夹的路径
#set(EXECUTABLE_OUTPUT_PATH .)
#生成可执行文件 main.exe
#add_executable(main ${ALL_SRCS})

include_directories(./include)
add_executable(main src/cart.cpp src/tool.cpp src/main.cpp)