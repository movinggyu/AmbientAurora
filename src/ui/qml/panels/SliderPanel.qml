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

    // 2. 스택 레이아웃 (탭에 따라 보여질 슬라이더 패널들)
    StackLayout {
        id: stackLayout
        Layout.fillWidth: true
        Layout.preferredHeight: children[currentIndex].implicitHeight
        currentIndex: tabBar.currentIndex 

        // [인덱스 0] Static 모드 패널
        ColumnLayout {
            id: staticPanel
            spacing: 5

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
            
            Text { text: "Hue Shift Speed"; color: "white"; font.pixelSize: 12 }
            Slider {
                id: dynSpeedSlider
                from: 0.1; to: 20; value: 1.0
                Layout.fillWidth: true
                onValueChanged: {
                    if (typeof AppController !== "undefined") {
                        // 예: AppController.setDynamicSpeed(value)
                    }
                }
            }
            Text { 
                text: dynSpeedSlider.value.toFixed(1)
                color: "white"
                Layout.alignment: Qt.AlignHCenter 
            }
        }

        // [인덱스 2] Aurora 모드 패널
        ColumnLayout {
            id: auroraPanel
            spacing: 5
            
            Text { text: "Sampling Range"; color: "white"; font.pixelSize: 12 }
            Slider {
                id: auroraRangeSlider
                from: 10; to: 180; value: 30
                Layout.fillWidth: true
                onValueChanged: {
                    if (typeof AppController !== "undefined") {
                        // 예: AppController.setAuroraSamplingRange(value)
                    }
                }
            }
            Text { 
                text: Math.round(auroraRangeSlider.value)
                color: "white"
                Layout.alignment: Qt.AlignHCenter 
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
}