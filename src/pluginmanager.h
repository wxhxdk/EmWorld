#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QString>
#include "plugininterface.h"

/**
 * @brief 插件管理器类
 * 
 * 负责插件的加载、卸载和管理
 * 提供插件状态的查询和操作接口
 * 支持QML界面的动态插件管理
 */
class PluginManager : public QObject
{
    Q_OBJECT
    // 导出已加载插件列表属性，只读，支持变更通知
    Q_PROPERTY(QStringList loadedPlugins READ loadedPlugins NOTIFY loadedPluginsChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     * 
     * 初始化插件管理器，设置插件搜索路径
     */
    explicit PluginManager(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     * 
     * 清理所有已加载的插件
     */
    ~PluginManager();

    /**
     * @brief 获取已加载的插件列表
     * @return 返回已加载插件的名称列表
     */
    QStringList loadedPlugins() const;

public slots:
    /**
     * @brief 加载所有可用插件
     * 
     * 扫描插件目录并加载所有有效的插件
     */
    void loadPlugins();

    /**
     * @brief 加载指定路径的插件
     * @param pluginPath 插件文件路径
     * @return 是否成功加载插件
     * 
     * 加载并初始化指定路径的插件
     * 成功加载后会触发pluginLoaded信号
     */
    bool loadPlugin(const QString &pluginPath);

    /**
     * @brief 卸载指定名称的插件
     * @param pluginName 插件名称
     * @return 是否成功卸载插件
     * 
     * 清理并卸载指定名称的插件
     * 成功卸载后会触发pluginUnloaded信号
     */
    bool unloadPlugin(const QString &pluginName);

    /**
     * @brief 获取插件的QML对象
     * @param pluginName 插件名称
     * @return 返回插件的QML对象指针，如果插件不存在则返回nullptr
     * 
     * 用于QML界面获取插件的交互对象
     */
    QObject* getPluginQmlObject(const QString &pluginName);

signals:
    /**
     * @brief 插件加载成功信号
     * @param pluginName 加载成功的插件名称
     */
    void pluginLoaded(const QString &pluginName);

    /**
     * @brief 插件卸载成功信号
     * @param pluginName 卸载成功的插件名称
     */
    void pluginUnloaded(const QString &pluginName);

    /**
     * @brief 已加载插件列表变更信号
     * 
     * 当插件被加载或卸载时触发
     * QML界面监听此信号以更新插件列表
     */
    void loadedPluginsChanged();

private slots:
    /**
     * @brief 处理插件加载完成
     * @param pluginName 插件名称
     */
    void handlePluginLoaded(const QString &pluginName);

    /**
     * @brief 处理插件卸载完成
     * @param pluginName 插件名称
     */
    void handlePluginUnloaded(const QString &pluginName);

private:
    /// 存储插件加载器的映射表
    QMap<QString, QPluginLoader*> m_pluginLoaders;
    /// 存储插件接口的映射表
    QMap<QString, PluginInterface*> m_plugins;

    /**
     * @brief 扫描并加载插件
     * 
     * 扫描插件目录中的所有插件文件
     * 尝试加载每个找到的插件
     */
    void scanPlugins();
};

#endif // PLUGINMANAGER_H 