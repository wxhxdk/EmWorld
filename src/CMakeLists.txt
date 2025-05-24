# 设置Qt自动生成的文件
qt_add_executable(QtPluginSystem
    main.cpp
    pluginmanager.cpp
    pluginmanager.h
    plugininterface.h
    qml.qrc
)

# 设置Qt模块
target_link_libraries(QtPluginSystem
    PRIVATE Qt6::Quick
)

# 设置Qt元对象编译器选项
target_compile_definitions(QtPluginSystem
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

# 设置包含目录
target_include_directories(QtPluginSystem
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置输出目录
set_target_properties(QtPluginSystem
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy the executable to the bin directory
add_custom_command(TARGET QtPluginSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/plugins"
) 