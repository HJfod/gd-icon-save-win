#include "ProfilePage.hpp"
#include "../managers/IconKitManager.hpp"

void ProfilePage::onSaveIcons(cocos2d::CCObject* pSender) {
    auto icon = new IconKitObject();
    icon->setCubeID(this->m_pScore->getPlayerCube());
    icon->setShipID(this->m_pScore->getPlayerShip());
    icon->setBallID(this->m_pScore->getPlayerBall());
    icon->setUfoID(this->m_pScore->getPlayerUfo());
    icon->setWaveID(this->m_pScore->getPlayerWave());
    icon->setRobotID(this->m_pScore->getPlayerRobot());
    icon->setSpiderID(this->m_pScore->getPlayerSpider());
    icon->setStreakID(this->m_pScore->getPlayerStreak());
    icon->setDeathID(this->m_pScore->getPlayerExplosion());
    icon->setPlayerFrame(this->m_pScore->getIconType());
    icon->setGlowEnabled(this->m_pScore->getGlowEnabled());
    icon->setColor1(this->m_pScore->getPlayerColor1());
    icon->setColor2(this->m_pScore->getPlayerColor2());

    auto im = IconKitManager::sharedState();

    if (im) {
        im->addKit(icon);

        auto popup = gd::TextAlertPopup::create("Icon Kit Added!", .5f, .6f);
        this->addChild(popup, 100);
    }
}

bool __fastcall ProfilePage::initHook(ProfilePage* _self, edx_t, int _dunno, bool _dunno_2) {
    if (!init(_self, _dunno, _dunno_2))
        return false;

    auto spr_download = cocos2d::CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
    spr_download->setScale(.8f);

    auto downloadButton = gd::CCMenuItemSpriteExtra::create(
        spr_download, _self, (cocos2d::SEL_MenuHandler)&ProfilePage::onSaveIcons
    );

    downloadButton->setPosition(
        15.0f,
        -95.0f
    );

    _self->m_pButtonMenu->addChild(downloadButton);
    
    return true;
}