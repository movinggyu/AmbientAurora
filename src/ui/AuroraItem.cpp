#include "AuroraItem.h"
#include "AuroraRenderer.h"

namespace AmbientAurora {

AuroraItem::AuroraItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent) {
    // 초기 더미 색상 세팅 (초기 렌더링 시 검은 화면 방지)
    m_oldColor = OKLCHColor(0.5f, 0.5f, 0.0f);
    m_newColor = OKLCHColor(0.5f, 0.5f, 0.0f);
}

QQuickFramebufferObject::Renderer *AuroraItem::createRenderer() const {
    return new AuroraRenderer(); 
}

float AuroraItem::progress() const { return m_progress; }
TransitionMode AuroraItem::mode() const { return m_mode; }
OKLCHColor AuroraItem::oldColor() const { return m_oldColor; }
OKLCHColor AuroraItem::newColor() const { return m_newColor; }
bool AuroraItem::isGradient() const { return m_isGradient; }
float AuroraItem::gradientFeather() const { return m_gradientFeather; }
int AuroraItem::transitionMode() const { return static_cast<int>(m_mode); }

void AuroraItem::setProgress(float progress) {
    if (m_progress != progress) {
        m_progress = progress;
        emit progressChanged();
        update(); // 값이 변경되었을 때만 렌더링 스레드에 화면 갱신 요청
    }
}

void AuroraItem::setRenderColor(const OKLCHColor& newColor) {
    // 1. 방지턱
    if (m_oldColor.l == newColor.l && m_oldColor.c == newColor.c && m_oldColor.h == newColor.h) return;

    // 2. 그리기 요청
    m_oldColor = m_newColor;
    m_newColor = newColor;
    update();
}

void AuroraItem::setTransitionMode(int mode) {
    setMode(static_cast<TransitionMode>(mode));
}

void AuroraItem::setMode(TransitionMode mode) {
    if (m_mode != mode) {
        m_mode = mode;
        emit transitionModeChanged();
        update();
    }
}

void AuroraItem::setIsGradient(bool isGradient) {
    if (m_isGradient != isGradient) {
        m_isGradient = isGradient;
        emit isGradientChanged();
        update();
    }
}

void AuroraItem::setGradientFeather(float feather) {
    if (m_gradientFeather != feather) {
        m_gradientFeather = feather;
        emit gradientFeatherChanged();
        update();
    }
}

} // namespace AmbientAurora