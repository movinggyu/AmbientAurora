import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 1280
    height: 720

    property bool isHubVisible: false // 하단 바 표시 상태
    property bool isPanelVisible: false
    property url currentPanelSource: ""

    // 1. 패널을 띄우는 로더 (동적 생성)
    Loader {
        id: panelLoader
        anchors.centerIn: parent
        source: currentPanelSource
        visible: currentPanelSource !== ""

        // 패널이 부드럽게 나타나고 사라지는 애니메이션
        Behavior on opacity { NumberAnimation { duration: 200 } }
        opacity: isPanelVisible ? 1.0 : 0.0
    }

    Timer {
        id: panelHideTimer
        interval: 220
        repeat: false
        onTriggered: currentPanelSource = ""
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        anchors.horizontalCenter: parent.horizontalCenter
        
        // 상태(isHubVisible)에 따라 위아래로 움직임
        // 1. 부모의 하단 가장자리에 이 요소를 실시간으로 강력하게 고정합니다.
        anchors.bottom: parent.bottom

        // 2. isHubVisible 상태에 따라 하단 여백(Margin)만 조절하여 요소를 올리거나 숨깁니다.
        anchors.bottomMargin: root.isHubVisible ? 20 : -(height + 20)
        
        Behavior on anchors.bottomMargin {
            NumberAnimation {
                duration: 500;
                easing.type: Easing.InOutQuad
            }
        }

        // BottomBar에서 신호가 오면 Loader의 소스 파일을 교체하여 패널을 염
        onRequestSliderPanel: {
            currentPanelSource = "panels/SliderPanel.qml"
            panelHideTimer.stop()
            isPanelVisible = true
        }
        onRequestClosePanel: {
            root.isHubVisible = false
            isPanelVisible = false
            panelHideTimer.restart()
        }
    }

    // 3. 중앙 로고 (토글 버튼 역할)
    LogoButton {
        id: logoButton
        anchors.centerIn: parent
        enabled: !root.isHubVisible
        opacity: root.isHubVisible ? 0.0 : 1.0
        Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }

        onToggled: {
            if (!root.isHubVisible) {
                root.isHubVisible = true
            }
        }
    }
}