// qmllint disable
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    implicitWidth: Math.max(10, buttonRow.implicitWidth + 80) // 최소 너비 10, 내부 콘텐츠에 따라 조정
    implicitHeight: Math.max(10, buttonRow.implicitHeight + 40) // 최소 높이 10, 내부 콘텐츠에 따라 조정
    radius: 100
    color: Qt.rgba(1, 1, 1, 0.15) 
    border.color: Qt.rgba(1, 1, 1, 0.4)
    border.width: 1
    
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom

    property bool isVisible: false
    property int animationDuration: 300
    property var animationEasing: Easing.InOutQuad 

    enabled: isVisible
    visible: opacity > 0
    opacity: isVisible ? 1.0 : 0.0
    transform: Translate {
        id: bottomTranslate
        y: root.isVisible ? -20 : 0
        Behavior on y {
            NumberAnimation {
                duration: root.animationDuration * 1.5;
                easing.type: root.animationEasing
            }
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: root.animationDuration;
            easing.type: root.animationEasing
        }
    }

    // 어떤 패널을 열어야 하는지 부모에게 알리는 신호들
    signal requestColorPanel()
    signal requestAdvancedPanel()
    signal requestCameraPanel()
    signal requestClosePanel()

    Row {
        id: buttonRow
        anchors.centerIn: parent
        spacing: 30

        Button { 
            text: "닫기" 
            onClicked: root.requestClosePanel()
        }
        Button { 
            text: "색상 설정" 
            onClicked: root.requestColorPanel() 
        }
        Button { 
            text: "고급 설정" 
            onClicked: root.requestAdvancedPanel() 
        }
        Button { 
            text: "카메라" 
            onClicked: root.requestCameraPanel() 
        }
    }
}