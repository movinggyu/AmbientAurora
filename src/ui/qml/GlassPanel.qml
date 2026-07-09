import QtQuick

Rectangle {
    radius: 20
    color: Qt.rgba(1, 1, 1, 0.15) 
    border.color: Qt.rgba(1, 1, 1, 0.4)
    border.width: 1
    
    // 이 패널 안에 다른 컴포넌트들을 넣을 수 있도록 설정
    default property alias content: container.data 
    
    Item { 
        id: container
        anchors.fill: parent
        anchors.margins: 20 
    }
}