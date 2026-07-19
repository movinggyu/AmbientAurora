#ifndef AMBIENT_AURORA_APPLICATION_H
#define AMBIENT_AURORA_APPLICATION_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <memory>
#include "../lighting/LightingManager.h"
#include "../ui/AuroraItem.h"
#include "Types.h"

namespace AmbientAurora {

class Application : public QObject {
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    ~Application() override;

    // QML 엔진에서 생성된 AuroraItem을 연결하는 함수
    void setAuroraItem(AuroraItem* item);
    
    // 타이머 및 로직 구동 시작
    void start();

    // QML에서 호출할 수 있는 UI 제어 메서드
    Q_INVOKABLE void setLightingMode(int modeIndex); // 0: Static, 1: Dynamic, 2: Aurora
    Q_INVOKABLE void setTransitionMode(int transitionMode);
    Q_INVOKABLE void applyColor(float l, float c, float h);
    Q_INVOKABLE void applyColor(float l, float c);
    Q_INVOKABLE void applyColorRange(float startHue, float endHue);
    Q_INVOKABLE void applySamplingRange(float range);
    Q_INVOKABLE void applyDeltaHue(float deltaHue);

private slots:
    void onTick();

private:
    AuroraItem* m_auroraItem;
    std::unique_ptr<LightingManager> m_lightingManager;
    QTimer* m_timer;
    QElapsedTimer m_elapsedTimer;
};

} // namespace AmbientAurora
#endif // AMBIENT_AURORA_APPLICATION_H