#ifndef AMBIENT_AURORA_AURORAITEM_H
#define AMBIENT_AURORA_AURORAITEM_H

#include <QQuickFramebufferObject>
#include "../core/Color.h"
#include "../core/Types.h"

namespace AmbientAurora {

class AuroraItem : public QQuickFramebufferObject {
    Q_OBJECT
    // QML에서 제어할 수 있도록 기본 프로퍼티 노출 (QML UI 연동용)
    Q_PROPERTY(float progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(float totalProgress READ totalProgress WRITE setTotalProgress NOTIFY totalProgressChanged)
    Q_PROPERTY(int transitionMode READ transitionMode WRITE setTransitionMode NOTIFY transitionModeChanged)
    Q_PROPERTY(bool isGradient READ isGradient WRITE setIsGradient NOTIFY isGradientChanged)
    Q_PROPERTY(float gradientFeather READ gradientFeather WRITE setGradientFeather NOTIFY gradientFeatherChanged)

public:
    explicit AuroraItem(QQuickItem *parent = nullptr);

    Renderer *createRenderer() const override;

    // --- Getters (스케치북이 데이터를 복사해갈 때 사용) ---
    float progress() const;
    float totalProgress() const;
    TransitionMode mode() const;
    OKLCHColor oldColor() const;
    OKLCHColor newColor() const;
    bool isGradient() const;
    float gradientFeather() const;
    float hueOffset() const;
    int transitionMode() const;

    // --- Setters (LightingManager, ColorAnimator 등 비즈니스 로직에서 호출) ---
    void setProgress(float progress);
    void setTotalProgress(float totalProgress);
    void setRenderColor(const OKLCHColor& newColor);
    void setTransitionMode(int mode);
    void setMode(TransitionMode mode);
    void setIsGradient(bool isGradient);
    void setGradientFeather(float feather);
    void setHueOffset(float offset);
signals:
    void progressChanged();
    void totalProgressChanged();
    void transitionModeChanged();
    void isGradientChanged();
    void gradientFeatherChanged();
    void hueOffsetChanged();

private:
    // 렌더링에 필요한 상태 데이터만 저장 (계산 로직 없음)
    float m_progress = 0.0f;
    float m_totalProgress = 0.0f;
    TransitionMode m_mode = TransitionMode::DISSOLVE;
    OKLCHColor m_oldColor;
    OKLCHColor m_newColor;
    bool m_isGradient = true;
    float m_gradientFeather = 0.5f;
    float m_hueOffset = 0.5f;
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_AURORAITEM_H