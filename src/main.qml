import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 主应用程序窗口
 * 提供插件管理的主界面，包含插件列表和插件内容显示区域
 */
ApplicationWindow {
    id: window
    width: 1024
    height: 768
    visible: true
    title: qsTr("Qt Plugin System")

    // Add debug function
    function debugObject(obj) {
        if (!obj) {
            console.log("Object is null")
            return
        }
        for (var prop in obj) {
            console.log(prop + ":", obj[prop])
        }
    }

    // 监听插件管理器的信号
    Connections {
        target: pluginManager
        
        // 插件加载成功时的处理
        function onPluginLoaded(pluginName) {
            console.log("Plugin loaded:", pluginName)
        }
        
        // 插件卸载时的处理
        function onPluginUnloaded(pluginName) {
            console.log("Plugin unloaded:", pluginName)
        }
        
        // 插件列表变化时的处理
        function onLoadedPluginsChanged() {
            if (pluginManager) {
                console.log("Loaded plugins changed, current plugins:", pluginManager.loadedPlugins)
            } else {
                console.warn("pluginManager is null in onLoadedPluginsChanged")
            }
        }
    }

    // 主布局：使用RowLayout实现左右分栏
    RowLayout {
        anchors.fill: parent
        spacing: 10

        // 左侧：插件列表面板
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#f0f0f0"  // 浅灰色背景
            
            // 插件列表的垂直布局
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // 标题文本
                Label {
                    text: qsTr("Loaded Plugins")
                    font.bold: true
                    font.pixelSize: 16
                }

                // 插件列表视图
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // 使用pluginManager的loadedPlugins属性作为数据源
                    model: pluginManager ? pluginManager.loadedPlugins : []
                    // 列表项委托
                    delegate: ItemDelegate {
                        width: parent.width
                        text: modelData  // 显示插件名称
                        
                        // 卸载按钮
                        Button {
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Unload"
                            // 点击时卸载对应的插件
                            onClicked: {
                                if (pluginManager) {
                                    pluginManager.unloadPlugin(modelData)
                                }
                            }
                        }
                    }
                }
            }
        }

        // 右侧：插件内容显示区域
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffffff"  // 白色背景
            
            // 插件内容的垂直布局
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // 标题文本
                Label {
                    text: qsTr("Plugin Content")
                    font.bold: true
                    font.pixelSize: 16
                }

                // 插件内容列表视图
                ListView {
                    id: pluginContentView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // 使用相同的数据源
                    model: pluginManager ? pluginManager.loadedPlugins : []
                    // 内容显示委托
                    delegate: Component {
                        Item {
                            width: pluginContentView.width
                            height: 400

                            // 使用Loader动态加载插件的QML界面
                            Loader {
                                id: pluginLoader
                                anchors.fill: parent
                                // 获取插件的QML对象
                                property var pluginObject: pluginManager ? 
                                    pluginManager.getPluginQmlObject(modelData) : null
                                // 设置QML源
                                source: pluginObject ? pluginObject.qmlPath : ""
                                
                                // 监听加载状态
                                onStatusChanged: {
                                    if (status === Loader.Error) {
                                        console.error("Error loading plugin QML:", source)
                                    } else if (status === Loader.Ready && item) {
                                        // 加载成功后，将插件对象传递给QML
                                        item.pluginObject = pluginObject
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
} 