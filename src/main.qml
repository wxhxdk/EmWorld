import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 主应用程序窗口
 * 提供插件管理的主界面，包含插件列表和插件内容显示区域
 */
ApplicationWindow {
    id: window
    width: 390  // iPhone 12 width
    height: 844 // iPhone 12 height
    visible: true
    title: qsTr("EmWorld")

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

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.bottomMargin: tabBar.height // 为底部导航栏留出空间

        // 首页
        Page {
            Rectangle {
                anchors.fill: parent
                color: "#f8f8f8"
                Label {
                    anchors.centerIn: parent
                    text: qsTr("首页")
                    font.pixelSize: 20
                }
            }
        }

        // 课堂
        Page {
            Rectangle {
                anchors.fill: parent
                color: "#f8f8f8"
                Label {
                    anchors.centerIn: parent
                    text: qsTr("课堂")
                    font.pixelSize: 20
                }
            }
        }

        // 说说
        Page {
            Rectangle {
                anchors.fill: parent
                color: "#f8f8f8"
                Label {
                    anchors.centerIn: parent
                    text: qsTr("说说")
                    font.pixelSize: 20
                }
            }
        }

        // 我的（原主界面内容）
        Page {
            id: myPage
            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                // 顶部标题
                Rectangle {
                    Layout.fillWidth: true
                    height: 50
                    color: "#f0f0f0"
                    Label {
                        anchors.centerIn: parent
                        text: qsTr("插件管理")
                        font.bold: true
                        font.pixelSize: 18
                    }
                }

                // 插件列表
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.margins: 10
                    clip: true
                    spacing: 10
                    model: pluginManager ? pluginManager.loadedPlugins : []
                    delegate: Rectangle {
                        width: parent.width
                        height: modelData.includes("VideoPlayer") ? 400 : 120  // 视频播放器使用更大的高度
                        radius: 8
                        color: "#ffffff"
                        border.color: "#e0e0e0"
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 5

                            // 插件名称和卸载按钮
                            RowLayout {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 30
                                Label {
                                    text: modelData
                                    font.bold: true
                                    font.pixelSize: 16
                                    Layout.fillWidth: true
                                }
                                Button {
                                    text: qsTr("卸载")
                                    onClicked: {
                                        if (pluginManager) {
                                            pluginManager.unloadPlugin(modelData)
                                        }
                                    }
                                }
                            }

                            // 插件内容区域
                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.preferredHeight: modelData.includes("VideoPlayer") ? 350 : 70  // 视频播放器使用更大的高度
                                color: "#f8f8f8"
                                radius: 4

                                Loader {
                                    anchors.fill: parent
                                    anchors.margins: modelData.includes("VideoPlayer") ? 0 : 5  // 视频播放器不需要内边距
                                    property var pluginObject: pluginManager ? 
                                        pluginManager.getPluginQmlObject(modelData) : null
                                    source: pluginObject ? pluginObject.qmlPath : ""
                                    
                                    onStatusChanged: {
                                        if (status === Loader.Error) {
                                            console.error("Error loading plugin QML:", source)
                                        } else if (status === Loader.Ready && item) {
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

    // 底部导航栏
    TabBar {
        id: tabBar
        width: parent.width
        anchors.bottom: parent.bottom
        currentIndex: stackLayout.currentIndex
        onCurrentIndexChanged: stackLayout.currentIndex = currentIndex
        background: Rectangle {
            color: "#ffffff"
            Rectangle {
                anchors.top: parent.top
                width: parent.width
                height: 1
                color: "#e0e0e0"
            }
        }

        TabButton {
            text: qsTr("首页")
            width: tabBar.width / 4
        }
        TabButton {
            text: qsTr("课堂")
            width: tabBar.width / 4
        }
        TabButton {
            text: qsTr("说说")
            width: tabBar.width / 4
        }
        TabButton {
            text: qsTr("我的")
            width: tabBar.width / 4
        }
    }
} 