// 열거형 트랜지션 모드
namespace AmbientAurora {
    enum class TransitionMode {
        DISSOLVE, // 디졸브 (다음 색상과 현재 색상을 혼합하여 전환)
        WIPE,     // 와이프 (다음 색상이 오른쪽에서 나타나며 현재 색상을 밀어내며 전환)
        CENTER    // 센터 (다음 색상이 중앙에서 시작하여 바깥으로 퍼지며 전환)
    };
}