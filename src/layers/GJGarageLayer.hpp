#pragma once

#include "../offsets.hpp"
#include "IconSelectPopup.hpp"

class GJGarageLayer : public cocos2d::CCLayer {
    private:
        PAD(0x18)
        gd::SimplePlayer* m_pPlayerPreview;

        GJGarageLayer* originalPointer;

        static inline bool (__fastcall* init)(GJGarageLayer*);
        static bool __fastcall initHook(GJGarageLayer*);

        void onSelectKit(cocos2d::CCObject*);
        void onSaveCurrent(cocos2d::CCObject*);
    
    public:
        inline gd::SimplePlayer* getPlayerPreview() { return m_pPlayerPreview; }

        static inline bool loadHook() {
            return MH_CreateHook(
                (PVOID)(gd::base + 0x1255d0),
                as<LPVOID>(GJGarageLayer::initHook),
                as<LPVOID*>(&GJGarageLayer::init)
            ) == MH_OK;
        }
};
