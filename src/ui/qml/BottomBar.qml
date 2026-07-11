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

    // 어떤 패널을 열어야 하는지 부모에게 알리는 신호들
    signal requestSliderPanel()
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
            text: "슬라이더" 
            onClicked: root.requestSliderPanel() 
        }
        Button { 
            text: "카메라" 
            onClicked: root.requestCameraPanel() 
        }
    }
}