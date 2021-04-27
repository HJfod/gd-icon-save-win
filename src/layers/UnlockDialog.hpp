#pragma once

#include "../offsets.hpp"
#include "../nodes/BrownAlertDelegate.hpp"

using UnlockList = std::vector<std::pair<gd::UnlockType, uint32_t>>;

#include "IconKitWidget.hpp"

#define CONV_UT_IT(x) case gd::kUnlockType##x: return gd::kIconType##x;

constexpr gd::IconType getRelatedIconType(gd::UnlockType _type) {
    switch (_type) {
        CONV_UT_IT(Cube);
        CONV_UT_IT(Ship);
        CONV_UT_IT(Ball);
        CONV_UT_IT(Ufo);
        CONV_UT_IT(Wave);
        CONV_UT_IT(Robot);
        CONV_UT_IT(Spider);
        CONV_UT_IT(DeathEffect);
        CONV_UT_IT(Special);
    }

    return gd::IconType::kIconTypeCube;
}

#undef CONV_UT_IT

class IconKitWidget;

class UnlockDialog : public gd::FLAlertLayer {
    protected:
        cocos2d::CCSize m_pLrSize;
        IconKitWidget* m_pKitWidget;

        bool init(IconKitWidget*, float, float, UnlockList const&);
        void onUseAnyway(cocos2d::CCObject*);
    
    public:
        static UnlockDialog * create(IconKitWidget*, UnlockList const&);
};
