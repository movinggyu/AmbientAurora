#include "../render/glad/glad.h" 
#include "OpenGLWidget.h"
#include <QOpenGLContext>
#include <iostream>

void* qt_get_proc_address(const char* procName) {
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (!context) {
        std::cerr << "qt_get_proc_address: no current OpenGL context" << std::endl;
        return nullptr;
    }
    return reinterpret_cast<void*>(context->getProcAddress(procName));
}

namespace AmbientAurora {

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), renderer(nullptr) 
    , currentProgress(0.0f) {
    
    // 60FPS 애니메이션을 목표로 약 16ms마다 화면을 갱신(update)하도록 타이머 설정
    renderTimer = new QTimer(this);
    connect(renderTimer, &QTimer::timeout, this, QOverload<>::of(&QWidget::update));
    renderTimer->start(16);
}

OpenGLWidget::~OpenGLWidget() {
    // OpenGL 리소스를 안전하게 해제하기 위해 컨텍스트를 활성화(makeCurrent)한 후 소멸시킵니다.
    makeCurrent();
    renderer.reset(); 
    doneCurrent();
}

void OpenGLWidget::initializeGL() {
    // 1. Qt의 현재 OpenGL 컨텍스트를 이용해 GLAD 초기화
    if (!gladLoadGLLoader((GLADloadproc)qt_get_proc_address)) {
        std::cerr << "GLAD 초기화 실패!" << std::endl;
        return;
    }

    // 2. 기존 ScreenRenderer 객체 생성 및 초기화 로직 실행
    renderer = std::make_unique<ScreenRenderer>();
    
    // 위젯의 현재 너비와 높이를 넘겨 초기화합니다.
    if (!renderer->initialize(width(), height())) {
        std::cerr << "ScreenRenderer 초기화 실패!" << std::endl;
    }

    // 3. 렌더링 프레임 간 시간 계산을 위한 타이머 시작
    elapsedTimer.start();
}

void OpenGLWidget::resizeGL(int w, int h) {
    // 창 크기가 변경될 때 뷰포트 업데이트 (w, h는 변경된 창의 너비와 높이)
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL() {
    // 시간에 따라 progress 값을 0.0 ~ 1.0 사이로 부드럽게 왕복(Ping-Pong)하도록 계산
    float deltaTime = elapsedTimer.restart() / 1000.0f; // 초 단위 경과 시간
    float speed = 0.5f; // 전환 속도 (값이 커지면 빨라짐)


    // 화면 렌더링을 위한 파라미터 세팅 (UI와 연동 전 테스트용 하드코딩 값)
    // 참고: Types.h에 선언된 enum과 Color 구조체의 실제 정의에 따라 캐스팅이 필요할 수 있습니다.
    TransitionMode mode = static_cast<TransitionMode>(2); // 2 = CENTER 전환 효과 모드[cite: 8]
    static RGBColor oldColor = {1.0f, 0.1f, 0.2f}; // 다홍색 느낌
    static RGBColor newColor = {0.1f, 0.5f, 1.0f}; // 푸른색 느낌
    bool isGradient = true;
    float gradientFeather = 0.5f;
    
    currentProgress += deltaTime * speed;
    if (currentProgress > 1.0f) {
        currentProgress = 0.0f;
        RGBColor tempColor = oldColor;
        oldColor = newColor; // 이전 색상을 새 색상으로 업데이트
        newColor = tempColor; // 새 색상을 이전 색상으로 업데이트
    }

    // std::cout << "Rendering Frame: Mode=" << static_cast<int>(mode)
    //           << ", Progress=" << currentProgress
    //           << ", OldColor=(" << oldColor.r << "," << oldColor.g << "," << oldColor.b << ")"
    //           << ", NewColor=(" << newColor.r << "," << newColor.g << "," << newColor.b << ")"
    //           << ", IsGradient=" << isGradient
    //           << ", GradientFeather=" << gradientFeather
    //           << std::endl;

    // 3. 실제 그리기 함수 호출
    if (renderer) {
        renderer->renderFrame(
            mode, 
            currentProgress, 
            oldColor, 
            newColor, 
            isGradient, 
            gradientFeather
        );
    }
}

} // namespace AmbientAurora