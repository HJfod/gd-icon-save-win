#include "RenameDialog.hpp"

void RenameDialog::setup() {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    this->m_pInput = InputNode::create(150.0f, this->m_sPlaceholder, "bigFont.fnt");
    this->m_pInput->setPosition(winSize / 2);
    this->m_pLayer->addChild(this->m_pInput);

    auto button = gd::CCMenuItemSpriteExtra::create(
        gd::ButtonSprite::create(
            this->m_sBtnText, 0, 0, "goldFont.fnt", "GJ_button_01.png", 0, .8f
        ),
        this,
        (cocos2d::SEL_MenuHandler)&RenameDialog::onRename
    );
    button->setPosition(0.0f, - this->m_pLrSize.height / 2 + 25.0f);

    this->m_pButtonMenu->addChild(button);
}

void RenameDialog::onRename(cocos2d::CCObject* pSender) {
    if (this->m_cbCallback)
        this->m_cbCallback(this->m_pInput->getString());
    
    this->onClose(nullptr);
}

void RenameDialog::setCallback(RenameCallback _cb) {
    this->m_cbCallback = _cb;
}

bool RenameDialog::initWithText(float _w, float _h, const char * _bg, const char* _pg, const char* _btn) {
    this->m_sPlaceholder = _pg;
    this->m_sBtnText = _btn;

    return this->init(_w, _h, _bg);
}

RenameDialog * RenameDialog::create() {
    auto pRet = new RenameDialog();

    if (pRet && pRet->initWithText(200.0f, 140.0f, "GJ_square02.png")) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

RenameDialog * RenameDialog::create(const char* _ph, const char* _btn) {
    auto pRet = new RenameDialog();

    if (pRet && pRet->initWithText(200.0f, 140.0f, "GJ_square02.png", _ph, _btn)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
