set(PLUGIN_NAME "VideoPlayer")

set(PROJECT_SOURCES
    videoplugin.cpp
    videoplugin.h
    qml.qrc
)

# 设置Qt自动生成的文件
qt_add_library(VideoPlayer MODULE
    ${PROJECT_SOURCES}
)

# 设置Qt模块
target_link_libraries(VideoPlayer
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Multimedia
)

# 设置包含目录
target_include_directories(VideoPlayer
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# 设置输出目录
set_target_properties(VideoPlayer
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    PREFIX ""
)

# 安装插件
install(TARGETS VideoPlayer
    LIBRARY DESTINATION plugins
)

# Copy the plugin to the plugins directory
add_custom_command(TARGET VideoPlayer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:VideoPlayer> "${CMAKE_BINARY_DIR}/bin/plugins/"
) 