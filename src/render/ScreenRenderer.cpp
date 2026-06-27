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

uniform vec3 u_OldColors[4]; // 이전 색상 배열
uniform int u_OldColorCount; // 이전 색상 개수
uniform vec3 u_NewColors[4]; // 새 색상 배열
uniform int u_NewColorCount; // 새 색상 개수

// 의사 난수 생성 함수 (Dissolve 모드에서 사용)
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// 색상 배열과 UV 좌표를 기반으로 세로 그라데이션 색상을 보간 추출
vec3 sampleColor(vec3 colors[4], int count, vec2 uv) {
    if (count <= 1) return colors[0];
    
    float t = uv.y; 
    if (count == 2) {
        return mix(colors[0], colors[1], t);
    } else if (count == 3) {
        if (t < 0.5) return mix(colors[0], colors[1], t * 2.0);
        else return mix(colors[1], colors[2], (t - 0.5) * 2.0);
    } else {
        if (t < 0.333) return mix(colors[0], colors[1], t * 3.0);
        else if (t < 0.666) return mix(colors[1], colors[2], (t - 0.333) * 3.0);
        else return mix(colors[2], colors[3], (t - 0.666) * 3.0);
    }
}

void main()
{
    // 현재 픽셀 위치에서의 이전 색상과 새 색상을 샘플링
    vec3 oldColor = sampleColor(u_OldColors, u_OldColorCount, TexCoord);
    vec3 newColor = sampleColor(u_NewColors, u_NewColorCount, TexCoord);

    float mask = 0.0; // 마스크 값 초기화
    float edgeSmooth = 1.0; // 가장자리 부드럽게 처리 (0.0 ~ 1.0)
    
    // 전환 모드에 따라 색상 혼합 처리 (DISSOLVE, WIPE, CENTER 등)
    if (u_TransitionMode == 0) { // DISSOLVE
        float noise = rand(TexCoord * 100.0); 
        mask = smoothstep(u_Progress - edgeSmooth, u_Progress + edgeSmooth, noise);
    }
    else if (u_TransitionMode == 1) { // WIPE
        mask = smoothstep(u_Progress - edgeSmooth, u_Progress + edgeSmooth, TexCoord.x);
    }
    else if (u_TransitionMode == 2) { // CENTER
        float distToCenter = distance(TexCoord, vec2(0.5));
        float maxDist = 0.7071;
        float currentRadius = u_Progress * maxDist;
        mask = smoothstep(u_Progress - edgeSmooth, currentRadius + edgeSmooth, distToCenter);
    } else {
        mask = 0.0;
    }

    // mask가 0.0에 가까우면 newColor, 1.0에 가까우면 oldColor를 반환
    FragColor = vec4(mix(newColor, oldColor, mask), 1.0);
}
)";

ScreenRenderer::ScreenRenderer()
    : m_screenWidth(0), m_screenHeight(0), m_shaderProgram(0), m_vao(0), m_vbo(0),
      m_locTransitionMode(-1), m_locProgress(-1),
      m_locOldColors(-1), m_locOldColorCount(-1),
      m_locNewColors(-1), m_locNewColorCount(-1) {}

ScreenRenderer::~ScreenRenderer() {
    shutdown();
}

bool ScreenRenderer::initialize(int width, int height) {
    m_screenWidth = width;
    m_screenHeight = height;

    // OpenGL 셰이더 컴파일 및 프로그램 링크
    m_shaderProgram = compileShader(vertexShaderSource, fragmentShaderSource);
    if (m_shaderProgram == 0) {
        std::cerr << "Failed to compile or link shaders." << std::endl;
        return false;
    }

    // Uniform 캐싱
    m_locTransitionMode = glGetUniformLocation(m_shaderProgram, "u_TransitionMode");
    m_locProgress = glGetUniformLocation(m_shaderProgram, "u_Progress");
    m_locOldColors = glGetUniformLocation(m_shaderProgram, "u_OldColors");
    m_locOldColorCount = glGetUniformLocation(m_shaderProgram, "u_OldColorCount");
    m_locNewColors = glGetUniformLocation(m_shaderProgram, "u_NewColors");
    m_locNewColorCount = glGetUniformLocation(m_shaderProgram, "u_NewColorCount");

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

void ScreenRenderer::renderFrame(TransitionMode mode, float progress, 
                                  const std::vector<RGBColor>& oldColors, 
                                  const std::vector<RGBColor>& newColors)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    // Uniform 설정
    glUniform1i(m_locTransitionMode, static_cast<int>(mode));
    glUniform1f(m_locProgress, progress);

    // 최대 4개의 색상 제한
    int oldColorCount = std::min(static_cast<int>(oldColors.size()), 4);
    int newColorCount = std::min(static_cast<int>(newColors.size()), 4);

    glUniform1i(m_locOldColorCount, oldColorCount);
    glUniform1i(m_locNewColorCount, newColorCount);

    // std::vector<RGBColor> 구조체를 OpenGL용 단일 float 배열로 변환
    if (oldColorCount > 0) {
        std::vector<float> oldColorFloats;
        for (int i = 0; i < oldColorCount; ++i) {
            oldColorFloats.push_back(oldColors[i].r);
            oldColorFloats.push_back(oldColors[i].g);
            oldColorFloats.push_back(oldColors[i].b);
        }
        glUniform3fv(m_locOldColors, oldColorCount, oldColorFloats.data());
    }

    if (newColorCount > 0) {
        std::vector<float> newColorFloats;
        for (int i = 0; i < newColorCount; ++i) {
            newColorFloats.push_back(newColors[i].r);
            newColorFloats.push_back(newColors[i].g);
            newColorFloats.push_back(newColors[i].b);
        }
        glUniform3fv(m_locNewColors, newColorCount, newColorFloats.data());
    }

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