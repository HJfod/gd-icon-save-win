#include "IconKitManager.hpp"

IconKitManager* g_IconKitManager;

bool IconKitManager::init() {
    this->m_sFileName = "CCIconKitManager.dat";

    this->m_vKits = cocos2d::CCArray::create();
    this->m_vKits->retain();

    this->setup();

    return true;
}

void IconKitManager::encodeDataTo(DS_Dictionary* data) {
    data->setIntegerForKey("version", MOD_VERSION_INT);
    data->setIntegerForKey("kitcount", this->m_vKits->count());

    if (!data->stepIntoSubDictWithKey("kits")) {
        data->setSubDictForKey("kits");

        if (!data->stepIntoSubDictWithKey("kits"))
            return;
    }

    for (auto ix = 0u; ix < this->m_vKits->count(); ix++) {
        auto key = std::string("k") + std::to_string(ix);

        data->setSubDictForKey(key.c_str());
        if (!data->stepIntoSubDictWithKey(key.c_str()))
            return;

        auto kit = dynamic_cast<IconKitObject*>(this->m_vKits->objectAtIndex(ix));
        kit->encodeWithCoder(data);

        data->stepOutOfSubDict();
    }

    data->stepOutOfSubDict();
}

void IconKitManager::dataLoaded(DS_Dictionary* data) {
    // if (data->getIntegerForKey("version") > MOD_VERSION_INT) return;
    
    if (!data->stepIntoSubDictWithKey("kits"))
        return;
    
    for (auto key : data->getAllKeys()) {
        if (!data->stepIntoSubDictWithKey(key.c_str()))
            return;
        
        auto obj = IconKitObject::createWithCoder(data);

        // note: is there a memory leak here?
        // iconkitobject doesn't call autorelease...

        this->m_vKits->addObject(obj);

        data->stepOutOfSubDict();
    }

    data->stepOutOfSubDict();
}

void IconKitManager::firstLoad() {
    return;
}

void IconKitManager::addKit(IconKitObject* _obj) {
    this->m_vKits->addObject(_obj);
}

void IconKitManager::removeKit(IconKitObject* _obj) {
    this->m_vKits->removeObject(_obj, true);
}

cocos2d::CCArray* IconKitManager::getKits() {
    return this->m_vKits;
}

IconKitManager* IconKitManager::sharedState() {
    if (g_IconKitManager)
        return g_IconKitManager;
    
    g_IconKitManager = new IconKitManager();
    if (g_IconKitManager && g_IconKitManager->init())
        return g_IconKitManager;
    
    CC_SAFE_DELETE(g_IconKitManager);
    return nullptr;
}

IconKitManager::~IconKitManager() {
    this->m_vKits->release();
}
