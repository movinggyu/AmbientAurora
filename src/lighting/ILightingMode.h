#ifndef AMBIENT_AURORA_ILIGHTINGMODE_H
#define AMBIENT_AURORA_ILIGHTINGMODE_H

#include "../core/Color.h"

namespace AmbientAurora {
    class ILightingMode {
    public:
        // 가상 소멸자
        virtual ~ILightingMode() = default;

        // 화면에 그려야 할 색을 반환한다.)
        virtual RGBColor getCurrentColor() const = 0;
    };
} // namespace AmbientAurora

#endif // AMBIENT_AURORA_ILIGHTINGMODE_H