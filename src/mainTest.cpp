#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // 컨텍스트 속성 주입을 위해 추가
#include <QQuickWindow>
#include "ui/AuroraItem.h"
#include "core/Application.h" // Application 헤더 추가

using namespace AmbientAurora;

int main(int argc, char *argv[]) {
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);

    qmlRegisterType<AuroraItem>("AmbientAurora", 1, 0, "AuroraItem");

    QQmlApplicationEngine engine;

    // 1. Application 인스턴스 생성 및 QML 컨텍스트에 'AppController'로 등록
    Application coreApp;
    engine.rootContext()->setContextProperty("AppController", &coreApp);

    const QUrl url(QStringLiteral("../src/ui/qml/MainUI.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    // 2. 생성된 UI 트리에서 AuroraItem 객체를 찾아 C++ 컨트롤러에 연결
    QObject *rootObject = engine.rootObjects().constFirst();
    if (rootObject) {
        // MainUI.qml에서 지정한 objectName "auroraBg"로 검색
        AuroraItem *auroraItem = rootObject->findChild<AuroraItem*>("auroraBg");
        if (auroraItem) {
            coreApp.setAuroraItem(auroraItem);
        }
    }

    // 3. 메인 타이머 및 로직 구동 시작
    coreApp.start();

    return app.exec();
}