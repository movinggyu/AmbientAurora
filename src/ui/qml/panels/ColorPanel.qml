// qmllint disable
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml

GlassPanel {
    id: root
    title: "Color Setting"

    property int activeTab: 0

    // 1. 탭 바 (모드 선택)
    TabBar {
        id: tabBar
        Layout.fillWidth: true
        currentIndex: 0 
        
        onCurrentIndexChanged: {
            if (typeof AppController !== "undefined") {
                AppController.setLightingMode(currentIndex)
            }
            tabTransition.restart()
        }

        TabButton { text: "Static" }
        TabButton { text: "Dynamic" }
        TabButton { text: "Aurora" }
    }

    // 🌟 2. 순차 애니메이션 (Fade-Out -> 투명한 순간 상태 변경 -> Fade-In)
    SequentialAnimation {
        id: tabTransition
        // 1단계: 페이드 아웃
        NumberAnimation { target: contentArea; property: "opacity"; to: 0.0; duration: 200 }
        
        // 2단계: 대기
        ScriptAction {
            script: {
                root.activeTab = tabBar.currentIndex
            }
        }
        PauseAnimation { duration: 200 }

        // 3단계: 페이드 인
        NumberAnimation { target: contentArea; property: "opacity"; to: 1.0; duration: 200 }
    }

    ColumnLayout {
        id: contentArea
        spacing: 10

        // 2. 스택 레이아웃 (탭에 따라 보여질 슬라이더 패널들)
        StackLayout {
            id: colorStackLayout
            Layout.fillWidth: true
            Layout.preferredHeight: children[currentIndex].implicitHeight
            // Aurora일 때는 Dynamic과 같은 슬라이더(start,end)를 갖는다. 아니면 원래 자기 인덱스를 사용
            currentIndex: root.activeTab === 2 ? 1 : root.activeTab

            // [인덱스 0] Static 모드 패널
            ColumnLayout {
                id: staticPanel
                spacing: 10

                RowLayout{
                    Layout.fillWidth: true
                    Text { text: "Color"; color: "white"; font.pixelSize: 12}
                    Item { Layout.fillWidth: true }
                    Text {
                        text: Math.floor(staticHueSlider.value) + "˚"
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter 
                    }
                }
                
                Slider {
                    id: staticHueSlider
                    from: 0.0; to: 360.0; value: 0.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (typeof AppController !== "undefined") {
                            AppController.applyColor(
                                saturationSlider.value,
                                valueSlider.value,
                                staticHueSlider.value
                            )
                        }
                    }
                }
            }

            // [인덱스 1] Dynamic 모드 패널
            ColumnLayout {
                id: dynamicPanel
                spacing: 10
                
                RowLayout{
                    Layout.fillWidth: true
                    Text { text: "Start Color"; color: "white"; font.pixelSize: 12}
                    Item { Layout.fillWidth: true }
                    Text {
                        text: Math.floor(startHueSlider.value) + "˚"
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter 
                    }
                }
                
                Slider {
                    id: startHueSlider
                    from: 0.0; to: 360.0; value: 0.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (typeof AppController !== "undefined") {
                            AppController.applyColorRange(
                                startHueSlider.value,
                                endHueSlider.value
                            )
                        }
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Text { text: "End Color"; color: "white"; font.pixelSize: 12}
                    Item { Layout.fillWidth: true }
                    Text {
                        text: Math.floor(endHueSlider.value) + "˚"
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter 
                    }
                }
                
                Slider {
                    id: endHueSlider
                    from: 0.0; to: 360.0; value: 360.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (typeof AppController !== "undefined") {
                            AppController.applyColorRange(
                                startHueSlider.value,
                                endHueSlider.value
                            )
                        }
                    }
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Text { text: "Lightness"; color: "white"; font.pixelSize: 12}
            Item { Layout.fillWidth: true }
            Text {
                text: Math.floor(valueSlider.value * 100) + "%"
                color: "white"
                Layout.alignment: Qt.AlignHCenter 
            }
        }
        
        Slider {
            id: valueSlider
            from: 0.0; to: 1.0; value: 0.9
            Layout.fillWidth: true
            onValueChanged: {
                if (typeof AppController !== "undefined") {
                    AppController.applyColor(
                        saturationSlider.value,
                        valueSlider.value
                    )
                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Text { text: "Saturation"; color: "white"; font.pixelSize: 12}
            Item { Layout.fillWidth: true }
            Text {
                text: Math.floor(saturationSlider.value * 100) + "%"
                color: "white"
                Layout.alignment: Qt.AlignHCenter 
            }
        }
        
        Slider {
            id: saturationSlider
            from: 0.0; to: 1.0; value: 0.9
            Layout.fillWidth: true
            onValueChanged: {
                if (typeof AppController !== "undefined") {
                    AppController.applyColor(
                        saturationSlider.value,
                        valueSlider.value
                    )
                }
            }
        }
        StackLayout {
            id: effectStackLayout
            Layout.fillWidth: true
            Layout.preferredHeight: children[currentIndex].implicitHeight
            // Aurora일 때는 Dynamic과 같은 슬라이더(start,end)를 갖는다. 아니면 원래 자기 인덱스를 사용
            currentIndex: root.activeTab

            // [인덱스 0]
            ColumnLayout {
                
            }

            // [인덱스 1]
            ColumnLayout {
                spacing: 10
                
                RowLayout{
                    Layout.fillWidth: true
                    Text { text: "Color Variation"; color: "white"; font.pixelSize: 12}
                    Item { Layout.fillWidth: true }
                    Text {
                        text: Math.floor(deltaHueSlider.value)
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter 
                    }
                }
                
                Slider {
                    id: deltaHueSlider
                    from: 10.0; to: 100.0; value: 10.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (typeof AppController !== "undefined") {
                            AppController.applyDeltaHue(deltaHueSlider.value)
                        }
                    }
                }
            }

            // [인덱스 2]
            ColumnLayout {
                spacing: 10
                
                RowLayout{
                    Layout.fillWidth: true
                    Text { text: "Sampling Range"; color: "white"; font.pixelSize: 12}
                    Item { Layout.fillWidth: true }
                    Text {
                        text: Math.floor(samplingRangeSlider.value)
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter 
                    }
                }
                
                Slider {
                    id: samplingRangeSlider
                    from: 10.0; to: 100.0; value: 10.0
                    Layout.fillWidth: true
                    onValueChanged: {
                        if (typeof AppController !== "undefined") {
                            AppController.applySamplingRange(samplingRangeSlider.value)
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            if (typeof AppController !== "undefined") {
                // 1. Static 모드 색상 초기화 (Hue, Saturation, Value)
                AppController.applyColor(
                    saturationSlider.value,
                    valueSlider.value,
                    staticHueSlider.value
                )
                // 2. Dynamic/Aurora 모드 색상 범위 초기화
                AppController.applyColorRange(
                    startHueSlider.value,
                    endHueSlider.value
                )
                // 3. Dynamic deltaHue / Aurora samplingRange 초기화
                AppController.applyDeltaHue(deltaHueSlider.value)
                AppController.applySamplingRange(samplingRangeSlider.value)
            }
        }

    }
}