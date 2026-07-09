import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 1280
    height: 720

    property bool isHubVisible: false // 하단 바 표시 상태

    // 1. 패널을 띄우는 로더 (동적 생성)
    Loader {
        id: panelLoader
        anchors.centerIn: parent
        // 하단 바가 숨겨지면 열려있던 패널도 닫히도록 설정
        active: root.isHubVisible 
        
        // 패널이 부드럽게 나타나고 사라지는 애니메이션
        Behavior on opacity { NumberAnimation { duration: 200 } }
        opacity: status === Loader.Ready ? 1.0 : 0.0
    }

    // 2. 하단 독(Bottom Bar)
    BottomBar {
        id: bottomBar
        anchors.horizontalCenter: parent.horizontalCenter
        
        // 상태(isHubVisible)에 따라 위아래로 움직임
        y: root.isHubVisible ? (root.height - height - 30) : (root.height + 50)
        
        Behavior on y {
            NumberAnimation { duration: 400; easing.type: Easing.OutBack }
        }

        // BottomBar에서 신호가 오면 Loader의 소스 파일을 교체하여 패널을 염
        onRequestSliderPanel: panelLoader.source = "panels/SliderPanel.qml"
        onRequestClosePanel: panelLoader.source = "" // 소스를 비우면 패널이 닫힘
    }

    // 3. 중앙 로고 (토글 버튼 역할)
    Rectangle {
        id: logoButton
        width: 80
        height: 80
        radius: 40
        anchors.centerIn: parent
        color: "white"

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                // 버튼을 누를 때마다 하단 바 상태 변경
                root.isHubVisible = !root.isHubVisible
                
                // 바가 닫힐 때는 로드된 패널도 초기화
                if(!root.isHubVisible) {
                    panelLoader.source = ""
                }
            }
        }
    }
}