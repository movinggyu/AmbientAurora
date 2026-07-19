import QtQuick
import QtQuick.Window
// qmllint disable
import AmbientAurora 1.0
import "panels"

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: "Ambient Aurora"

    property bool isHubVisible: false
    property bool colorPanelOpen: false
    property bool advancedPanelOpen: false
    property bool cameraPanelOpen: false

    // 0. OpenGL 배경 영역
    // qmllint disable
    AuroraItem {
        id: backgroundRenderer
        objectName: "auroraBg"
        anchors.fill: parent
    }


    // 1. 패널 영역: 모든 패널을 불러와놓고 보이기/숨기기만 처리
    ColorPanel {
        id: colorPanel
        isOpen: root.colorPanelOpen

        onClosePanel: {
            root.colorPanelOpen = false
        }
    }

    AdvancedPanel {
        id: advancedPanel
        isOpen: root.advancedPanelOpen

        onClosePanel: {
            root.advancedPanelOpen = false
        }
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        isVisible: root.isHubVisible

        onRequestColorPanel: {
            root.colorPanelOpen = !root.colorPanelOpen
        }
        onRequestAdvancedPanel: {
            root.advancedPanelOpen = !root.advancedPanelOpen
        }
        onRequestCameraPanel: {
            root.cameraPanelOpen = !root.cameraPanelOpen
        }
        onRequestClosePanel: {
            root.colorPanelOpen = false
            root.cameraPanelOpen = false
            root.isHubVisible = false
        }
    }

    // 3. 중앙 로고 (토글 버튼 역할)
    LogoButton {
        id: logoButton
        isVisible: !root.isHubVisible

        onToggled: {
            if (!root.isHubVisible) {
                root.isHubVisible = true
            }
        }
    }
}