#include "Color.h"
#include <iostream>
#include <cmath>

namespace AmbientAurora {
    // 위 주석에 따라 순차적으로 구현
    OKLCHColor RGBToOKLCH(const RGBColor& rgb) {
        // 1단계: RGB를 선형 공간으로 변환
        auto linearize = [](float channel) -> float {
            float c = std::max(0.0f, std::min(1.0f, channel));
            return (c <= 0.04045f) ? (c / 12.92f) : std::pow((c + 0.055f) / 1.055f, 2.4f);
        };

        float r_lin = linearize(rgb.r);
        float g_lin = linearize(rgb.g);
        float b_lin = linearize(rgb.b);

        // 2단계: 선형 RGB를 LMS 색 공간으로 변환
        float L = 0.4122214708f * r_lin + 0.5363325363f * g_lin + 0.0514459929f * b_lin;
        float M = 0.2119034982f * r_lin + 0.6806995451f * g_lin + 0.1073969566f * b_lin;
        float S = 0.0883024619f * r_lin + 0.2817188376f * g_lin + 0.6299787005f * b_lin;

        // 세제곱근 적용
        L = std::cbrt(L);
        M = std::cbrt(M);
        S = std::cbrt(S);

        // 3단계: LMS 공간을 OKLab 공간으로 변환
        float L_lab = 0.2104542553f * L + 0.7936177850f * M - 0.0040720468f * S;
        float a = 1.9779984951f * L - 2.4285922050f * M + 0.4505937099f * S;
        float b = 0.0259040371f * L + 0.7827717662f * M - 0.8086757660f * S;

        // 4단계: OKLab을 OKLCH로 변환
        float C = std::sqrt(a * a + b * b);
        float H_rad = std::atan2(b, a);
        float H_deg = H_rad * (180.0f / static_cast<float>(M_PI));
        if (H_deg < 0) H_deg += 360.0f;

        // 최종 반환 전 소수점 아래 3자리까지만 내림으로 반환
        L_lab = std::floor(L_lab * 1000.0f) / 1000.0f;
        C = std::floor(C * 1000.0f) / 1000.0f;
        H_deg = std::floor(H_deg * 1000.0f) / 1000.0f;

        return OKLCHColor(L_lab, C, H_deg);
    }

    RGBColor OKLCHToRGB(const OKLCHColor& oklch) {
        // 4단계: OKLCH를 OKLab으로 변환
        float L_lab = oklch.l;
        float C = oklch.c;
        float H_rad = oklch.h * (static_cast<float>(M_PI) / 180.0f);
        float a = C * std::cos(H_rad);
        float b = C * std::sin(H_rad);

        // 3단계: OKLab을 LMS 공간으로 변환
        float L = L_lab + 0.3963377774f * a + 0.2158037573f * b;
        float M = L_lab - 0.1055613458f * a - 0.0638541728f * b;
        float S = L_lab - 0.0894841775f * a - 1.2914855480f * b;

        // 세제곱 적용
        L = L * L * L;
        M = M * M * M;
        S = S * S * S;

        // 2단계: LMS를 선형 RGB로 변환
        float r_lin = 4.0767416621f * L - 3.3077115913f * M + 0.2309699292f * S;
        float g_lin = -1.2684380046f * L + 2.6097574011f * M - 0.3413193965f * S;
        float b_lin = -0.0041960863f * L - 0.7034186147f * M + 1.7076147010f * S;

        // 1단계: 선형 RGB를 sRGB로 변환
        auto delinearize = [](float channel) -> float {
            channel = std::max(0.0f, std::min(1.0f, channel)); // Clamp to [0,1]
            return (channel <= 0.0031308f) ? (channel * 12.92f)
                                           : (1.055f * std::pow(channel, 1.0f / 2.4f) - 0.055f);
        };
        return RGBColor(delinearize(r_lin), delinearize(g_lin), delinearize(b_lin));
    }
} // namespace AmbientAurora