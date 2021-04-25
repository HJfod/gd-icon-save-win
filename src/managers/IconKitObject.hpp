#pragma once

#include "../offsets.hpp"

class IconKitObject : public cocos2d::CCObject {
    CC_SYNTHESIZE(uint32_t, m_nCubeID,  CubeID);
    CC_SYNTHESIZE(uint32_t, m_nShipID,  ShipID);
    CC_SYNTHESIZE(uint32_t, m_nBallID,  BallID);
    CC_SYNTHESIZE(uint32_t, m_nUfoID,   UfoID);
    CC_SYNTHESIZE(uint32_t, m_nWaveID,  WaveID);
    CC_SYNTHESIZE(uint32_t, m_nRobotID, RobotID);
    CC_SYNTHESIZE(uint32_t, m_nSpiderID,SpiderID);
    CC_SYNTHESIZE(uint32_t, m_nStreakID,StreakID);
    CC_SYNTHESIZE(uint32_t, m_nDeathID, DeathID);
    CC_SYNTHESIZE(gd::IconType, m_pFrame,   PlayerFrame);

    CC_SYNTHESIZE(uint32_t, m_nColor1,  Color1);
    CC_SYNTHESIZE(uint32_t, m_nColor2,  Color2);

    CC_SYNTHESIZE(bool,     m_bGlow,    GlowEnabled);

    public:
        virtual inline bool canEncode() override { return true; }

        inline void loadFromDict(DS_Dictionary* dict) {
            this->m_nCubeID     = dict->getIntegerForKey("k1");
            this->m_nShipID     = dict->getIntegerForKey("k2");
            this->m_nBallID     = dict->getIntegerForKey("k3");
            this->m_nUfoID      = dict->getIntegerForKey("k4");
            this->m_nWaveID     = dict->getIntegerForKey("k5");
            this->m_nRobotID    = dict->getIntegerForKey("k6");
            this->m_nSpiderID   = dict->getIntegerForKey("k7");
            this->m_nStreakID   = dict->getIntegerForKey("k8");
            this->m_nDeathID    = dict->getIntegerForKey("k9");

            this->m_pFrame      = static_cast<gd::IconType>(dict->getIntegerForKey("k10"));

            this->m_bGlow       = dict->getBoolForKey("k11");

            this->m_nColor1     = dict->getIntegerForKey("k12");
            this->m_nColor2     = dict->getIntegerForKey("k13");
        }

        virtual inline void encodeWithCoder(DS_Dictionary* dict) override {
            dict->setIntegerForKey("kCEK", 53);

            dict->setIntegerForKey("k1", this->m_nCubeID);
            dict->setIntegerForKey("k2", this->m_nShipID);
            dict->setIntegerForKey("k3", this->m_nBallID);
            dict->setIntegerForKey("k4", this->m_nUfoID);
            dict->setIntegerForKey("k5", this->m_nWaveID);
            dict->setIntegerForKey("k6", this->m_nRobotID);
            dict->setIntegerForKey("k7", this->m_nSpiderID);
            dict->setIntegerForKey("k8", this->m_nStreakID);
            dict->setIntegerForKey("k9", this->m_nDeathID);

            dict->setIntegerForKey("k10", static_cast<int>(this->m_pFrame));

            dict->setBoolForKey("k11", this->m_bGlow);

            dict->setIntegerForKey("k12", this->m_nColor1);
            dict->setIntegerForKey("k13", this->m_nColor2);
        }

        static inline IconKitObject* createWithCoder(DS_Dictionary* dict) {
            auto obj = new IconKitObject();
            obj->loadFromDict(dict);
            return obj;
        }
};
