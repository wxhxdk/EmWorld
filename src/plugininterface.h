#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QtPlugin>

/**
 * @brief 插件接口类
 * 
 * 定义了插件系统中所有插件必须实现的接口
 * 提供插件的基本信息和生命周期管理方法
 * 支持QML界面的插件开发
 */
class PluginInterface {
public:
    /**
     * @brief 虚析构函数
     * 
     * 确保正确释放派生类资源
     */
    virtual ~PluginInterface() = default;
    
    /**
     * @brief 获取插件名称
     * @return 返回插件的唯一标识名称
     */
    virtual QString name() const = 0;

    /**
     * @brief 获取插件版本
     * @return 返回插件的版本号
     */
    virtual QString version() const = 0;

    /**
     * @brief 获取插件描述
     * @return 返回插件的功能描述
     */
    virtual QString description() const = 0;
    
    /**
     * @brief 初始化插件
     * 
     * 在插件被加载时调用
     * 用于初始化插件资源和状态
     */
    virtual void initialize() = 0;

    /**
     * @brief 关闭插件
     * 
     * 在插件被卸载时调用
     * 用于清理插件资源和状态
     */
    virtual void shutdown() = 0;

    /**
     * @brief 获取插件的QML对象
     * @return 返回用于QML交互的对象指针
     * 
     * 如果插件提供QML界面，需要实现此方法
     * 返回的对象将被注册到QML上下文中
     */
    virtual QObject* getQmlObject() = 0;
};

// 定义插件接口的唯一标识符
#define PluginInterface_iid "com.example.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H 