// qmllint disable
import QtQuick
import QtQuick.Window
import AmbientAurora 1.0
import "panels"

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: "Ambient Aurora"

    property color glassColor: {
        // 1. 현재 배경의 RGB 값 추출 (0.0 ~ 1.0)
        var r = backgroundRenderer.currentColor.r
        var g = backgroundRenderer.currentColor.g
        var b = backgroundRenderer.currentColor.b
        var s = backgroundRenderer.currentColor.hsvSaturation

        // 2. 인간의 눈이 느끼는 실제 밝기인 '인체 인지 휘도' 계산
        var luminance = 0.2126 * r + 0.7152 * g + 0.0822 * b

        // 3. 휘도와 채도를 조합하여 최종 가중치 산출
        // 초록색처럼 밝고 눈부신 색상은 채도가 있어도 weight가 높아져 패널이 어두워집니다.
        var weight = luminance * (1.0 - (s * 0.4)) // 채도 보정을 살짝 주어 원색 보호

        // 4. 가중치에 따른 패널 투명도 및 색상 보간
        var panelR = 1.0 * (1.0 - weight) + 0.0 * weight
        var panelG = 1.0 * (1.0 - weight) + 0.0 * weight
        var panelB = 1.0 * (1.0 - weight) + 0.0 * weight
        var panelA = 0.15 * (1.0 - weight) + 0.45 * weight

        return Qt.rgba(panelR, panelG, panelB, panelA)
    }

    property bool isHubVisible: false
    property bool colorPanelOpen: false
    property bool advancedPanelOpen: false
    property bool cameraPanelOpen: false

    // 0. OpenGL 배경 영역
    AuroraItem {
        id: backgroundRenderer
        objectName: "auroraBg"
        anchors.fill: parent
    }


    // 1. 패널 영역: 모든 패널을 불러와놓고 보이기/숨기기만 처리
    ColorPanel {
        id: colorPanel
        isOpen: root.colorPanelOpen
        panelColor: root.glassColor
        glassDuration: advancedPanel.transitionTime

        onClosePanel: {
            root.colorPanelOpen = false
        }
    }

    AdvancedPanel {
        id: advancedPanel
        isOpen: root.advancedPanelOpen
        panelColor: root.glassColor
        glassDuration: advancedPanel.transitionTime

        onClosePanel: {
            root.advancedPanelOpen = false
        }
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        isVisible: root.isHubVisible
        panelColor: root.glassColor
        glassDuration: advancedPanel.transitionTime

        progressValue: backgroundRenderer.totalProgress
        currentColor: backgroundRenderer.currentColor

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