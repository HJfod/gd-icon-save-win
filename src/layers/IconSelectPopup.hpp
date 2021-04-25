#pragma once

#include "../nodes/BrownAlertDelegate.hpp"
#include "GJGarageLayer.hpp"

class GJGarageLayer;

class IconSelectPopup : public BrownAlertDelegate {
    protected:
        GJGarageLayer* m_pGarage;

        void setup() override;
        void onClose(cocos2d::CCObject*);

    public:
        void showKitRemovedMessage(float);

        static IconSelectPopup * create(GJGarageLayer* = nullptr);
};
