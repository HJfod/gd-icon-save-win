#include "IconSelectPopup.hpp"
#include "../managers/IconKitManager.hpp"
#include "IconKitWidget.hpp"
#undef max

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

void IconSelectPopup::setup() {
    // TODO: not write code this horrible lmao
    // (fixes the border color of ScrollingLayer)
    patchBytes(0x2db6f, { 0x6a, 0x1a, 0x6a, 0x29, 0x6a, 0x4c });

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    auto kits = IconKitManager::sharedState()->getKits();
    
    constexpr const float scrollLrOffset = 20.0f;
    float lrWidth = this->m_pLrSize.width - 50.0f;
    float lrHeight = this->m_pLrSize.height - 50.0f - scrollLrOffset;

    auto title = cocos2d::CCLabelBMFont::create("Saved Icon Kits", "bigFont.fnt");

    title->setScale(.75f);
    title->setPosition(winSize.width / 2, winSize.height - 50.0f);

    this->m_pLayer->addChild(title);

    this->m_pScrollingLayer = gd::ScrollingLayer::create(
        { lrWidth, lrHeight },
        { winSize.width / 2, winSize.height / 2 - 10.0f },
        std::max(0.0f, (kits->count() + 1) * IconKitWidget::s_defHeight - lrHeight)
    );
    this->m_pScrollingLayer->setMouseEnabled(true);
    this->m_pScrollingLayer->m_pParent = this->m_pLayer;

    auto w = this->m_pScrollingLayer->m_pScrollLayer->getScaledContentSize().width;
    auto h = IconKitWidget::s_defHeight;

    if (kits->count()) {
        for (auto ix = 0u; ix < kits->count(); ix++) {
            auto kit = dynamic_cast<IconKitObject*>(kits->objectAtIndex(ix));
            auto kitWidget = IconKitWidget::create(kit, lrWidth - 20.0f);

            kitWidget->setPosition(
                lrWidth / 2 - (this->m_pLrSize.height - lrHeight) - 5.0f, lrHeight - h / 2 - (h + 5.0f) * ix
            );
            kitWidget->setParentPopup(this);
            kitWidget->setGarage(this->m_pGarage);

            this->m_pScrollingLayer->m_pScrollLayer->addChild(kitWidget);

            this->registerWithTouchDispatcher();
            cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

            this->setMouseEnabled(true);
            this->setTouchEnabled(true);
        }
    } else {
        auto noKitsLabel = cocos2d::CCLabelBMFont::create("You have no saved kits! :(", "goldFont.fnt");

        noKitsLabel->setScale(.8f);
        noKitsLabel->setPosition(winSize.width / 2, winSize.height / 2 - scrollLrOffset / 2);

        this->m_pLayer->addChild(noKitsLabel, 105);
    }

    this->m_pLayer->addChild(this->m_pScrollingLayer);
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
