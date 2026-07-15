#ifndef AMBIENT_AURORA_AURORARENDERER_H
#define AMBIENT_AURORA_AURORARENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <memory>
#include "../render/ScreenRenderer.h"
#include "../core/Color.h"
#include "../core/Types.h"

namespace AmbientAurora {

class AuroraRenderer : public QQuickFramebufferObject::Renderer {
public:
    AuroraRenderer();
    ~AuroraRenderer() override;

    void render() override;
    void synchronize(QQuickFramebufferObject *item) override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

private:
    std::unique_ptr<ScreenRenderer> m_screenRenderer;
    bool m_initialized = false;

    // 메인 스레드(AuroraItem)에서 계산된 최종 결과를 안전하게 담아둘 구조체
    struct RenderState {
        float progress = 0.0f;
        TransitionMode mode = TransitionMode::CENTER;
        OKLCHColor oldColor;
        OKLCHColor newColor;
        bool isGradient = true;
        float gradientFeather = 0.5f;
    } m_renderState;
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_AURORARENDERER_H