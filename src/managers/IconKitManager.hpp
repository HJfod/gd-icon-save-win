#pragma once

#include "../offsets.hpp"
#include "IconKitObject.hpp"

class IconKitManager : public gd::GManager {
    cocos2d::CCArray* m_vKits;

    virtual bool init() override;

    virtual void encodeDataTo(DS_Dictionary* data) override;
    virtual void dataLoaded(DS_Dictionary* data) override;
    virtual void firstLoad() override;

    public:
        static IconKitManager* sharedState();

        void addKit(IconKitObject*);
        void removeKit(IconKitObject*);
        cocos2d::CCArray* getKits();

        ~IconKitManager();
};
