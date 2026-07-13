import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml을 가져오기 위한 경로

GlassPanel {
    id: root
    title: "Slider"

    // 외부에서 패널을 쉽게 제어할 수 있도록 프로퍼티 개방
    property alias value: slider.value
    property alias from: slider.from
    property alias to: slider.to
    
    // 슬라이딩 필 네비게이션 탭 바 (모드 설정: Static, Dynamic, Aurora)
    TabBar {
        id: tabBar
        Layout.fillWidth: true
        currentIndex: 0 // Default to the first tab

        TabButton {
            text: "Static"
        }
        TabButton {
            text: "Dynamic"
        }
        TabButton {
            text: "Aurora"
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