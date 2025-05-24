#include "videoplugin.h"
#include <QUrl>
#include <QDebug>

/**
 * @brief 视频播放器QML对象类
 * 
 * 为QML提供视频播放功能的接口类
 * 管理视频源和播放状态，提供属性和方法供QML使用
 * 实现了视频文件路径的管理和变更通知
 */
class VideoPluginObject : public QObject
{
    Q_OBJECT
    // 导出QML路径属性，只读
    Q_PROPERTY(QString qmlPath READ qmlPath CONSTANT)
    // 导出当前视频路径属性，支持读写和变更通知
    Q_PROPERTY(QString currentVideo READ currentVideo WRITE setCurrentVideo NOTIFY currentVideoChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit VideoPluginObject(QObject *parent = nullptr) 
        : QObject(parent)
        , m_currentVideo("")  // 初始化为空视频路径
    {}

    /**
     * @brief 获取QML文件路径
     * @return 返回视频播放器界面的QML文件路径
     */
    QString qmlPath() const { 
        QString path = QStringLiteral("qrc:/VideoPlayer.qml");
        qDebug() << "Plugin QML path:" << path;
        return path;
    }

    /**
     * @brief 获取当前视频路径
     * @return 返回当前正在播放的视频文件路径
     */
    QString currentVideo() const { return m_currentVideo; }

public slots:
    /**
     * @brief 设置当前视频路径
     * @param video 新的视频文件路径
     * 
     * 当设置新的视频路径时，如果路径发生变化，
     * 会触发currentVideoChanged信号通知QML更新界面
     */
    void setCurrentVideo(const QString &video) {
        if (m_currentVideo != video) {
            m_currentVideo = video;
            emit currentVideoChanged();
        }
    }

signals:
    /**
     * @brief 当前视频变更信号
     * 
     * 当视频路径发生变化时发出此信号
     * QML界面会监听此信号并更新视频播放器
     */
    void currentVideoChanged();

private:
    QString m_currentVideo;  ///< 存储当前视频文件路径
};

/**
 * @brief VideoPlugin的私有实现类
 * 
 * 使用PIMPL模式封装插件的实现细节
 * 管理插件的QML对象实例
 */
class VideoPlugin::VideoPluginPrivate
{
public:
    VideoPluginObject* qmlObject;  ///< 插件的QML对象实例
};

// VideoPlugin类的实现

VideoPlugin::VideoPlugin(QObject *parent)
    : QObject(parent)
    , d(new VideoPluginPrivate)  // 创建私有实现实例
{
    // 创建QML对象并设置父对象关系
    d->qmlObject = new VideoPluginObject(this);
}

VideoPlugin::~VideoPlugin()
{
    // 清理私有实现
    delete d;
}

void VideoPlugin::initialize()
{
    // 插件初始化时的处理
    qDebug() << "VideoPlayer plugin initialized";
}

void VideoPlugin::shutdown()
{
    // 插件关闭时的处理
    qDebug() << "VideoPlayer plugin shutdown";
}

QObject* VideoPlugin::getQmlObject()
{
    // 返回QML对象实例
    return d->qmlObject;
}

#include "videoplugin.moc"  // 包含自动生成的元对象代码 