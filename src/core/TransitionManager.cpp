#include "TransitionManager.h"
#include <algorithm>
#include <cmath>

namespace AmbientAurora {

    TransitionManager::TransitionManager()
        : m_state(AnimationState::IDLE),
          m_elapsedTime(0.0f),
          m_totalTime(0.0f),
          m_transitionDuration(0.0f)
    {
    }

    void TransitionManager::startTransition(float totalTime, float transitionDuration) {
        m_totalTime = std::max(transitionDuration, totalTime);
        m_transitionDuration = transitionDuration;

        m_elapsedTime = 0.0f;          // 경과 시간 초기화(변환 시작)

        if (m_transitionDuration > 0.0f) {
            m_state = AnimationState::TRANSITION; // 변환 상태로 전환
        } else if (m_totalTime > 0.0f) {
            m_state = AnimationState::HOLD;  // 유지 상태로 전환
        } else {
            m_state = AnimationState::IDLE;  // 대기 상태로 전환
        }
    }

    void TransitionManager::update(float deltaTime) {
        if (m_state == AnimationState::IDLE) return; // 대기 상태이면 아무 작업도 하지 않음

        m_elapsedTime += deltaTime;

        // 애니메이션이 완전히 끝난 경우
        if (m_elapsedTime >= m_totalTime) {
            m_state = AnimationState::IDLE;  // 대기 상태로 전환
            return;
        }

        // 경과 시간에 따른 상태 분기 및 색상 처리
        if (m_elapsedTime < m_transitionDuration) { // 변하는 구간 (0 <= m_elapsedTime < m_transitionDuration)
            m_state = AnimationState::TRANSITION; 
            float progress = m_elapsedTime / m_transitionDuration; 
        } else { // 유지하는 구간 (m_elapsedTime >= m_transitionDuration)
            m_state = AnimationState::HOLD; 
        }
    }

} // namespace AmbientAurora
