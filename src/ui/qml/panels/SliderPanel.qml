import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml을 가져오기 위한 경로

GlassPanel {
    id: root
    
    // 외부에서 패널을 쉽게 제어할 수 있도록 프로퍼티 개방
    property alias title: titleLabel.text
    property alias value: slider.value
    property alias from: slider.from
    property alias to: slider.to
    
    // 헤더(제목, 닫기버튼) 행
    RowLayout {
        id: headerRow
        Layout.fillWidth: true
        spacing: 10

        Text {
            id: titleLabel
            text: "Slider Value"
            color: "white" 
            font.bold: true
            font.pixelSize: 16
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
        }

        Button {
            text: "닫기"
            onClicked: root.requestClosePanel() // 닫기 버튼 클릭 시 패널 닫기 요청 신호 발생
        }
    }

    // 슬라이더 컴포넌트 1
    Slider {
        id: slider
        from: 0
        to: 100
        value: 50
        Layout.fillWidth: true
    }

    // 슬라이더 컴포넌트 2
    Slider {
        id: slider1
        from: 0
        to: 100
        value: 50
        Layout.fillWidth: true
    }

    // 슬라이더 컴포넌트 3
    Slider {
        id: slider2
        from: 0
        to: 100
        value: 50
        Layout.fillWidth: true
    }

    // 현재 값 표시 라벨
    Text {
        text: Math.round(slider.value)
        color: "white"
        font.pixelSize: 14
        Layout.alignment: Qt.AlignHCenter
    }
}