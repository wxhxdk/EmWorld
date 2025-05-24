import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia

/**
 * 视频播放器插件的主界面
 * 提供视频播放、暂停、进度控制等基本功能
 * 支持通过文件对话框选择视频文件
 * 支持键盘快捷键控制播放
 */
Item {
    id: root
    width: parent.width
    height: parent.height
    
    // 插件对象属性，由主程序传入，用于与C++层通信
    property var pluginObject

    // 视频源属性，绑定到插件对象的currentVideo属性
    property string videoSource: pluginObject ? pluginObject.currentVideo : ""

    // 主布局容器
    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // 视频播放区域
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"  // 视频未播放时的背景色

            // 视频播放组件
            Video {
                id: videoPlayer
                anchors.fill: parent
                source: root.videoSource
                fillMode: VideoOutput.PreserveAspectFit  // 保持视频比例
                focus: true  // 接收键盘焦点

                // 键盘快捷键控制
                Keys.onSpacePressed: videoPlayer.playbackState === MediaPlayer.PlayingState ? 
                                   videoPlayer.pause() : videoPlayer.play()  // 空格键：播放/暂停
                Keys.onLeftPressed: videoPlayer.position -= 5000  // 左方向键：后退5秒
                Keys.onRightPressed: videoPlayer.position += 5000  // 右方向键：前进5秒
            }

            // 点击视频区域控制播放/暂停
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (videoPlayer.playbackState === MediaPlayer.PlayingState)
                        videoPlayer.pause()
                    else
                        videoPlayer.play()
                }
            }
        }

        // 控制面板
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            spacing: 10

            // 播放/暂停按钮
            Button {
                text: videoPlayer.playbackState === MediaPlayer.PlayingState ? "Pause" : "Play"
                onClicked: {
                    if (videoPlayer.playbackState === MediaPlayer.PlayingState)
                        videoPlayer.pause()
                    else
                        videoPlayer.play()
                }
            }

            // 进度控制滑块
            Slider {
                Layout.fillWidth: true
                from: 0
                to: videoPlayer.duration  // 视频总时长
                value: videoPlayer.position  // 当前播放位置
                onMoved: videoPlayer.position = value  // 拖动滑块改变播放位置
            }

            // 打开文件按钮
            Button {
                text: "Open File"
                onClicked: fileDialog.open()
            }
        }
    }

    // 文件选择对话框
    FileDialog {
        id: fileDialog
        title: "Please choose a video file"
        nameFilters: ["Video files (*.mp4 *.avi *.mkv *.mov)"]  // 支持的视频格式
        onAccepted: {
            if (pluginObject) {
                // 设置新的视频文件并开始播放
                pluginObject.currentVideo = fileDialog.selectedFile
                videoPlayer.play()
            }
        }
    }

    // 组件初始化完成后的处理
    Component.onCompleted: {
        console.log("VideoPlayer.qml loaded")
        console.log("pluginObject:", pluginObject)
        if (pluginObject) {
            console.log("currentVideo:", pluginObject.currentVideo)
        }
    }
} 