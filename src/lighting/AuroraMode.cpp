#include "AuroraMode.h"
#include <random>
#include <algorithm>

namespace AmbientAurora {

    AuroraMode::AuroraMode()
        : startHue(0.0f), // 기본 시작 색상 (빨간색)
          endHue(360.0f), // 기본 끝 색상 (빨간색)
          samplingRange(30.0f), // 기본 샘플링 범위
          m_transitionMode(AmbientAurora::TransitionMode::DISSOLVE) // 기본 트랜지션 모드
    {
        std::random_device rd;
        m_gen.seed(rd());

        // 초기 색상은 시작 Hue로 설정 (빨간색)
        m_currentColor = OKLCHColor(0.5f, 0.5f, startHue);
    }

    AuroraMode::AuroraMode(const float startHue, const float endHue, float samplingRange, const OKLCHColor& prevColor, const AmbientAurora::TransitionMode transitionMode)
        : startHue(startHue),
          endHue(endHue),
          samplingRange(samplingRange),
          m_currentColor(prevColor),
          m_transitionMode(transitionMode)
    {
        std::random_device rd;
        m_gen.seed(rd());

        // 현재 색상이 시작 Hue와 끝 Hue 사이에 있는지 확인
        if (!isHueInRange(m_currentColor.h)) {
            // 현재 색상이 범위 밖에 있다면, 시작 Hue로 재설정
            m_currentColor.h = startHue;
        }
    }

    OKLCHColor AuroraMode::getCurrentColor() const {
        return m_currentColor;
    }

    OKLCHColor AuroraMode::getAndSetNextColor() {
        updateColor();
        return m_currentColor;
    }

    bool AuroraMode::isHueInRange(float hue) const {
        if (startHue <= endHue) {
            return (hue >= startHue && hue <= endHue);
        } else { 
            return (hue >= startHue || hue <= endHue);
        }
    }

    void AuroraMode::setStartHue(float hue) {
        startHue = hue;
        // 현재 색상이 새로운 범위 밖에 있다면, 시작 Hue로 재설정
        if (!isHueInRange(m_currentColor.h)) {
            m_currentColor.h = startHue;
        }
    }

    void AuroraMode::setEndHue(float hue) {
        endHue = hue;
        // 현재 색상이 새로운 범위 밖에 있다면, 끝 Hue로 재설정
        if (!isHueInRange(m_currentColor.h)) {
            m_currentColor.h = endHue;
        }
    }

    void AuroraMode::setSamplingRange(float range) {
        samplingRange = range;
    }

    void AuroraMode::setTransitionMode(AmbientAurora::TransitionMode transitionMode) {
        m_transitionMode = transitionMode;
    }

    float AuroraMode::calculateNextColor() {
        std::uniform_real_distribution<float> dist(m_currentColor.h - samplingRange, m_currentColor.h + samplingRange);
        float nextHue = dist(m_gen);

        // Hue가 0~360 범위를 벗어나면 wrap-around 처리
        if (nextHue < 0.0f) {
            nextHue += 360.0f;
        } else if (nextHue > 360.0f) {
            nextHue -= 360.0f;
        }

        // 색상범위를 벗어나지 않도록 클램핑
        if (!isHueInRange(nextHue)) {
            float distToStart = std::abs(nextHue - startHue);
            float distToEnd = std::abs(nextHue - endHue);
            nextHue = (distToStart < distToEnd) ? startHue : endHue;
        }
        return nextHue;
    }

    void AuroraMode::updateColor() {
        m_currentColor.h = calculateNextColor();
    }

} // namespace AmbientAurora
