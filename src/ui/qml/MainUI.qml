import QtQuick
import QtQuick.Controls
import "panels"

Item {
    id: root
    width: 1280
    height: 720

    property bool isHubVisible: false
    property bool sliderPanelOpen: false
    property bool cameraPanelOpen: false

    function openPanel(name) {
        if (name === "slider") sliderPanelOpen = true
        else if (name === "camera") cameraPanelOpen = true
    }
    function closePanel(name) {
        if (name === "slider") sliderPanelOpen = false
        else if (name === "camera") cameraPanelOpen = false
    }
    function closeAllPanels() {
        sliderPanelOpen = false
        cameraPanelOpen = false
    }

    // 1. 패널 영역: 모든 패널을 불러와놓고 보이기/숨기기만 처리
    SliderPanel {
        id: sliderPanel
        isOpen: root.sliderPanelOpen

        onRequestClosePanel: {
            root.closePanel("slider")
        }
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        opacity: root.isHubVisible ? 1.0 : 0.0
        transform: Translate {
            id: bottomTranslate
            y: root.isHubVisible ? -20 : bottomBar.height
            Behavior on y {
                NumberAnimation {
                    duration: root.animationDuration;
                    easing.type: root.animationEasing
                }
            }
        }

        Behavior on opacity {
            NumberAnimation {
                duration: root.animationDuration * 2;
                easing.type: root.animationEasing
            }
        }

        onRequestSliderPanel: {
            root.openPanel("slider")
        }
        onRequestCameraPanel: {
            root.openPanel("camera")
        }
        onRequestClosePanel: {
            root.closeAllPanels()
            root.isHubVisible = false
        }
    }

    // 3. 중앙 로고 (토글 버튼 역할)
    LogoButton {
        id: logoButton
        anchors.centerIn: parent
        enabled: !root.isHubVisible
        opacity: root.isHubVisible ? 0.0 : 1.0
        Behavior on opacity {
            NumberAnimation {
                duration: root.animationDuration;
                easing.type: root.animationEasing
            }
        }

        onToggled: {
            if (!root.isHubVisible) {
                root.isHubVisible = true
            }
        }
    }
}