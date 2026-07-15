#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include "ui/AuroraItem.h" // 새로 생성할 커스텀 아이템 헤더

using namespace AmbientAurora;

int main(int argc, char *argv[]) {
    // 성능 최적화를 위해 OpenGL 렌더러 명시적 지정
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    
    QGuiApplication app(argc, argv);

    // 1. 커스텀 C++ 타입(AuroraItem)을 QML 시스템에 등록
    // (모듈 이름: AmbientAurora, 버전 1.0, QML에서 사용할 이름: AuroraItem)
    qmlRegisterType<AuroraItem>("AmbientAurora", 1, 0, "AuroraItem");

    // 2. QML 엔진 설정 및 메인 UI 로드
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("../src/ui/qml/MainUI.qml")); // 리소스 파일(qrc) 사용 권장
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}