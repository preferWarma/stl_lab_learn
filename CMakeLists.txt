# CMakeLists.txt
cmake_minimum_required(VERSION 3.20) # cmake的最低版本要求

project(code_template_cmake CXX) # 项目名称 CXX表示C++项目

add_subdirectory(lib) # 添加lib文件夹作为子目录

set(exeFileName ${PROJECT_NAME}) # 设置可执行文件名称

add_executable(${exeFileName} main.cpp) # 添加可执行文件

target_link_libraries(${exeFileName} PUBLIC MyLib) # 添加链接库, MyLib为lib文件夹下CMakeLists.txt中定义的MyLib