import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 1280
    height: 720

    Rectangle {
        id: glassPanel
        width: 450
        height: 600
        anchors.centerIn: parent
        radius: 20
        color: Qt.rgba(1, 1, 1, 0.15) 
        border.color: Qt.rgba(1, 1, 1, 0.4)
        border.width: 1

        Column {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 20

            Text {
                text: "MODE"
                color: "white"
                font.bold: true
                font.pixelSize: 14
            }

            // 임시 탭 세그먼트
            Row {
                spacing: 5
                width: parent.width
                Button { text: "A" }
                Button { text: "B" }
                Button { text: "C" }
            }

            // 슬라이더 구역
            Column {
                width: parent.width
                spacing: 5
                Text { text: "SLIDER"; color: "white"; font.pixelSize: 12 }
                Slider {
                    width: parent.width
                    from: 0
                    to: 100
                    value: 52
                }
            }
        }

        // Column 밖으로 빼내어 패널 바닥에 직접 고정
        Row {
            spacing: 15
            anchors.bottom: parent.bottom 
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            
            Button {
                text: "CANCEL"
            }
            Button {
                text: "CONFIRM"
            }
        }
    }
}