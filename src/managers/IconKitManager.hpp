#pragma once

#include "../offsets.hpp"
#include "IconKitObject.hpp"

class IconKitManager : public gd::GManager {
    cocos2d::CCArray* m_vKits;

    virtual bool init() override;

    virtual void encodeDataTo(DS_Dictionary* data);
    virtual void dataLoaded(DS_Dictionary* data);
    virtual void firstLoad();

    public:
        static IconKitManager* sharedState();

        void addKit(IconKitObject*);
        void removeKit(IconKitObject*);
        cocos2d::CCArray* getKits();

        ~IconKitManager();
};

namespace AppDelegateHook {
    inline void (__fastcall* trySaveGame)(cocos2d::CCObject*);
    inline void __fastcall trySaveGameHook(cocos2d::CCObject* _self) {
        IconKitManager::sharedState()->save();

        return trySaveGame(_self);
    }

    inline void (__thiscall* dataLoaded)(gd::GameManager*, DS_Dictionary*);
    inline void __fastcall dataLoadedHook(gd::GameManager* _self, edx_t, DS_Dictionary* _dict) {
        IconKitManager::sharedState();

        return dataLoaded(_self, _dict);
    }

    static inline bool loadHook() {
        auto res = MH_CreateHook(
            (PVOID)(gd::base + 0xCC500),
            as<LPVOID>(AppDelegateHook::trySaveGameHook),
            as<LPVOID*>(&AppDelegateHook::trySaveGame)
        ) == MH_OK;

        if (!res)
            return false;

        return MH_CreateHook(
            (PVOID)(gd::base + 0x3D5E0),
            as<LPVOID>(AppDelegateHook::trySaveGameHook),
            as<LPVOID*>(&AppDelegateHook::trySaveGame)
        ) == MH_OK;
    }
}
