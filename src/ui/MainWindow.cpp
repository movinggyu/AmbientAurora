#include "MainWindow.h"
#include "OpenGLWidget.h"
#include <QQuickWidget>
#include <QStackedLayout>

namespace AmbientAurora {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    resize(1280, 720);
    setWindowTitle("Ambient Aurora - Glassmorphism UI");

    // 1층: 오로라 배경 위젯 생성
    glWidget = new OpenGLWidget(this);

    // 2층: QQuickWidget 생성
    qmlWidget = new QQuickWidget(this);
    qmlWidget->setAttribute(Qt::WA_AlwaysStackOnTop); // OpenGL 위로 무조건 올리기
    qmlWidget->setClearColor(Qt::transparent);
    qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // QML 파일 경로 지정 (상황에 맞게 경로 수정 필요)
    qmlWidget->setSource(QUrl::fromLocalFile("../src/ui/TestPanel.qml"));

    if (qmlWidget->status() == QQuickWidget::Error) {
        for (const QQmlError &error : qmlWidget->errors()) {
            qDebug() << "QML 로드 에러:" << error.toString();
        }
    }

    qmlWidget->raise();

    // 3. 창 크기가 변할 때 두 위젯이 똑같이 겹쳐서 늘어나도록 메인 위젯 설정
    // 레이아웃 없이 가려면 이벤트 필터나 resizeEvent를 써도 되지만, 
    // 가장 편한 건 중앙 뼈대 위젯을 하나 두고 자식으로 겹쳐 배치하는 것입니다.
    glWidget->setGeometry(0, 0, width(), height());
    qmlWidget->setGeometry(0, 0, width(), height());
    
    // 창 크기 변경 대응을 위해 부모 관계 및 속성 정리
    glWidget->setParent(this);
    qmlWidget->setParent(this);
}

MainWindow::~MainWindow() {
    // QMainWindow가 소멸될 때 자식 위젯들도 자동으로 소멸되기 때문에 특별한 처리는 필요 없습니다.
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    
    if(glWidget) {
        glWidget->setGeometry(0, 0, width(), height());
    }
    if(qmlWidget) {
        qmlWidget->setGeometry(0, 0, width(), height());
        // QML 레이어를 항상 최상단으로 보장
        qmlWidget->raise(); 
    }
}

} // namespace AmbientAurora