#include "IconKitWidget.hpp"
#include "../managers/IconKitManager.hpp"
#include "RenameDialog.hpp"
#include "../nodes/PickMenu.hpp"
#include <shlobj_core.h>
#undef max

const wchar_t *GetWC(const char *c) {
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];

    size_t out;
    mbstowcs_s(&out, wc, cSize, c, cSize - 1);

    return wc;
}

UnlockList IconKitWidget::checkRequiredIcons() {
    auto gm = gd::GameManager::sharedState();

    UnlockList unlocks;

    for (auto const& icon : UnlockList {
        { gd::kUnlockTypeCube,          this->m_pKitObject->getCubeID() },
        { gd::kUnlockTypeShip,          this->m_pKitObject->getShipID() },
        { gd::kUnlockTypeBall,          this->m_pKitObject->getBallID() },
        { gd::kUnlockTypeUfo,           this->m_pKitObject->getUfoID() },
        { gd::kUnlockTypeWave,          this->m_pKitObject->getWaveID() },
        { gd::kUnlockTypeRobot,         this->m_pKitObject->getRobotID() },
        { gd::kUnlockTypeSpider,        this->m_pKitObject->getSpiderID() },
        { gd::kUnlockTypeDeathEffect,   this->m_pKitObject->getDeathID() },
        { gd::kUnlockTypeSpecial,       this->m_pKitObject->getStreakID() },
    })
        if (!gm->isIconUnlocked(icon.second, getRelatedIconType(icon.first)))
            unlocks.push_back(icon);
    
    if (!gm->isColorUnlocked(this->m_pKitObject->getColor1(), false))
        unlocks.push_back({ gd::kUnlockTypeColor1, this->m_pKitObject->getColor1() });

    if (!gm->isColorUnlocked(this->m_pKitObject->getColor2(), true))
        unlocks.push_back({ gd::kUnlockTypeColor2, this->m_pKitObject->getColor2() });

    if (this->m_pKitObject->getGlowEnabled() && !gm->isIconUnlocked(2, gd::kIconTypeSpecial))
        unlocks.push_back({ gd::kUnlockTypeSpecial, 2 });

    return unlocks;
}

void IconKitWidget::FLAlert_Clicked(gd::FLAlertLayer* _this, bool _btn2) {
    if (_btn2) {
        PWSTR path;
        if (!SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path)) {
            auto name = this->m_pKitObject->getName() + ".xml";
            auto wname = GetWC( name.c_str() );

            std::wstring w = path;
            ITEMIDLIST *pidl = ILCreateFromPathW((w + L"\\GeometryDash\\" + wname).c_str());

            if (pidl) {
                SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
                ILFree(pidl);
            }
            
            delete wname;
            free(path);
        }
    }
}

void IconKitWidget::onRemove(cocos2d::CCObject* pSender) {
    IconKitManager::sharedState()->removeKit(this->m_pKitObject);

    if (this->m_pParentPopup)
        this->m_pParentPopup->showKitRemovedMessage(this->getPositionY());

    this->removeFromParentAndCleanup(true);
}

void IconKitWidget::onUse(cocos2d::CCObject* pSender) {
    auto req = this->checkRequiredIcons();
    if (req.size() && pSender) {
        UnlockDialog::create(this, req)->show();
        return;
    }

    auto gm = gd::GameManager::sharedState();

    gm->setPlayerFrame(this->m_pKitObject->getCubeID());
    gm->setPlayerShip(this->m_pKitObject->getShipID());
    gm->setPlayerBall(this->m_pKitObject->getBallID());
    gm->setPlayerBird(this->m_pKitObject->getUfoID());
    gm->setPlayerDart(this->m_pKitObject->getWaveID());
    gm->setPlayerRobot(this->m_pKitObject->getRobotID());
    gm->setPlayerSpider(this->m_pKitObject->getSpiderID());
    gm->setPlayerStreak(std::max(this->m_pKitObject->getStreakID(), 1u));
    gm->setPlayerDeathEffect(this->m_pKitObject->getDeathID());
    gm->setPlayerIconType(this->m_pKitObject->getPlayerFrame());
    gm->setPlayerGlow(this->m_pKitObject->getGlowEnabled());
    gm->setPlayerColor(this->m_pKitObject->getColor1());
    gm->setPlayerColor2(this->m_pKitObject->getColor2());

    if (this->m_pGarage) {
        auto id = 1u;

        switch (this->m_pKitObject->getPlayerFrame()) {
            case gd::kIconTypeCube:
                id = this->m_pKitObject->getCubeID();
                break;
            case gd::kIconTypeShip:
                id = this->m_pKitObject->getShipID();
                break;
            case gd::kIconTypeBall:
                id = this->m_pKitObject->getBallID();
                break;
            case gd::kIconTypeUfo:
                id = this->m_pKitObject->getUfoID();
                break;
            case gd::kIconTypeWave:
                id = this->m_pKitObject->getWaveID();
                break;
            case gd::kIconTypeRobot:
                id = this->m_pKitObject->getRobotID();
                break;
            case gd::kIconTypeSpider:
                id = this->m_pKitObject->getSpiderID();
                break;
        }
        
        this->m_pGarage->getPlayerPreview()->updatePlayerFrame(
            id, this->m_pKitObject->getPlayerFrame()
        );
        this->m_pGarage->getPlayerPreview()->setColor(
            gm->colorForIdx(this->m_pKitObject->getColor1())
        );
        this->m_pGarage->getPlayerPreview()->setSecondColor(
            gm->colorForIdx(this->m_pKitObject->getColor2())
        );
        this->m_pGarage->getPlayerPreview()->setGlowOutline(this->m_pKitObject->getGlowEnabled());
        this->m_pGarage->getPlayerPreview()->updateColors();
    }

    auto alert = gd::TextAlertPopup::create("Kit Enabled!", .5f, .6f);
    this->m_pGarage->addChild(alert, 9999);

    this->m_pParentPopup->removeFromParentAndCleanup(true);
}

void IconKitWidget::onRename(cocos2d::CCObject* pSender) {
    auto dialog = RenameDialog::create();
    
    if (dialog) {
        dialog->show();
        dialog->setCallback([this](const char* _str) -> void {
            if (!strlen(_str))
                _str = "Unnamed";
                
            this->m_pNameLabel->setString(_str);
            this->m_pKitObject->setName(_str);
        });
    }
}

void IconKitWidget::onShare(cocos2d::CCObject*) {
    auto dict = new DS_Dictionary();

    dict->setSubDictForKey("kit");
    if (!dict->stepIntoSubDictWithKey("kit"))
        return delete dict;

    this->m_pKitObject->encodeWithCoder(dict);

    dict->stepOutOfSubDict();

    auto name = this->m_pKitObject->getName() + ".xml";

    if (!dict->saveRootSubDictToFile(name.c_str()))
        return delete dict;

    gd::FLAlertLayer::create(
        this,
        "Kit saved!",
        "OK", "Show File",
        "Saved kit to <co>" + name + "</c>!"
    )->show();

    delete dict;
}

void IconKitWidget::onMoveToTop(cocos2d::CCObject*) {
    IconKitManager::sharedState()->moveKitToTop(this->m_pKitObject);
    
    if (this->m_pParentPopup)
        this->m_pParentPopup->refreshList();
}

void IconKitWidget::onEditKit(cocos2d::CCObject*) {
    if (this->m_pParentPopup)
        this->m_pParentPopup->editKit(this->m_pKitObject);
}

void IconKitWidget::onSettings(cocos2d::CCObject*) {
    PickMenu::create(
        "",
        this,
        {
            // { "Modify kit", (cocos2d::SEL_MenuHandler)&IconKitWidget::onEditKit },
            { "Delete kit", (cocos2d::SEL_MenuHandler)&IconKitWidget::onRemove },
            { "Move to top", (cocos2d::SEL_MenuHandler)&IconKitWidget::onMoveToTop },
            // { "Export to File", (cocos2d::SEL_MenuHandler)&IconKitWidget::onShare },
        }
    )->show();
}

bool IconKitWidget::init(IconKitObject* _obj, float _width) {
    if (!cocos2d::CCNode::init())
        return false;

    this->m_fWidth = _width;
    this->m_pKitObject = _obj;
    
    const float padding = this->m_fHeight / 2;

    auto gm = gd::GameManager::sharedState();

    this->setContentSize({ this->m_fWidth, this->m_fHeight });
    
    this->m_pBGSprite = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    this->m_pBGSprite->setContentSize({ this->m_fWidth, this->m_fHeight });
    this->m_pBGSprite->setPosition(this->getContentSize() / 2);

    auto menu = cocos2d::CCMenu::create();
    
    this->m_pNameLabel = cocos2d::CCLabelBMFont::create(
        this->m_pKitObject->getName().c_str(),
        "goldFont.fnt"
    );
    this->m_pNameLabel->setScale(.5f);
    auto labelBtn = gd::CCMenuItemSpriteExtra::create(
        this->m_pNameLabel,
        this,
        (cocos2d::SEL_MenuHandler)&IconKitWidget::onRename
    );
    labelBtn->setPosition(0.0f, this->m_fHeight / 2 - 10.0f);
    menu->addChild(labelBtn);

    auto ix = 0u;
    for (auto const& icon_d : std::vector<std::pair<uint32_t, gd::IconType>> {
        { _obj->getCubeID(),    gd::IconType::kIconTypeCube },
        { _obj->getShipID(),    gd::IconType::kIconTypeShip },
        { _obj->getBallID(),    gd::IconType::kIconTypeBall },
        { _obj->getUfoID(),     gd::IconType::kIconTypeUfo },
        { _obj->getWaveID(),    gd::IconType::kIconTypeWave },
        { _obj->getRobotID(),   gd::IconType::kIconTypeRobot },
        { _obj->getSpiderID(),  gd::IconType::kIconTypeSpider },
    }) {
        auto icon = gd::SimplePlayer::create(1);

        icon->updatePlayerFrame(icon_d.first, icon_d.second);

        icon->setColor(gm->colorForIdx(_obj->getColor1()));
        icon->setSecondColor(gm->colorForIdx(_obj->getColor2()));
        icon->setScale(.8f);

        if (_obj->getGlowEnabled()) {
            icon->setGlowOutline(true);
            icon->updateColors();
        }

        icon->setPosition({ padding + ix++ * 33.0f, this->m_fHeight / 2 });
        this->m_pBGSprite->addChild(icon);
    }

    auto spr_delete = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"); // GJ_trashBtn_001.png
    spr_delete->setScale(.8f);

    auto deleteButton = gd::CCMenuItemSpriteExtra::create(
        spr_delete, this, (cocos2d::SEL_MenuHandler)&IconKitWidget::onSettings
    );
    deleteButton->setPosition(this->m_fWidth / 2 - padding, 0.0f);

    auto spr_use = cocos2d::CCSprite::createWithSpriteFrameName("GJ_selectSongBtn_001.png");
    spr_use->setScale(.8f);

    auto useButton = gd::CCMenuItemSpriteExtra::create(
        spr_use, this, (cocos2d::SEL_MenuHandler)&IconKitWidget::onUse
    );
    useButton->setPosition(this->m_fWidth / 2 - padding - 40.0f, 0.0f);

    // auto spr_share = cocos2d::CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    // spr_share->setScale(.5f);
    // spr_share->setFlipX(true);
    // spr_share->setRotation(270.0f);

    // auto shareButton = gd::CCMenuItemSpriteExtra::create(
    //     spr_share, this, (cocos2d::SEL_MenuHandler)&IconKitWidget::onShare
    // );
    // shareButton->setPosition(this->m_fWidth / 2 - 15.0f, this->m_fHeight / 2 - 15.0f);

    menu->addChild(useButton);
    menu->addChild(deleteButton);
    // menu->addChild(shareButton);
    menu->setPosition(this->m_fWidth/ 2, this->m_fHeight / 2);

    this->addChild(menu, 100);

    this->addChild(this->m_pBGSprite);

    return true;
}

IconKitWidget* IconKitWidget::create(IconKitObject* _obj) {
    return IconKitWidget::create(_obj, IconKitWidget::s_defWidth);
}

IconKitWidget* IconKitWidget::create(IconKitObject* _obj, float _width) {
    auto pRet = new IconKitWidget();

    if (pRet && pRet->init(_obj, _width)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
