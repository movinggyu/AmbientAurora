#ifndef AMBIENT_AURORA_OPENGLWIDGET_H
#define AMBIENT_AURORA_OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <memory>

// 기존 렌더러 헤더
#include "../render/ScreenRenderer.h"
#include "../core/Types.h"
#include "../core/Color.h"

namespace AmbientAurora {

class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget() override;

protected:
    // QOpenGLWidget의 필수 오버라이드 함수들
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QTimer *renderTimer; // 화면을 주기적으로 갱신하기 위한 타이머
    std::unique_ptr<ScreenRenderer> renderer;

    QElapsedTimer elapsedTimer; // 프레임 시간 측정을 위한 타이머
    float currentProgress;
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_OPENGLWIDGET_H