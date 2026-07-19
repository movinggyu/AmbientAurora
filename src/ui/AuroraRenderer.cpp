#include "../render/glad/glad.h"
#include "AuroraRenderer.h"
#include "AuroraItem.h"
#include <QOpenGLContext>
#include <QQuickWindow>
#include <iostream>

namespace AmbientAurora {

// GLAD 로딩을 위한 Qt 프로시저 주소 헬퍼
static void* qt_get_proc_address(const char* procName) {
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context) return nullptr;
    return reinterpret_cast<void*>(context->getProcAddress(procName));
}

AuroraRenderer::AuroraRenderer() 
    : m_screenRenderer(std::make_unique<ScreenRenderer>()) {
    // 생성자에서는 FBO 컨텍스트가 아직 활성화되지 않았으므로 초기화를 지연합니다.
}

AuroraRenderer::~AuroraRenderer() {
    // ScreenRenderer의 소멸자에서 OpenGL 자원을 안전하게 해제합니다.
}

void AuroraRenderer::synchronize(QQuickFramebufferObject *item) {
    AuroraItem *auroraItem = static_cast<AuroraItem *>(item);
    if (!auroraItem) return;

    // --- 핵심: 메인 스레드의 최종 계산 결과를 스케치북으로 단순 복사만 수행 ---
    m_renderState.progress = auroraItem->progress();
    m_renderState.mode = auroraItem->mode();
    m_renderState.oldColor = auroraItem->oldColor();
    m_renderState.newColor = auroraItem->newColor();
    m_renderState.isGradient = auroraItem->isGradient();
    m_renderState.gradientFeather = auroraItem->gradientFeather();
    m_renderState.hueOffset = auroraItem->hueOffset();
}

QOpenGLFramebufferObject *AuroraRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4); // MSAA 4x 설정 (그래픽 계단 현상 완화)
    
    if (!m_initialized) {
        // 렌더링 스레드의 OpenGL 컨텍스트가 바인딩된 시점이므로 GLAD 초기화가 안전합니다.
        if (!gladLoadGLLoader((GLADloadproc)qt_get_proc_address)) {
            std::cerr << "GLAD 초기화 실패!" << std::endl;
        }
        m_screenRenderer->initialize(size.width(), size.height());
        m_initialized = true;
    } else {
        m_screenRenderer->resize(size.width(), size.height());
    }

    return new QOpenGLFramebufferObject(size, format);
}

void AuroraRenderer::render() {
    if (!m_initialized) return;

    // --- 핵심: 일체의 연산 로직(시간 계산, 색상 보간) 없이 그리기 명령만 수행 ---
    m_screenRenderer->renderFrame(
        m_renderState.mode, 
        m_renderState.progress, 
        m_renderState.oldColor, 
        m_renderState.newColor, 
        m_renderState.isGradient, 
        m_renderState.gradientFeather,
        m_renderState.hueOffset
    );
}

} // namespace AmbientAurora