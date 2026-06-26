#include "ColorAnimator.h"
#include <algorithm>
#include <cmath>

namespace AmbientAurora {

    ColorAnimator::ColorAnimator()
        : m_state(AnimationState::IDLE),
          m_elapsedTime(0.0f),
          m_totalTime(0.0f),
          m_transitionDuration(0.0f),
          m_startColor(OKLCHColor(0.5f, 1.0f, 0.0f)), // 초기 색상은 흰색
          m_targetColor(OKLCHColor(0.5f, 1.0f, 0.0f)), // 초기 목표 색상은 흰색
          m_currentColor(OKLCHColor(0.5f, 1.0f, 0.0f)) // 초기 현재 색상은 흰색
    {
    }

    void ColorAnimator::startTransition(const OKLCHColor& targetColor, float totalTime, float transitionDuration) {
        m_totalTime = std::max(transitionDuration, totalTime);
        m_transitionDuration = transitionDuration;

        m_startColor = m_currentColor; // 현재 색상을 시작 색상으로 설정
        m_targetColor = targetColor;   // 목표 색상 설정
        m_elapsedTime = 0.0f;          // 경과 시간 초기화(변환 시작)

        if (m_transitionDuration > 0.0f) {
            m_state = AnimationState::TRANSITION; // 변환 상태로 전환
        } else if (m_totalTime > 0.0f) {
            m_currentColor = m_targetColor; // 변환 시간이 0이면 즉시 목표 색상으로 설정
            m_state = AnimationState::HOLD;  // 유지 상태로 전환
        } else {
            m_currentColor = m_targetColor; // 변환 시간과 유지 시간이 모두 0이면 즉시 목표 색상으로 설정
            m_state = AnimationState::IDLE;  // 대기 상태로 전환
        }
    }

    void ColorAnimator::update(float deltaTime) {
        if (m_state == AnimationState::IDLE) return; // 대기 상태이면 아무 작업도 하지 않음

        m_elapsedTime += deltaTime;

        // 애니메이션이 완전히 끝난 경우
        if (m_elapsedTime >= m_totalTime) {
            m_currentColor = m_targetColor; // 목표 색상으로 설정
            m_state = AnimationState::IDLE;  // 대기 상태로 전환
            return;
        }

        // 경과 시간에 따른 상태 분기 및 색상 처리
        if (m_elapsedTime < m_transitionDuration) { // 변하는 구간 (0 <= m_elapsedTime < m_transitionDuration)
            m_state = AnimationState::TRANSITION; 
            float progress = m_elapsedTime / m_transitionDuration; 
            m_currentColor = interpolateColor(m_startColor, m_targetColor, progress); 
        } else { // 유지하는 구간 (m_elapsedTime >= m_transitionDuration)
            m_state = AnimationState::HOLD; 
            m_currentColor = m_targetColor; 
        }
    }

    RGBColor ColorAnimator::getCurrentColor() const {
        return OKLCHToRGB(m_currentColor); // 현재 색상을 RGB로 변환하여 반환
    }

    OKLCHColor ColorAnimator::interpolateColor(const OKLCHColor& start, const OKLCHColor& target, float progress) {
        // 선형 보간을 사용하여 색상 계산
        float l = start.l + (target.l - start.l) * progress;
        float c = start.c + (target.c - start.c) * progress;

        // Hue는 원형 보간을 사용하여 계산
        float hDiff = target.h - start.h;
        if (hDiff > 180.0f) {
            hDiff -= 360.0f; // 시계 반대 방향으로 회전
        } else if (hDiff < -180.0f) {
            hDiff += 360.0f; // 시계 방향으로 회전
        }
        float h = start.h + hDiff * progress;

        // Hue를 0~360 범위로 조정
        if (h < 0.0f) h += 360.0f;
        if (h >= 360.0f) h -= 360.0f;

        return OKLCHColor(l, c, h);
    }
} // namespace AmbientAurora