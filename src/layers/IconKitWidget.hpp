#pragma once

#include "../offsets.hpp"
#include "../managers/IconKitObject.hpp"
#include "IconSelectPopup.hpp"
#include "GJGarageLayer.hpp"
#include "UnlockDialog.hpp"

class IconKitWidget : public cocos2d::CCLayer {
    public:
        static constexpr const float s_defWidth = 360.0f;
        static constexpr const float s_defHeight = 70.0f;

    protected:
        float m_fWidth = s_defWidth;
        float m_fHeight = s_defHeight;
        IconKitObject* m_pKitObject;

        CC_SYNTHESIZE(IconSelectPopup*, m_pParentPopup, ParentPopup);
        CC_SYNTHESIZE(GJGarageLayer*, m_pGarage, Garage);

        cocos2d::extension::CCScale9Sprite* m_pBGSprite;

        bool init(IconKitObject*, float);

        void onRemove(cocos2d::CCObject*);
        void onUse(cocos2d::CCObject*);
        UnlockList checkRequiredIcons();

    public:
        static IconKitWidget * create(IconKitObject*);
        static IconKitWidget * create(IconKitObject*, float);
};
