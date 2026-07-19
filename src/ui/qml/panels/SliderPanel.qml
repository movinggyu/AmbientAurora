import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml

GlassPanel {
    id: root
    title: "Slider"

    // 1. 탭 바 (모드 선택)
    TabBar {
        id: tabBar
        Layout.fillWidth: true
        currentIndex: 0 
        
        onCurrentIndexChanged: {
            if (typeof AppController !== "undefined") {
                // qmllint disable
                AppController.setLightingMode(currentIndex)
            }
        }

        TabButton { text: "Static" }
        TabButton { text: "Dynamic" }
        TabButton { text: "Aurora" }
    }

    Text{ text: "Color Setting"; color: "white"; font.pixelSize: 14; font.bold: true}

    // 2. 스택 레이아웃 (탭에 따라 보여질 슬라이더 패널들)
    StackLayout {
        id: colorStackLayout
        Layout.fillWidth: true
        Layout.preferredHeight: children[currentIndex].implicitHeight
        // Aurora일 때는 Dynamic과 같은 슬라이더(start,end)를 갖는다. 아니면 원래 자기 인덱스를 사용
        currentIndex: tabBar.currentIndex === 2 ? 1 : tabBar.currentIndex

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
                            lightnessSlider.value,
                            chromaSlider.value,
                            staticHueSlider.value
                        )
                    }
                }
            }
        }

        // [인덱스 1] Dynamic 모드 패널
        ColumnLayout {
            id: dynamicPanel
            spacing: 5
            
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
            text: Math.floor(lightnessSlider.value * 100) + "%"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: lightnessSlider
        from: 0.0; to: 1.0; value: 0.5
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyColor(
                    lightnessSlider.value,
                    chromaSlider.value
                )
            }
        }
    }

    RowLayout{
        Layout.fillWidth: true
        Text { text: "Chroma"; color: "white"; font.pixelSize: 12}
        Item { Layout.fillWidth: true }
        Text {
            text: Math.floor(chromaSlider.value * 100) + "%"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: chromaSlider
        from: 0.0; to: 1.0; value: 0.5
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyColor(
                    lightnessSlider.value,
                    chromaSlider.value
                )
            }
        }
    }

    Text{ text: "Effect Setting"; color: "white"; font.pixelSize: 14; font.bold: true}

    StackLayout {
        id: effectStackLayout
        Layout.fillWidth: true
        Layout.preferredHeight: children[currentIndex].implicitHeight
        // Aurora일 때는 Dynamic과 같은 슬라이더(start,end)를 갖는다. 아니면 원래 자기 인덱스를 사용
        currentIndex: tabBar.currentIndex

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
                from: 10.0; to: 50.0; value: 10.0
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
                from: 5.0; to: 20.0; value: 5.0
                Layout.fillWidth: true
                onValueChanged: {
                    if (typeof AppController !== "undefined") {
                        AppController.applySamplingRange(samplingRangeSlider.value)
                    }
                }
            }
        }
    }
}