#pragma once

#include "../offsets.hpp"
#include "../nodes/BrownAlertDelegate.hpp"
#include "../nodes/InputNode.hpp"

using RenameCallback = std::function<void(const char*)>;

class RenameDialog : public BrownAlertDelegate {
    protected:
        RenameCallback m_cbCallback = nullptr;
        InputNode* m_pInput;

        void setup() override;

        void onRename(cocos2d::CCObject*);
    
    public:
        static RenameDialog * create();

        void setCallback(RenameCallback);
};
