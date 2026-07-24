// qmllint disable
import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    id: root
    implicitWidth: Math.max(10, buttonRow.implicitWidth + 80) // 최소 너비 10, 내부 콘텐츠에 따라 조정
    implicitHeight: Math.max(10, buttonRow.implicitHeight + 40) // 최소 높이 10, 내부 콘텐츠에 따라 조정
    radius: 100

    property color panelColor: Qt.rgba(1, 1, 1, 0.15)
    Behavior on color {
        ColorAnimation {
            duration: root.glassDuration
            easing.type: Easing.InOutQuad
        }
    }

    color: root.panelColor
    border.color: Qt.rgba(1, 1, 1, 0.4)
    border.width: 1
    
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom

    property bool isVisible: false
    property int animationDuration: 300
    property var animationEasing: Easing.InOutQuad 
    property int glassDuration: 200

    property real progressValue: 0.5
    property color currentColor: "#ffffff"

    enabled: isVisible
    visible: opacity > 0
    opacity: isVisible ? 1.0 : 0.0
    transform: Translate {
        id: bottomTranslate
        y: root.isVisible ? -20 : 0
        Behavior on y {
            NumberAnimation {
                duration: root.animationDuration * 1.5;
                easing.type: root.animationEasing
            }
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: root.animationDuration;
            easing.type: root.animationEasing
        }
    }

    // 어떤 패널을 열어야 하는지 부모에게 알리는 신호들
    signal requestColorPanel()
    signal requestAdvancedPanel()
    signal requestCameraPanel()
    signal requestClosePanel()

    RowLayout {
        id: buttonRow
        anchors.centerIn: parent
        spacing: 30

        Item {
            id: colorIndicator
            width: 36
            height: 36
            implicitWidth: 36
            implicitHeight: 36
            Layout.alignment: Qt.AlignVCenter
            
            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.samples: 4 // 매끄러운 계단 현상 방지

                ShapePath {
                    strokeColor: Qt.rgba(1, 1, 1, 0.3) // 링 색상
                    strokeWidth: 3
                    fillColor: "transparent"
                    capStyle: ShapePath.RoundCap

                    PathAngleArc {
                        centerX: colorIndicator.width / 2
                        centerY: colorIndicator.height / 2
                        radiusX: (colorIndicator.width - 4) / 2
                        radiusY: (colorIndicator.height - 4) / 2
                        startAngle: -90 // 12시 방향부터 시작
                        sweepAngle: 360
                    }
                }
            }

            // 1. 바깥쪽 원형 프로그레스 링 (Outer Arc)
            Shape {
                anchors.fill: parent
                layer.enabled: true
                layer.samples: 4 // 매끄러운 계단 현상 방지

                ShapePath {
                    strokeColor: Qt.rgba(1, 1, 1, 0.8) // 링 색상
                    strokeWidth: 3
                    fillColor: "transparent"
                    capStyle: ShapePath.RoundCap

                    PathAngleArc {
                        centerX: colorIndicator.width / 2
                        centerY: colorIndicator.height / 2
                        radiusX: (colorIndicator.width - 4) / 2
                        radiusY: (colorIndicator.height - 4) / 2
                        startAngle: -90 // 12시 방향부터 시작
                        sweepAngle: 360 * Math.min(1.0, Math.max(0.0, root.progressValue))
                    }
                }
            }

            // 2. 안쪽 현재 색상 동그라미 (Inner Circle)
            Rectangle {
                width: 22
                height: 22
                radius: width / 2
                color: root.currentColor
                anchors.centerIn: parent

                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
            }
        }

        Button { 
            text: "닫기" 
            onClicked: root.requestClosePanel()
        }
        Button { 
            text: "색상 설정" 
            onClicked: root.requestColorPanel() 
        }
        Button { 
            text: "고급 설정" 
            onClicked: root.requestAdvancedPanel() 
        }
        Button { 
            text: "카메라" 
            onClicked: root.requestCameraPanel() 
        }
    }
}