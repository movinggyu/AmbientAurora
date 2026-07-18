#ifndef AMBIENT_AURORA_ILIGHTINGMODE_H
#define AMBIENT_AURORA_ILIGHTINGMODE_H

#include "../core/Color.h"

namespace AmbientAurora {
    class ILightingMode {
    public:
        // 가상 소멸자
        virtual ~ILightingMode() = default;

        // 현재 색을 반환한다.
        virtual OKLCHColor getCurrentColor() const = 0;

        // 다음 목표 색을 반환하고 현재 색상을 업데이트한다.
        virtual OKLCHColor getAndSetNextColor() = 0;

        virtual void onActivate(const OKLCHColor& prevColor) = 0;
    };
} // namespace AmbientAurora

#endif // AMBIENT_AURORA_ILIGHTINGMODE_H
