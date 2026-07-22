// qmllint disable
import QtQuick
import QtQuick.Effects

Rectangle {
    id: root
    width: 80
    height: 80
    radius: width / 2
    color: Qt.rgba(1, 1, 1, 0.0) // 투명 배경
    anchors.centerIn: parent

    property bool isVisible: true
    property int animationDuration: 300
    property var animationEasing: Easing.InOutQuad

    enabled: isVisible
    opacity: isVisible ? 1.0 : 0.0
    Behavior on opacity {
        NumberAnimation {
            duration: root.animationDuration;
            easing.type: root.animationEasing
        }
    }

    Image {
        id: logoImage
        anchors.centerIn: parent
        source: "../../assets/logo.png" // QML 파일 기준 로고 이미지 경로
        fillMode: Image.PreserveAspectFit
        // smooth: true // 이미지 스무딩
        mipmap: true // 이미지 품질 향상
        width: parent.width
        height: parent.height
        visible: false
    }

    MultiEffect {
        anchors.fill: logoImage
        source: logoImage
        brightness: 1.0
    }

    signal toggled()

    MouseArea {
        anchors.fill: parent
        enabled: root.enabled
        cursorShape: Qt.PointingHandCursor
        onClicked: root.toggled()
    }
}
