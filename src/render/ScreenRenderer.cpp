#include <glad/glad.h>
#include "ScreenRenderer.h"
#include <iostream>
#include <vector>
#include <algorithm>

namespace AmbientAurora {

// OpenGL 셰이더 소스 코드 (버텍스 셰이더)
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform int u_TransitionMode; // 0: DISSOLVE, 1: WIPE, 2: CENTER etc.
uniform float u_Progress; // 0.0 ~ 1.0
uniform float u_AspectRatio; // 화면 비율 (width / height)

uniform vec3 u_OldColor; // 이전 색상
uniform vec3 u_NewColor; // 새 색상
uniform int u_Isgradient; // 0: 단색, 1: 그라데이션
uniform float u_GradientFeather; // 그라데이션 경계의 부드러움 정도 (0.0 ~ 1.0)
uniform float u_HueOffset; // 그라데이션 강도 (도 단위)

// HSV -> RGB 변환 함수
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx / 360.0 + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Hue 지름길 보간 함수
float mixHue(float h1, float h2, float t) {
    float diff = h2 - h1;
    
    // 각도 차이를 [-PI, PI] 범위로 변환하여 지름길 탐색
    diff = mod(diff + 180.0, 360.0) - 180.0;
    
    return mod(h1 + diff * t, 360.0);
}

void main()
{
    // 색상은 CPU에서 이미 HSV로 변환해 uniform으로 전달한다.
    vec3 oldHSV = u_OldColor;
    vec3 newHSV = u_NewColor;

    if (u_Isgradient == 1) {
        // 1. 중심(0.5, 0.5)으로부터의 절대 거리 계산
        vec2 centerDist = abs(TexCoord - vec2(0.5));
        
        float plusField = centerDist.x * 0.5 + centerDist.y * 0.7;
        float centerMask = smoothstep(0.0, u_GradientFeather, plusField);

        // 네 개의 모서리(Corner)별 고유한 Hue offset 가중치
        float tlOffset = u_HueOffset;          // Top-Left (왼쪽 위)
        float trOffset = u_HueOffset;          // Top-Right (오른쪽 위)
        float brOffset = -u_HueOffset * 0.5;   // Bottom-Right (오른쪽 아래)
        float blOffset = -u_HueOffset * 0.5;   // Bottom-Left (왼쪽 아래)

        // 바이리니어 보간을 통해 네 모서리의 색상을 부드럽게 분배
        float topCornerHue = mix(tlOffset, trOffset, TexCoord.x);
        float bottomCornerHue = mix(blOffset, brOffset, TexCoord.x);
        float finalCornerOffset = mix(bottomCornerHue, topCornerHue, TexCoord.y);

        // (TexCoord.y - 0.5) * 2.0 은 위아래로 갈수록 -1.0 ~ 1.0이 되는 세로 축입니다.
        float verticalGrad = (TexCoord.y - 0.5) * 2.0 * u_HueOffset;
        
        // 기존 모서리 색상(50%)과 세로 그라데이션(50%)을 반반씩 섞어 아치형 느낌을 냅니다.
        float combinedOffset = mix(finalCornerOffset, verticalGrad, 0.5); // 마지막 인자가 클수록 세로 그라데이션이 강해집니다.

        // 최종 HSV의 Hue(x)값 적용
        oldHSV.x = mod(oldHSV.x + mix(0.0, combinedOffset, centerMask), 360.0);
        newHSV.x = mod(newHSV.x + mix(0.0, combinedOffset, centerMask), 360.0);
    }

    float mask = 0.0; // 마스크 값 초기화
    float edgeSmooth = 1.0; // 가장자리 부드럽게 처리 (0.0 ~ 1.0)
    
    // 전환 모드에 따라 색상 혼합 처리 (DISSOLVE, WIPE, CENTER 등)
    if (u_TransitionMode == 0) { // DISSOLVE
        mask = u_Progress;
    }
    else if (u_TransitionMode == 1) { // WIPE
        float extendProgress = u_Progress * (1.0 + edgeSmooth * 2.0) - edgeSmooth; // 가장자리 부드럽게 처리
        mask = smoothstep(extendProgress - edgeSmooth, extendProgress + edgeSmooth, TexCoord.x);
    }
    else if (u_TransitionMode == 2) { // CENTER
        float centerSmooth = edgeSmooth * 2.0; // 화면 비율에 따라 가장자리 부드럽게 처리

        vec2 correctedTexCoord = vec2(TexCoord.x * u_AspectRatio, TexCoord.y);
        vec2 correctedCenter = vec2(0.5 * u_AspectRatio, 0.5);

        float distToCenter = length(correctedTexCoord - correctedCenter);
        float maxDist = 0.5 * sqrt(u_AspectRatio * u_AspectRatio + 1.0); // 화면 대각선 거리의 절반

        float minRadius = -centerSmooth;
        float maxRadius = maxDist + centerSmooth;
        float currentRadius = mix(minRadius, maxRadius, u_Progress);
        
        mask = smoothstep(currentRadius - centerSmooth, currentRadius + centerSmooth, distToCenter);
    } else {
        mask = 0.0;
    }

    float finalH = mixHue(oldHSV.x, newHSV.x, mask);
    float finalS = mix(oldHSV.y, newHSV.y, mask);
    float finalV = mix(oldHSV.z, newHSV.z, mask);

    vec3 finalRGB = hsv2rgb(vec3(finalH, finalS, finalV));
    FragColor = vec4(finalRGB, 1.0);
}
)";

ScreenRenderer::ScreenRenderer()
    : m_screenWidth(0), m_screenHeight(0), m_shaderProgram(0), m_vao(0), m_vbo(0),
      m_locTransitionMode(-1), m_locProgress(-1),
      m_locOldColor(-1), m_locNewColor(-1), m_locIsgradient(-1),
      m_locGradientFeather(-1), m_locHueOffset(-1), m_aspectRatio(1.0f) {}

ScreenRenderer::~ScreenRenderer() {
    shutdown();
}

bool ScreenRenderer::initialize(int width, int height) {
    resize(width, height);

    // OpenGL 셰이더 컴파일 및 프로그램 링크
    m_shaderProgram = compileShader(vertexShaderSource, fragmentShaderSource);
    if (m_shaderProgram == 0) {
        std::cerr << "Failed to compile or link shaders." << std::endl;
        return false;
    }

    // Uniform 캐싱
    m_locTransitionMode = glGetUniformLocation(m_shaderProgram, "u_TransitionMode");
    m_locProgress = glGetUniformLocation(m_shaderProgram, "u_Progress");
    m_locAspectRatio = glGetUniformLocation(m_shaderProgram, "u_AspectRatio");
    m_locOldColor = glGetUniformLocation(m_shaderProgram, "u_OldColor");
    m_locNewColor = glGetUniformLocation(m_shaderProgram, "u_NewColor");
    m_locIsgradient = glGetUniformLocation(m_shaderProgram, "u_Isgradient");
    m_locGradientFeather = glGetUniformLocation(m_shaderProgram, "u_GradientFeather");
    m_locHueOffset = glGetUniformLocation(m_shaderProgram, "u_HueOffset");

    // 화면 전체를 덮는 사각형 VAO/VBO 설정
    setupQuad();

    return true;
}

void ScreenRenderer::setupQuad() {
    // 화면 전체를 덮는 삼각형 2개 정의 (X, Y, U, V)
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // 위치 속성 (Location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // 텍스처 좌표 속성 (Location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void ScreenRenderer::resize(int width, int height) {
    m_screenWidth = width;
    m_screenHeight = height;
    m_aspectRatio = height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
}

void ScreenRenderer::renderFrame(TransitionMode mode, float progress, 
                                  const HSVColor& oldColor, 
                                  const HSVColor& newColor,
                                  bool isGradient,
                                  float gradientFeather,
                                  float hueOffset)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    glUniform1f(m_locAspectRatio, m_aspectRatio);

    // Uniform 설정
    glUniform1i(m_locTransitionMode, static_cast<int>(mode));
    glUniform1f(m_locProgress, progress);

    constexpr float kDegreesToRadians = 0.01745329251994329577f;
    glUniform3f(m_locOldColor, oldColor.h, oldColor.s, oldColor.v);
    glUniform3f(m_locNewColor, newColor.h, newColor.s, newColor.v);
    glUniform1i(m_locIsgradient, isGradient ? 1 : 0);
    glUniform1f(m_locGradientFeather, gradientFeather);
    glUniform1f(m_locHueOffset, hueOffset * 90.0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

unsigned int ScreenRenderer::compileShader(const char* vertexSource, const char* fragmentSource) {
    // 버텍스 셰이더 컴파일
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation Failed: " << infoLog << std::endl;
        return 0;
    }

    // 프래그먼트 셰이더 컴파일
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return 0;
    }

    // 프로그램 링크
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // 사용이 끝난 개별 셰이더 자원 해제
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void ScreenRenderer::shutdown() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_shaderProgram != 0) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
}

} // namespace AmbientAurora
