#include <Geode/Bindings.hpp>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include "ListManager.h"

class $modify(MDDemonFilterSelectLayer, DemonFilterSelectLayer) {

    struct Fields {
        CCSprite* m_demon0Spr = nullptr;
        CCSprite* m_demon1Spr = nullptr;
        CCSprite* m_demon2Spr = nullptr;
        CCSprite* m_demon3Spr = nullptr;
        CCSprite* m_demon4Spr = nullptr;
        CCSprite* m_demon5Spr = nullptr;
    };


    static void onModify(auto & self) {
        self.setHookPriority("DemonFilterSelectLayer::init", -1);
    }

    bool init() {
        DemonFilterSelectLayer::init();

        if (ListManager::demonIDList.empty()) {
            return true;
        }

        handleTouchPriority(this);
        CCLayer* layer = nullptr;

        CCObject* layerObj;
        CCARRAY_FOREACH(this->getChildren(), layerObj) {
            if (auto newObj = static_cast<CCLayer*>(layerObj)) {
                layer = newObj;
            }
        }

        CCScale9Sprite* s9spr = nullptr;
        CCLabelBMFont* label = nullptr;
        CCMenu* menu = nullptr;

        CCObject* obj;
        CCARRAY_FOREACH(layer->getChildren(), obj) {
            if (auto newObj = dynamic_cast<CCScale9Sprite*>(obj)) {
                s9spr = newObj;
            }
            else if (auto newObj2 = dynamic_cast<CCLabelBMFont*>(obj)) {
                label = newObj2;
            }
            else if (auto newObj3 = dynamic_cast<CCMenu*>(obj)) {
                menu = newObj3;
            }
        }

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("resources/difficulty_sprites/spritesheet.plist");

        CCMenuItemSpriteExtra* okButton = nullptr;
        CCObject* obj_ok;
        CCARRAY_FOREACH(menu->getChildren(), obj_ok) {
            if (auto newObj = dynamic_cast<CCMenuItemSpriteExtra*>(obj_ok)) {
                if (newObj->getPositionY() < 0) {
                    okButton = newObj;
                    break;
                }
            }
        }

        auto csize = s9spr->getContentSize();
        s9spr->setContentSize({ csize.width, csize.height * 1.1f });
        auto newcsize = s9spr->getContentSize();
        label->setPositionY(newcsize.height + 45.f);
        label->setScale(0.8f);
        menu->setPositionY(newcsize.height);
        okButton->setPositionY(-110.f);

        handleTouchPriority(this);
        auto newMenu = CCMenu::create();
        layer->addChild(newMenu);
        newMenu->setPosition({ layer->getContentSize().width / 2 - 3, layer->getContentSize().height - 180.f });
        newMenu->setLayout(RowLayout::create()
            ->setGap(-0.1f)
            ->setAxisAlignment(AxisAlignment::Center));

        auto demon0Spr = CCSprite::createWithSpriteFrameName("brutal_demon.png"_spr);
        this->m_fields->m_demon0Spr = demon0Spr;
        auto demon0Btn = CCMenuItemSpriteExtra::create(demon0Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton0));
        newMenu->addChild(demon0Btn);

        auto demon1Spr = CCSprite::createWithSpriteFrameName("vicious_demon.png"_spr);
        this->m_fields->m_demon1Spr = demon1Spr;
        auto demon1Btn = CCMenuItemSpriteExtra::create(demon1Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton1));
        newMenu->addChild(demon1Btn);

        auto demon2Spr = CCSprite::createWithSpriteFrameName("twisted_demon.png"_spr);
        this->m_fields->m_demon2Spr = demon2Spr;
        auto demon2Btn = CCMenuItemSpriteExtra::create(demon2Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton2));
        newMenu->addChild(demon2Btn);

        auto demon3Spr = CCSprite::createWithSpriteFrameName("diabolical_demon.png"_spr);
        this->m_fields->m_demon3Spr = demon3Spr;
        auto demon3Btn = CCMenuItemSpriteExtra::create(demon3Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton3));
        newMenu->addChild(demon3Btn);

        auto demon4Spr = CCSprite::createWithSpriteFrameName("infernal_demon.png"_spr);
        this->m_fields->m_demon4Spr = demon4Spr;
        auto demon4Btn = CCMenuItemSpriteExtra::create(demon4Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton4));
        newMenu->addChild(demon4Btn);

        auto demon5Spr = CCSprite::createWithSpriteFrameName("omega_demon.png"_spr);
        this->m_fields->m_demon5Spr = demon5Spr;
        auto demon5Btn = CCMenuItemSpriteExtra::create(demon5Spr, this, menu_selector(MDDemonFilterSelectLayer::onButton5));
        newMenu->addChild(demon5Btn);

        newMenu->updateLayout();

        return true;
    }

    void onButton0(CCObject * sender) {
        ListManager::filterType = 0;
        ListManager::Searching = true;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(349, 249));
        geode::cocos::switchToScene(browserLayer);
    }
    void onButton1(CCObject * sender) {
        ListManager::filterType = 1;
        ListManager::Searching = false;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(249, 149));
        geode::cocos::switchToScene(browserLayer);
    }
    void onButton2(CCObject * sender) {
        ListManager::filterType = 2;
        ListManager::Searching = false;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(149, 74));
        geode::cocos::switchToScene(browserLayer);
    }
    void onButton3(CCObject * sender) {
        ListManager::filterType = 3;
        ListManager::Searching = false;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(74, 24));
        geode::cocos::switchToScene(browserLayer);
    }
    void onButton4(CCObject * sender) {
        ListManager::filterType = 4;
        ListManager::Searching = false;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(24, 0));
        geode::cocos::switchToScene(browserLayer);
    }
    void onButton5(CCObject * sender) {
        ListManager::filterType = 5;
        ListManager::Searching = false;
        auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(0, 0));
        geode::cocos::switchToScene(browserLayer);
    }
};
