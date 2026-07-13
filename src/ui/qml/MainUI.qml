import QtQuick
import "panels"

Item {
    id: root
    width: 1280
    height: 720

    property bool isHubVisible: false
    property bool sliderPanelOpen: false
    property bool cameraPanelOpen: false

    // 1. 패널 영역: 모든 패널을 불러와놓고 보이기/숨기기만 처리
    SliderPanel {
        id: sliderPanel
        isOpen: root.sliderPanelOpen

        onClosePanel: {
            root.sliderPanelOpen = false
        }
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        isVisible: root.isHubVisible

        onRequestSliderPanel: {
            root.sliderPanelOpen = !root.sliderPanelOpen
        }
        onRequestCameraPanel: {
            root.cameraPanelOpen = !root.cameraPanelOpen
        }
        onRequestClosePanel: {
            root.sliderPanelOpen = false
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