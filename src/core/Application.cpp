#include "Application.h"
#include "../lighting/StaticMode.h"
#include "../lighting/DynamicMode.h"
#include "../lighting/AuroraMode.h"

namespace AmbientAurora {

Application::Application(QObject *parent)
    : QObject(parent),
      m_auroraItem(nullptr),
      m_lightingManager(std::make_unique<LightingManager>()),
      m_timer(new QTimer(this))
{
    // 60FPS (약 16ms) 주기로 onTick 호출
    connect(m_timer, &QTimer::timeout, this, &Application::onTick);
}

Application::~Application() {
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void Application::setAuroraItem(AuroraItem* item) {
    m_auroraItem = item;
}

void Application::start() {
    // 앱 시작 시 기본 모드를 StaticMode로 설정
    m_lightingManager->changeMode(LightingModeType::Static);
    m_elapsedTimer.start();
    m_timer->start(16);
}

void Application::setLightingMode(int modeIndex) {
    switch (modeIndex) {
        case 0:
            m_lightingManager->changeMode(LightingModeType::Static);
            break;
        case 1:
            m_lightingManager->changeMode(LightingModeType::Dynamic);
            break;
        case 2:
            m_lightingManager->changeMode(LightingModeType::Aurora);
            break;
        default:
            break;
    }
}

void Application::setTransitionMode(int transitionMode) {
    if (m_auroraItem) {
        m_auroraItem->setTransitionMode(transitionMode);
    }
}

void Application::applyColor(float l, float c, float h){
    if (m_lightingManager) {
        m_lightingManager->updateActiveModeColor(l, c, h);
    }
}

void Application::applyColor(float l, float c){
    if (m_lightingManager) {
        m_lightingManager->updateActiveModeColor(l, c);
    }
}

void Application::applyColorRange(float startHue, float endHue){
    if (m_lightingManager) {
        m_lightingManager->updateActiveModeColorRange(startHue, endHue);
    }
}

void Application::applySamplingRange(float range){
    if (m_lightingManager) {
        m_lightingManager->updateActiveModeSamplingRange(range);
    }
}

void Application::applyDeltaHue(float deltaHue){
    if (m_lightingManager) {
        m_lightingManager->updateActiveModeDeltaHue(deltaHue);
    }
}

void Application::applyGradientFeather(float feather){
    if (m_auroraItem) {
        m_auroraItem->setGradientFeather(feather);
    }
}

void Application::applyHueOffset(float offset){
    if (m_auroraItem) {
        m_auroraItem->setHueOffset(offset);
    }
}

void Application::applyUserTime(float totalTime, float transitionTime){
    if (m_lightingManager) {
        m_lightingManager->updateUserTime(totalTime, transitionTime);
    }
}

void Application::onTick() {
    float deltaTime = static_cast<float>(m_elapsedTimer.restart());

    // 1. 내부 로직(조명 색상 및 애니메이션) 업데이트
    m_lightingManager->update(deltaTime);

    // 2. 계산된 결과를 AuroraItem(QML 객체)에 주입
    if (m_auroraItem && m_lightingManager) {
        m_auroraItem->setRenderColor(m_lightingManager->getCurrentColor());
        m_auroraItem->setProgress(m_lightingManager->getProgress());
    }
}

} // namespace AmbientAurora