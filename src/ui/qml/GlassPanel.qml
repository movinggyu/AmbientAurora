import QtQuick

Rectangle {
    radius: 20
    color: Qt.rgba(1, 1, 1, 0.15) 
    border.color: Qt.rgba(1, 1, 1, 0.4)
    border.width: 1
    
    implicitWidth: Math.max(200, container.childrenRect.width + 40) 
    implicitHeight: Math.max(150, container.childrenRect.height + 40) 

    default property alias content: container.data 
    
    Item { 
        id: container
        anchors.fill: parent
        anchors.margins: 20 
    }
}