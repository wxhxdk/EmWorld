#ifndef VIDEOPLUGIN_H
#define VIDEOPLUGIN_H

#include <QObject>
#include "../../src/plugininterface.h"

/**
 * @brief 视频播放器插件类
 * 
 * 实现了PluginInterface接口，提供视频播放功能
 * 使用PIMPL模式隐藏实现细节，提供清晰的插件接口
 * 通过QML界面实现视频播放器的用户交互
 */
class VideoPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid)  // 声明这是一个Qt插件
    Q_INTERFACES(PluginInterface)               // 实现PluginInterface接口

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     * 
     * 创建插件实例并初始化必要的资源
     */
    explicit VideoPlugin(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     * 
     * 清理插件资源，确保正确释放内存
     */
    ~VideoPlugin() override;

    // PluginInterface接口实现
    QString name() const override { return "VideoPlayer"; }        ///< 返回插件名称
    QString version() const override { return "1.0"; }            ///< 返回插件版本
    QString description() const override { return "Video Player Plugin"; }  ///< 返回插件描述

    /**
     * @brief 初始化插件
     * 
     * 在插件被加载时调用，用于初始化插件资源
     * 创建QML对象并设置必要的属性
     */
    void initialize() override;

    /**
     * @brief 关闭插件
     * 
     * 在插件被卸载时调用，用于清理资源
     * 确保所有资源被正确释放
     */
    void shutdown() override;

    /**
     * @brief 获取插件的QML对象
     * @return 返回用于QML交互的对象指针
     * 
     * 返回插件的QML对象，该对象包含了视频播放器的UI和控制接口
     */
    QObject* getQmlObject() override;

private:
    class VideoPluginPrivate;           ///< 前向声明私有实现类
    VideoPluginPrivate* d;              ///< 指向私有实现的指针
};

#endif // VIDEOPLUGIN_H 