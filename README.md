# Qt Plugin System

这是一个基于Qt6的插件管理系统示例，包含一个视频播放插件的实现。

## 功能特点

- 支持动态加载和卸载插件
- 使用QML构建现代化UI界面
- 插件与主程序通过信号槽通信
- 包含视频播放插件示例
- 跨平台支持

## 系统要求

- Qt 6.0 或更高版本
- CMake 3.16 或更高版本
- 支持C++17的编译器

## 构建说明

1. 确保已安装Qt6和CMake
2. 克隆此仓库
3. 创建构建目录：
   ```bash
   mkdir build
   cd build
   ```
4. 配置项目：
   ```bash
   cmake ..
   ```
5. 构建项目：
   ```bash
   cmake --build .
   ```

## 项目结构

```
QtPluginSystem/
├── src/                    # 主程序源代码
│   ├── main.cpp           # 程序入口
│   ├── pluginmanager.h    # 插件管理器
│   ├── pluginmanager.cpp
│   └── plugininterface.h  # 插件接口
├── plugins/               # 插件目录
│   └── videoPlayer/      # 视频播放插件
└── CMakeLists.txt        # CMake配置文件
```

## 使用说明

1. 运行主程序后，系统会自动加载plugins目录下的插件
2. 在左侧面板可以查看已加载的插件列表
3. 点击"Unload"按钮可以卸载相应的插件
4. 视频播放插件支持以下功能：
   - 打开视频文件
   - 播放/暂停
   - 进度条控制
   - 键盘快捷键（空格：播放/暂停，左右方向键：快进/快退）

## 开发新插件

1. 创建新的插件目录
2. 实现PluginInterface接口
3. 创建插件的QML界面
4. 在plugins/CMakeLists.txt中添加新插件

## 许可证

MIT License 