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

    // 핵심: GlassPanel의 기본 크기를 내부 ColumnLayout의 크기와 명시적으로 묶어줍니다!
    // 이렇게 하면 슬라이더가 추가되어 내부 높이가 늘어나는 만큼 패널 배경도 함께 늘어납니다.
    implicitWidth: Math.max(300, layout.implicitWidth + 40)
    implicitHeight: Math.max(150, layout.implicitHeight + 40)

    // GlassPanel의 content 속성 안으로 들어갈 레이아웃
    ColumnLayout {
        id: layout
        // 이제 패널 자체가 레이아웃 크기를 따라가게 설정되었으므로, 안심하고 부모(container)를 꽉 채우도록 설정해도 됩니다.
        anchors.fill: parent 
        spacing: 10

        // 제목 라벨
        Text {
            id: titleLabel
            text: "Slider Value"
            color: "white" 
            font.bold: true
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
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
}