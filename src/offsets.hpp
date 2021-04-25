#pragma once

#ifndef INCLUDE_SUBMODULES
#define INCLUDE_SUBMODULES

#define MOD_VERSION "1.0"
#define MOD_VERSION_INT 1

#pragma warning( push, 0 )
#include <cocos2d.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <MinHook.h>
#include <gd.h>
#pragma warning( pop )

template<typename T>
static T getChild(cocos2d::CCNode* x, int i) {
    return static_cast<T>(x->getChildren()->objectAtIndex(i));
}

template <typename T, typename R>
T as(R const v) { return reinterpret_cast<T>(v); }

inline std::string operator"" _s (const char* _txt, size_t) {
    return std::string(_txt);
}

template<typename T, typename U> constexpr size_t offsetOf(U T::*member) {
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

inline bool patchBytes(
    uintptr_t const address,
    std::vector<uint8_t> const& bytes
) {
    return WriteProcessMemory(
        GetCurrentProcess(),
        reinterpret_cast<LPVOID>(gd::base + address),
        bytes.data(),
        bytes.size(),
        nullptr
    );
}

using unknown_t = uintptr_t;
using edx_t = uintptr_t;

typedef const char* nullstr_t;
static constexpr nullstr_t nullstr = "";

#endif
