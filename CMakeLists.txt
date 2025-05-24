# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.16)

# 设置项目名称和版本
project(HelloWorld VERSION 1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 查找Qt包
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Quick
    Multimedia
)

# 设置插件输出目录
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)

# 添加主程序子目录
add_subdirectory(src)

# 添加插件子目录
add_subdirectory(plugins)

# 设置安装目标
install(TARGETS QtPluginSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
) 
