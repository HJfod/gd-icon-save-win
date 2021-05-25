#pragma once

#include "../offsets.hpp"

class ProfilePage : public gd::FLAlertLayer {
    protected:
        PAD(0x1c)
        gd::GJUserScore* m_pScore;

        inline static bool (__thiscall* init)(ProfilePage*, int, bool);
        static bool __fastcall initHook(ProfilePage*, edx_t, int, bool);

        void onSaveIcons(cocos2d::CCObject*);

    public:
        static inline bool loadHook() {
            return MH_CreateHook(
                (PVOID)(gd::base + 0x20ef00),
                as<LPVOID>(ProfilePage::initHook),
                as<LPVOID*>(&ProfilePage::init)
            ) == MH_OK;
        }
};
