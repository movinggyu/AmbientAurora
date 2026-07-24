// qmllint disable
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../" // GlassPanel.qml

GlassPanel {
    id: root
    title: "Advanced Setting"

    property alias totalTime: totalTimeSlider.value
    property alias transitionTime: transitionTimeSlider.value

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

    RowLayout{
        Layout.fillWidth: true
        Text { text: "Cycle Time"; color: "white"; font.pixelSize: 12}
        Item { Layout.fillWidth: true }
        Text {
            text: (totalTimeSlider.value / 1000).toFixed(1) + "s"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: totalTimeSlider
        from: 1000.0; to: 5000.0; value: 3000.0
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyUserTime(totalTimeSlider.value, transitionTimeSlider.value)
            }
        }
    }

    RowLayout{
        Layout.fillWidth: true
        Text { text: "Transition Time"; color: "white"; font.pixelSize: 12}
        Item { Layout.fillWidth: true }
        Text {
            text: (transitionTimeSlider.value / 1000).toFixed(1) + "s"
            color: "white"
            Layout.alignment: Qt.AlignHCenter 
        }
    }
    
    Slider {
        id: transitionTimeSlider
        from: 1000.0; to: totalTimeSlider.value; value: 2000.0
        Layout.fillWidth: true
        onValueChanged: {
            if (typeof AppController !== "undefined") {
                AppController.applyUserTime(totalTimeSlider.value, transitionTimeSlider.value)
            }
        }
    }

    Component.onCompleted: {
        if (typeof AppController !== "undefined") {
            AppController.applyGradientFeather(gradientFeatherSlider.value)
            AppController.applyHueOffset(hueOffsetSlider.value)
            AppController.applyUserTime(totalTimeSlider.value, transitionTimeSlider.value)
        }
    }
}