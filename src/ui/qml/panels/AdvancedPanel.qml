// qmllint disable
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml

GlassPanel {
    id: root
    title: "Advanced Setting"

    RowLayout{
        Layout.fillWidth: true
        Text { text: "Gradient Feather"; color: "white"; font.pixelSize: 12}
        Item { Layout.fillWidth: true }
        Text {
            text: Math.floor(gradientFeatherSlider.value * 100) + "%"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: gradientFeatherSlider
        from: 0.0; to: 1.0; value: 0.5
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyGradientFeather(gradientFeatherSlider.value)
            }
        }
    }

    RowLayout{
        Layout.fillWidth: true
        Text { text: "Gradient Intensity"; color: "white"; font.pixelSize: 12}
        Item { Layout.fillWidth: true }
        Text {
            text: Math.floor(hueOffsetSlider.value * 100) + "%"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: hueOffsetSlider
        from: 0.0; to: 1.0; value: 0.5
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyHueOffset(hueOffsetSlider.value)
            }
        }
    }
}