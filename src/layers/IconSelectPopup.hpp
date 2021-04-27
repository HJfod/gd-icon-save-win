#pragma once

#include "../nodes/BrownAlertDelegate.hpp"
#include "GJGarageLayer.hpp"

class GJGarageLayer;

class IconSelectPopup : public BrownAlertDelegate {
    public:
        static constexpr const unsigned int s_nMaxOnPage = 10;

    protected:
        GJGarageLayer* m_pGarage;
        cocos2d::CCLabelBMFont* m_pPageLabel;
        
        unsigned int m_nCurrentPage = 0;
        cocos2d::CCSize m_sScrLayerSize;

        gd::CCMenuItemSpriteExtra* m_pBtnLeft;
        gd::CCMenuItemSpriteExtra* m_pBtnRight;

        void setup() override;
        void onClose(cocos2d::CCObject*);

        void showPage(unsigned int);
        void onNextPage(cocos2d::CCObject*);
        void onPrevPage(cocos2d::CCObject*);

    public:
        void showKitRemovedMessage(float);

        static IconSelectPopup * create(GJGarageLayer* = nullptr);
};
