#include "IconSelectPopup.hpp"
#include "../managers/IconKitManager.hpp"
#include "IconKitWidget.hpp"
#include "RenameDialog.hpp"
#undef max
#undef min

std::string lower(std::string const& _text) {
    auto data = _text;

    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    
    return data;
}

void IconSelectPopup::onClose(cocos2d::CCObject* pSender) {
    // change border color back LOL
    patchBytes(0x2db6f, { 0x6a, 0x28, 0x6a, 0x14, 0x6a, 0x0 });

    BrownAlertDelegate::onClose(pSender);
}

void IconSelectPopup::showKitRemovedMessage(float _y) {
    auto label = cocos2d::CCLabelBMFont::create("Kit removed!", "bigFont.fnt");

    label->setScale(1.4f);

    auto bgSprite = cocos2d::extension::CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );

    bgSprite->setScale(.5f);
    bgSprite->setColor({ 0, 0, 0 });
    bgSprite->setOpacity(75);
    bgSprite->setContentSize(label->getScaledContentSize() * 2.0f);

    label->setPosition(label->getScaledContentSize());

    bgSprite->addChild(label);
    bgSprite->setPosition(
        this->m_pScrollingLayer->getScaledContentSize().width / 2,
        _y + IconKitWidget::s_defHeight / 2
    );

    this->m_pScrollingLayer->m_pScrollLayer->addChild(bgSprite);
}

void IconSelectPopup::onNextPage(cocos2d::CCObject*) {
    auto kits = IconKitManager::sharedState()->getKits();

    unsigned int maxPage = static_cast<unsigned int>(
        std::floor(kits->count() / static_cast<double>(this->s_nMaxOnPage))
    );

    if (this->m_nCurrentPage < maxPage)
        this->m_nCurrentPage++;
    
    if (this->m_nCurrentPage == maxPage)
        this->m_pBtnRight->setVisible(false);
    
    this->showPage(this->m_nCurrentPage);

    this->m_pBtnLeft->setVisible(true);
}

void IconSelectPopup::onPrevPage(cocos2d::CCObject*) {
    auto kits = IconKitManager::sharedState()->getKits();

    if (this->m_nCurrentPage > 0)
        this->m_nCurrentPage--;
    
    if (!this->m_nCurrentPage)
        this->m_pBtnLeft->setVisible(false);
    
    this->showPage(this->m_nCurrentPage);

    this->m_pBtnRight->setVisible(true);
}

void IconSelectPopup::showPage(unsigned int _page, const char* _filter) {
    std::vector<IconKitObject*> kits;
    auto kitsO = IconKitManager::sharedState()->getKits();
    
    for (auto ix = 0u; ix < kitsO->count(); ix++) {
        auto kit = dynamic_cast<IconKitObject*>(kitsO->objectAtIndex(ix));
        
        if (lower(kit->getName()).find(lower(_filter)) != std::string::npos)
            kits.push_back(kit);
    }

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    auto count = std::min(
        static_cast<unsigned int>(kits.size()) - this->m_nCurrentPage * IconSelectPopup::s_nMaxOnPage,
        IconSelectPopup::s_nMaxOnPage
    );
    unsigned int maxPage = static_cast<unsigned int>(
        std::floor(kits.size() / static_cast<double>(this->s_nMaxOnPage))
    );

    auto pageStr = "Page " + std::to_string(this->m_nCurrentPage + 1) + "/" + std::to_string(maxPage + 1);
    this->m_pPageLabel->setString(pageStr.c_str());

    auto w = m_sScrLayerSize.width - 20.0f;
    auto h = IconKitWidget::s_defHeight;

    this->m_pScrollingLayer->m_pScrollLayer->removeAllChildrenWithCleanup(true);
    this->m_pScrollingLayer->m_pScrollLayer->setPositionY(0.0f);

    for (auto ix = 0u; ix < count; ix++) {
        auto kit = dynamic_cast<IconKitObject*>(kits.at(
            ix + this->m_nCurrentPage * IconSelectPopup::s_nMaxOnPage
        ));
        auto kitWidget = IconKitWidget::create(kit, w);

        kitWidget->setPosition(
            winSize.width / 2 - w / 2, m_sScrLayerSize.height - h / 2 - (h + 5.0f) * ix
        );
        kitWidget->setParentPopup(this);
        kitWidget->setGarage(this->m_pGarage);

        this->m_pScrollingLayer->m_pScrollLayer->addChild(kitWidget);

        this->registerWithTouchDispatcher();
        cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

        this->setMouseEnabled(true);
        this->setTouchEnabled(true);
    }
}

void IconSelectPopup::onSearch(cocos2d::CCObject*) {
    auto dialog = RenameDialog::create("Search Kits", "Search");

    if (dialog) {
        dialog->show();
        dialog->setCallback([this](const char* _str) -> void {
            this->showPage(0, _str);
        });
    }
}

void IconSelectPopup::setup() {
    // TODO: not write code this horrible lmao
    // (fixes the border color of ScrollingLayer)
    patchBytes(0x2db6f, { 0x6a, 0x1a, 0x6a, 0x29, 0x6a, 0x4c });

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    auto kits = IconKitManager::sharedState()->getKits();
    
    constexpr const float scrollLrOffset = 20.0f;
    float lrWidth = this->m_pLrSize.width - 50.0f;
    float lrHeight = this->m_pLrSize.height - 50.0f - scrollLrOffset;

    this->m_sScrLayerSize = cocos2d::CCSize { lrWidth, lrHeight };

    auto titleStr = "Saved Icon Kits (" + std::to_string(kits->count()) + ")";
    auto title = cocos2d::CCLabelBMFont::create(titleStr.c_str(), "bigFont.fnt");

    title->setScale(.7f);
    title->setPosition(winSize.width / 2, winSize.height - 50.0f);

    this->m_pLayer->addChild(title);

    this->m_pScrollingLayer = gd::ScrollingLayer::create(
        { lrWidth, lrHeight },
        { winSize.width / 2, winSize.height / 2 - 10.0f },
        std::max(0.0f, (kits->count() + 1) * IconKitWidget::s_defHeight - lrHeight)
    );
    this->m_pScrollingLayer->setMouseEnabled(true);
    this->m_pScrollingLayer->m_pParent = this->m_pLayer;

    this->m_pPageLabel = cocos2d::CCLabelBMFont::create("Page ~/~", "goldFont.fnt");
    this->m_pPageLabel->setScale(.6f);
    this->m_pPageLabel->setPosition(winSize / 2 + this->m_pLrSize / 2 - cocos2d::CCSize { 52.0f, 23.0f });
    this->m_pLayer->addChild(this->m_pPageLabel);

    if (kits->count()) {
        unsigned int maxPage = static_cast<unsigned int>(
            std::floor(kits->count() / static_cast<double>(this->s_nMaxOnPage - 1))
        );

        this->m_pBtnLeft = gd::CCMenuItemSpriteExtra::create(
            cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            this,
            (cocos2d::SEL_MenuHandler)&IconSelectPopup::onPrevPage
        );

        this->m_pBtnLeft->setPosition(- m_pLrSize.width / 2, 0.0f);
        this->m_pButtonMenu->addChild(this->m_pBtnLeft);
        this->m_pBtnLeft->setVisible(false);

        auto rightArrow = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        rightArrow->setFlipX(true);

        this->m_pBtnRight = gd::CCMenuItemSpriteExtra::create(
            rightArrow,
            this,
            (cocos2d::SEL_MenuHandler)&IconSelectPopup::onNextPage
        );

        this->m_pBtnRight->setPosition(m_pLrSize.width / 2, 0.0f);
        this->m_pButtonMenu->addChild(this->m_pBtnRight);
        if (this->m_nCurrentPage == maxPage)
            this->m_pBtnRight->setVisible(false);

        this->showPage(0u);
    } else {
        auto noKitsLabel = cocos2d::CCLabelBMFont::create("You have no saved kits! :(", "goldFont.fnt");

        noKitsLabel->setScale(.8f);
        noKitsLabel->setPosition(winSize.width / 2, winSize.height / 2 - scrollLrOffset / 2);

        this->m_pLayer->addChild(noKitsLabel, 105);
    }

    this->m_pLayer->addChild(this->m_pScrollingLayer);

    auto searchBtn = gd::CCMenuItemSpriteExtra::create(
        cocos2d::CCSprite::createWithSpriteFrameName("gj_findBtn_001.png"),
        this,
        (cocos2d::SEL_MenuHandler)&IconSelectPopup::onSearch
    );
    searchBtn->setPosition(
        - this->m_pLrSize.width / 2 + 60.0f,
        this->m_pLrSize.height / 2 - 25.0f
    );

    this->m_pButtonMenu->addChild(searchBtn, 100);
}

IconSelectPopup* IconSelectPopup::create(GJGarageLayer* gl) {
    auto pRet = new IconSelectPopup();

    if (pRet) {
        pRet->m_pGarage = gl;

        if (pRet->init(420.0f, 270.0f, "GJ_square01.png")) {
            pRet->autorelease();
            return pRet;
        }
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

