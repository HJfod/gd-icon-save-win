#pragma once

#include "../nodes/BrownAlertDelegate.hpp"
#include "GJGarageLayer.hpp"

class GJGarageLayer;

class IconSelectPopup : public BrownAlertDelegate {
    public:
        static constexpr const unsigned int s_nMaxOnPage = 10;

    protected:
        GJGarageLayer* m_pGarage;
        unsigned int m_nCurrentPage = 0;

        void setup() override;
        void onClose(cocos2d::CCObject*);

    public:
        void showKitRemovedMessage(float);

        static IconSelectPopup * create(GJGarageLayer* = nullptr);
};
