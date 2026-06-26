#ifndef AMBIENT_AURORA_TYPES_H
#define AMBIENT_AURORA_TYPES_H

namespace AmbientAurora {

// 열거형 트랜지션 모드
enum class TransitionMode {
    DISSOLVE, // 디졸브 (다음 색상과 현재 색상을 혼합하여 전환)
    WIPE,     // 와이프 (다음 색상이 오른쪽에서 나타나며 현재 색상을 밀어내며 전환)
    CENTER    // 센터 (다음 색상이 중앙에서 시작하여 바깥으로 퍼지며 전환)
};

// 열거형 애니메이션 상태
enum class AnimationState {
    IDLE,        // 애니메이션이 없는 대기 상태
    TRANSITION,  // 색상이 스으윽 변하는 구간
    HOLD         // 목표 색상을 유지하는 구간
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_TYPES_H