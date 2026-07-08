#include <QApplication>
#include <QQuickWindow>
#include "ui/MainWindow.h"

using namespace AmbientAurora;

int main(int argc, char *argv[]) {
    // QML에서 OpenGL을 사용하도록 설정
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL); // QML에서 OpenGL 사용 설정

    // Qt 애플리케이션 객체 생성
    QApplication app(argc, argv);

    // 메인 윈도우 생성 및 표시
    MainWindow mainWindow;
    mainWindow.show();

    // Qt의 이벤트 루프 시작 (타이머, 입력 등의 이벤트 처리)
    return app.exec();
}