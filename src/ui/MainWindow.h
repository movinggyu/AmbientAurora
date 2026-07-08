#ifndef AMBIENT_AURORA_MAINWINDOW_H
#define AMBIENT_AURORA_MAINWINDOW_H

#include <QMainWindow>
#include <QQuickWidget>

namespace AmbientAurora {

// 전방 선언 (헤더 포함 의존성을 줄이기 위함)
class OpenGLWidget; 

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void resizeEvent(QResizeEvent *event) override;
    OpenGLWidget *glWidget; // 배경으로 쓰일 렌더링 위젯 포인터
    QQuickWidget *qmlWidget; // QML UI를 담을 위젯 포인터
};

} // namespace AmbientAurora

#endif // AMBIENT_AURORA_MAINWINDOW_H