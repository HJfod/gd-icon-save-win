#include "mod.hpp"
#include "layers/GJGarageLayer.hpp"
#include "layers/ProfilePage.hpp"
#include "managers/IconKitManager.hpp"

bool mod::load() {
    if (MH_Initialize() != MH_OK) [[unlikely]]
        return false;

    if (!GJGarageLayer::loadHook())
        return false;
    if (!ProfilePage::loadHook())
        return false;
    if (!AppDelegateHook::loadHook())
        return false;
        
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) [[unlikely]] {
        MH_Uninitialize();
        return false;
    }
    
    return true;
}

void mod::unload() {
    MH_DisableHook(MH_ALL_HOOKS);

    MH_Uninitialize();
}
