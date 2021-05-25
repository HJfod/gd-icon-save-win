// generated through GDMake https://github.com/HJfod/GDMake

// include GDMake & submodules
#include <GDMake.h>
#include "src/layers/GJGarageLayer.hpp"
#include "src/layers/ProfilePage.hpp"
#include "src/managers/IconKitManager.hpp"

GDMAKE_MAIN {
    GJGarageLayer::loadHook();
    ProfilePage::loadHook();
    AppDelegateHook::loadHook();
}

GDMAKE_UNLOAD {}
