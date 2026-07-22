#include "DynamicMode.h"
#include <algorithm>
#include <iostream>
#include <cmath>

namespace AmbientAurora {
    // 기본 생성자에서 시작 색상은 0도 빨간색, 끝 색상은 360도 빨간색으로 설정
    DynamicMode::DynamicMode()
        : m_startHue(0.0f), // 기본 시작 색상 (빨간색)
          m_endHue(360.0f), // 기본 끝 색상 (빨간색)
          m_deltaHue(1.0f), // 기본 변화량
          m_transitionMode(AmbientAurora::TransitionMode::DISSOLVE) // 기본 트랜지션 모드
    {
        m_currentColor = HSVColor(m_startHue, 0.9f, 0.8f); // 초기 현재 색상 설정
    }

    // 생성자에서 시작 색상, 끝 색상, 변화량, 트랜지션 모드를 설정
    DynamicMode::DynamicMode(const float startHue, const float endHue, float deltaHue,  const HSVColor& prevColor, const AmbientAurora::TransitionMode transitionMode)
        : m_startHue(startHue),
          m_endHue(endHue),
          m_deltaHue(deltaHue),
          m_transitionMode(transitionMode)
    {
        m_currentColor = HSVColor(m_startHue, prevColor.s, prevColor.v); // 초기 현재 색상 설정
    }

    HSVColor DynamicMode::getCurrentColor() const {
        return m_currentColor;
    }

    HSVColor DynamicMode::getAndSetNextColor() {
        m_currentColor.h = calculateNextHue();
        return m_currentColor;
    }

    bool DynamicMode::isHueInRange(float hue) const {
        // 현재 hue가 시작색상 ~ 끝색상 범위 내에 있는지 확인
        if (m_startHue <= m_endHue) {
            return (hue >= m_startHue && hue <= m_endHue);
        } else { // 시작색상이 끝색상보다 클 경우 (예: 350도 ~ 10도)
            return (hue >= m_startHue || hue <= m_endHue);
        }
    }

    void DynamicMode::setStartHue(float hue) {
        m_startHue = hue;
        // 현재 hue가 시작색상 ~ 끝색상 범위의 바깥일때만 현재 색상을 시작색상으로 초기화
        if (!isHueInRange(m_currentColor.h)) {
            m_currentColor.h = m_startHue;
        }
    }

    void DynamicMode::setEndHue(float hue) {
        m_endHue = hue;
        // 현재 hue가 시작색상 ~ 끝색상 범위의 바깥일때만 현재 색상을 끝색상으로 초기화
        if (!isHueInRange(m_currentColor.h)) {
            m_currentColor.h = m_endHue;
        }
    }

    void DynamicMode::setTransitionMode(AmbientAurora::TransitionMode transitionMode) {
        m_transitionMode = transitionMode;
    }

    void DynamicMode::setDeltaHue(float deltaHue) {
        m_deltaHue = deltaHue;
    }

    float DynamicMode::calculateNextHue() {// 총 이동해야 할 Hue의 거리 계산 (360도 순환 고려)
        float totalRange = (m_endHue >= m_startHue) ? (m_endHue - m_startHue) : (360.0f - m_startHue + m_endHue);
        
        // 시작 색상이 끝 색상과 같다면 변화 없음
        if (totalRange <= 0.0f) return m_startHue;

        // 현재 시작점으로부터 누적 진행량 계산
        float currentProgress = (m_currentColor.h >= m_startHue) ? (m_currentColor.h - m_startHue) : (360.0f - m_startHue + m_currentColor.h);
        
        // 다음 진행량 계산
        float nextProgress = currentProgress + m_deltaHue;
        
        // 양수/음수 deltaHue를 모두 지원하는 양방향 안전 래핑 처리
        if (nextProgress > totalRange) {
            nextProgress = std::fmod(nextProgress, totalRange);
        } else if (nextProgress < 0.0f) {
            nextProgress = totalRange + std::fmod(nextProgress, totalRange);
            if (nextProgress >= totalRange) nextProgress = 0.0f; // 부동소수점 오차 방지
        }
        
        // 실제 사용할 Hue 값으로 복원 및 360도 범위 제한 구하기
        float nextHue = m_startHue + nextProgress;
        if (nextHue >= 360.0f) {
            nextHue -= 360.0f;
        } else if (nextHue < 0.0f) {
            nextHue += 360.0f;
        }
        
        return nextHue;
    }
} // namespace AmbientAurora



// 아래는 모든 메서드들이 잘 작동하는지 테스트하는 코드입니다. 실제 프로젝트에서는 이 코드를 제거하거나 주석 처리해야 합니다.
/*
int main() {
    // 앞서 안내해 드린 안전한 채도(0.15f)로 세팅하면 색상이 더 다채롭게 보입니다.
    AmbientAurora::DynamicMode dynamicMode(
        0.0f, 360.0f, 10.0f, 
        AmbientAurora::OKLCHColor(0.5f, 0.1f, 0.0f), 
        AmbientAurora::TransitionMode::DISSOLVE
    );

    for (int i = 0; i < 40; ++i) {
        AmbientAurora::RGBColor currentColor = dynamicMode.getCurrentColor();
        
        int r = static_cast<int>(currentColor.r);
        int g = static_cast<int>(currentColor.g);
        int b = static_cast<int>(currentColor.b);

        // 1. [트릭] ANSI True Color 코드를 이용해 터미널 배경색을 RGB 값으로 지정하고, 공백 4칸으로 색상 블록을 그립니다.
        // \033[48;2;R;G;Bm -> 배경색 지정 / \033[0m -> 색상 초기화
        std::cout << "Color Block: \033[48;2;" << r << ";" << g << ";" << b << "m    \033[0m";

        // 2. 옆에 수치 데이터도 함께 출력해 줍니다.
        std::cout << "  ->  R=" << r << ", G=" << g << ", B=" << b << std::endl;
                  
        dynamicMode.getAndSetNextColor();
    }

    return 0;
}
*/
