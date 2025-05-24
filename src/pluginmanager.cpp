#include "pluginmanager.h"
#include <QPluginLoader>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "PluginManager initialized";
    // 连接内部信号槽
    connect(this, &PluginManager::pluginLoaded,
            this, &PluginManager::handlePluginLoaded);
    connect(this, &PluginManager::pluginUnloaded,
            this, &PluginManager::handlePluginUnloaded);
}

PluginManager::~PluginManager()
{
    // 卸载所有插件
    for (auto pluginName : m_plugins.keys()) {
        unloadPlugin(pluginName);
    }
}

void PluginManager::loadPlugins()
{
    scanPlugins();
}

void PluginManager::scanPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    for (const QString &fileName : pluginsDir.entryList(QDir::Files)) {
        QString filePath = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader* loader = new QPluginLoader(filePath);
        
        if (auto plugin = qobject_cast<PluginInterface*>(loader->instance())) {
            QString pluginName = plugin->name();
            m_plugins[pluginName] = plugin;
            plugin->initialize();
            
            // 发送插件加载成功信号
            emit pluginLoaded(pluginName);
            qDebug() << "Loaded plugin:" << pluginName;
        } else {
            qDebug() << "Failed to load plugin:" << fileName;
            delete loader;
        }
    }
}

void PluginManager::handlePluginLoaded(const QString &pluginName)
{
    // 插件加载后的处理
    qDebug() << "Handling plugin loaded:" << pluginName;
    // 通知UI更新插件列表
    emit loadedPluginsChanged();
}

void PluginManager::handlePluginUnloaded(const QString &pluginName)
{
    // 插件卸载后的处理
    qDebug() << "Handling plugin unloaded:" << pluginName;
    // 通知UI更新插件列表
    emit loadedPluginsChanged();
}

bool PluginManager::loadPlugin(const QString &pluginPath)
{
    // 创建插件加载器
    QPluginLoader loader(pluginPath);
    
    // 尝试加载插件
    QObject *plugin = loader.instance();
    if (!plugin) {
        qWarning() << "Failed to load plugin:" << loader.errorString();
        return false;
    }

    // 获取插件接口
    PluginInterface *pluginInterface = qobject_cast<PluginInterface*>(plugin);
    if (!pluginInterface) {
        qWarning() << "Plugin does not implement PluginInterface";
        loader.unload();
        return false;
    }

    // 初始化插件
    pluginInterface->initialize();
    
    // 存储插件实例
    QString pluginName = pluginInterface->name();
    m_plugins[pluginName] = pluginInterface;
    
    // 发送信号通知UI
    emit pluginLoaded(pluginName);
    emit loadedPluginsChanged();
    
    qDebug() << "Plugin loaded successfully:" << pluginName;
    return true;
}

bool PluginManager::unloadPlugin(const QString &pluginName)
{
    // 检查插件是否存在
    if (!m_plugins.contains(pluginName)) {
        qWarning() << "Plugin not found:" << pluginName;
        return false;
    }

    // 获取插件实例
    PluginInterface *plugin = m_plugins[pluginName];
    
    // 关闭插件
    plugin->shutdown();
    
    // 从管理器中移除插件
    m_plugins.remove(pluginName);
    
    // 删除插件实例
    delete plugin;
    
    // 发送信号通知UI
    emit pluginUnloaded(pluginName);
    emit loadedPluginsChanged();
    
    qDebug() << "Plugin unloaded successfully:" << pluginName;
    return true;
}

QObject* PluginManager::getPluginQmlObject(const QString &pluginName)
{
    // 检查插件是否存在
    if (!m_plugins.contains(pluginName)) {
        qWarning() << "Plugin not found:" << pluginName;
        return nullptr;
    }

    // 返回插件的QML对象
    return m_plugins[pluginName]->getQmlObject();
}

QStringList PluginManager::loadedPlugins() const
{
    return m_plugins.keys();
} 
