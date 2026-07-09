import QtQuick
import QtQuick.Controls
import "../" // GlassPanel.qml을 가져오기 위한 경로

GlassPanel {
    width: 450
    height: 600

    Column {
        anchors.fill: parent
        spacing: 20

        Text {
            text: "SLIDER PANEL"
            color: "white"
            font.bold: true
            font.pixelSize: 18
        }

        Column {
            width: parent.width
            spacing: 5
            Text { text: "SLIDER 1"; color: "white"; font.pixelSize: 12 }
            Slider {
                width: parent.width
                from: 0
                to: 100
                value: 52
            }
        }
        
        // 여기에 추가적인 슬라이더나 토글들을 배치합니다.
    }
}