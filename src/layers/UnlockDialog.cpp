#include "UnlockDialog.hpp"

#define ID_MAX_(x, y) case gd::kUnlockType##x: if (_id > __STR_CAT__(##y,u)) return __STR_CAT__(##y,u); break;

// because robtop sometimes uses 2.2 cubes
// and the only way to reasonably check is
// by hardcoding this shit

// thanks rob
uint32_t checkIfIDIsFuture(gd::UnlockType _type, uint32_t _id) {
    switch (_type) {
        ID_MAX_(Cube,   142)
        ID_MAX_(Ship,   51)
        ID_MAX_(Ball,   43)
        ID_MAX_(Ufo,    35)
        ID_MAX_(Wave,   35)
        ID_MAX_(Robot,  26)
        ID_MAX_(Spider, 17)
        ID_MAX_(DeathEffect, 16)
        ID_MAX_(Special,7)
        ID_MAX_(Color1, 42)
        ID_MAX_(Color2, 42)
    }

    return _id;
}

#undef ID_MAX_

void UnlockDialog::onUseAnyway(cocos2d::CCObject* pSender) {
    this->m_pKitWidget->onUse(nullptr);

    gd::FLAlertLayer::onBtn1(pSender);
}

bool UnlockDialog::init(IconKitWidget* _par, float _w, float _h, UnlockList const& _list) {
    if (!gd::FLAlertLayer::initWithColor({ 0x00, 0x00, 0x00, 0x3B }))
        return false;

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    this->m_pLrSize = cocos2d::CCSize { _w, _h };
    this->m_pKitWidget = _par;

    this->m_pLayer = cocos2d::CCLayer::create();
    this->addChild(this->m_pLayer);

    auto bg = cocos2d::extension::CCScale9Sprite::create("square01_001.png", { 0.0f, 0.0f, 94.0f, 94.0f });
    bg->setContentSize(this->m_pLrSize);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    this->m_pLayer->addChild(bg);

    auto title = cocos2d::CCLabelBMFont::create("Missing Icons", "goldFont.fnt");

    title->setPosition(winSize.width / 2, winSize.height / 2 + this->m_pLrSize.height / 2 - 25.0f);
    title->setScale(.8f);
    
    this->m_pLayer->addChild(title);

    

    constexpr const unsigned int row_max = 4u;
    auto col_c = (_list.size() - _list.size() % row_max) / row_max + 1;

    auto iconMenu = cocos2d::CCMenu::create();
    std::vector<cocos2d::CCMenu*> rowMenus;
    for (auto i = 0u; i < col_c; i++)
        rowMenus.push_back(cocos2d::CCMenu::create());

    auto ix = 0u;
    auto cur_c = 0u;
    for (auto const& icon : _list) {
        auto plr = gd::GJItemIcon::createBrowserIcon(
            icon.first, checkIfIDIsFuture(icon.first, icon.second)
        );
        
        if (++ix % 4 == 0) cur_c++;

        rowMenus.at(cur_c)->addChild(plr);
    }

    for (auto const& row : rowMenus) {
        row->alignItemsHorizontallyWithPadding(15.0f);
        row->setContentSize({ 0, getChild<gd::GJItemIcon*>(row, 0)->getScaledContentSize().height });
        iconMenu->addChild(row);
    }
    
    iconMenu->alignItemsVerticallyWithPadding(5.0f);
    iconMenu->setPosition(winSize / 2);
    this->m_pLayer->addChild(iconMenu);

    this->m_pButtonMenu = cocos2d::CCMenu::create();
    this->m_pLayer->addChild(this->m_pButtonMenu);

    this->m_pButton1 = gd::ButtonSprite::create("Use Anyway", 0, 0, "goldFont.fnt", "GJ_button_01.png", 0.0f, .8f);
    this->m_pButton2 = gd::ButtonSprite::create("Cancel", 0, 0, "goldFont.fnt", "GJ_button_01.png", 0.0f, .8f);

    this->m_pButton1->setScale(.8f);
    this->m_pButton2->setScale(.8f);

    auto btnUseAnyway = gd::CCMenuItemSpriteExtra::create(
        this->m_pButton1,
        this,
        (cocos2d::SEL_MenuHandler)&UnlockDialog::onUseAnyway
    );
    this->m_pButtonMenu->addChild(btnUseAnyway);

    // todo: dont fuck up when copying robtop's icons

    auto btnOK = gd::CCMenuItemSpriteExtra::create(
        this->m_pButton2,
        this,
        (cocos2d::SEL_MenuHandler)&gd::FLAlertLayer::onBtn2
    );
    this->m_pButtonMenu->addChild(btnOK);

    this->m_pButtonMenu->alignItemsHorizontallyWithPadding(5.0f);
    btnUseAnyway->setPositionY(- this->m_pLrSize.height / 2 + 30.0f);
    btnOK->setPositionY(- this->m_pLrSize.height / 2 + 30.0f);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

UnlockDialog * UnlockDialog::create(IconKitWidget* _par, UnlockList const& _list) {
    auto pRet = new UnlockDialog();

    if (pRet && pRet->init(_par, 260.0f, 220.0f, _list)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return pRet;
}
