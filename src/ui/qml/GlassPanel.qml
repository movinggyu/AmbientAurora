// qmllint disable
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    radius: 20
    color: Qt.rgba(1, 1, 1, 0.15) 
    border.color: Qt.rgba(1, 1, 1, 0.4)
    border.width: 1
    anchors.centerIn: parent
    
    implicitWidth: Math.max(300, layout.implicitWidth + 40) 
    implicitHeight: Math.max(150, layout.implicitHeight + 40)

    property string title: "None"
    property bool isOpen: false // 패널 열림 상태를 나타내는 프로퍼티
    property int animationDuration: 300
    property var animationEasing: Easing.InOutQuad

    signal closePanel()

    default property alias content: layout.data // GlassPanel의 content 속성을 ColumnLayout의 data로 연결
    
    enabled: isOpen
    visible: opacity > 0
    opacity: isOpen ? 1.0 : 0.0
    transform: Translate {
        id: sliderTranslate
        // 패널의 초기위치의 -20px에서 초기 위치로 이동하도록 설정
        y: root.isOpen ? -20 : 0
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

    ColumnLayout { 
        id: layout
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10
        
        // 헤더(제목, 닫기버튼) 행
        RowLayout {
            id: headerRow
            Layout.fillWidth: true

            Text {
                id: titleLabel
                text: root.title
                color: "white" 
                font.bold: true
                font.pixelSize: 16
                Layout.alignment: Qt.AlignVCenter
            }

            // 제목과 버튼 사이에 공간을 채워 양쪽으로 밀어내는 스페이서
            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "닫기"
                Layout.alignment: Qt.AlignVCenter
                onClicked: root.closePanel()
            }
        }
    }
}