#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QDir>
#include "pluginmanager.h"

using namespace Qt::StringLiterals;

/**
 * @brief 主程序入口函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 程序退出码
 * 
 * 初始化Qt应用程序和QML引擎
 * 创建插件管理器并注册到QML
 * 加载主界面QML文件
 */
int main(int argc, char *argv[])
{
    // 设置应用程序属性
    QGuiApplication::setApplicationName("Qt Plugin System");
    QGuiApplication::setOrganizationName("Example");
    QGuiApplication::setOrganizationDomain("example.com");

    // 创建Qt应用程序实例
    // QGuiApplication用于GUI应用程序，提供窗口系统集成
    QGuiApplication app(argc, argv);

    // 创建插件管理器实例
    // PluginManager负责插件的加载、卸载和管理
    PluginManager pluginManager;

    // 创建QML引擎实例
    // QQmlApplicationEngine用于加载和管理QML文档
    QQmlApplicationEngine engine;
    
    // 将插件管理器注册到QML环境
    // 注意：这必须在加载QML文件之前完成
    engine.rootContext()->setContextProperty("pluginManager", &pluginManager);

    // 输出调试信息，显示应用程序的工作目录
    qDebug() << "Current directory:" << QDir::currentPath();
    qDebug() << "Application directory:" << QCoreApplication::applicationDirPath();

    // 设置主QML文件的URL
    // 使用QStringLiteral可以在编译时创建字符串，提高性能
    const QUrl url(u"qrc:/main.qml"_s);
    
    // 添加QML加载错误处理
    // 当QML文件加载出现警告时，将警告信息输出到控制台
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                    [](const QList<QQmlError> &warnings) {
        for (const QQmlError &error : warnings) {
            qWarning() << "QML Warning:" << error.toString();
        }
    });

    // 监听QML对象创建
    // 如果主QML文档加载失败，则退出应用程序
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qWarning() << "Failed to create QML object for:" << url;
            QCoreApplication::exit(-1);
        } else if (obj) {
            qDebug() << "Successfully created QML object for:" << url;
        }
    }, Qt::QueuedConnection);

    // 加载主QML文件
    engine.load(url);

    // 检查QML是否成功加载
    if (engine.rootObjects().isEmpty()) {
        qDebug() << "Failed to load QML file:" << url;
        return -1;
    }
    
    // 加载插件
    // 在QML引擎准备就绪后加载插件，确保UI系统已经初始化
    pluginManager.loadPlugins();

    // 启动应用程序事件循环
    // 应用程序将持续运行，直到退出
    return app.exec();
} 
