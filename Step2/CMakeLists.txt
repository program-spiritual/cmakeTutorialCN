cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(Tutorial VERSION 1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置头文件以将某些 CMake 设置传递给源代码
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

# 将二叉树添加到包含文件的搜索路径
# 这样我们就可以找到 TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
