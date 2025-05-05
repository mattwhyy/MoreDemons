#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelCell.hpp>
#include "ListManager.h"
#include "EffectsManager.h"

using namespace geode::prelude;

class $modify(LevelCell) {

    // use m_mainLayer as parent
    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        if (ListManager::demonIDList.size() == 0) {
            return;
        }

        if (m_level->m_stars != 10) {
            return;
        }

        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);
        if (aredlPos == -1 || aredlPos > 1148) {
            return;
        }

        CCSprite* originalIcon = nullptr;

        CCObject* obj;
        CCARRAY_FOREACH(m_mainLayer->getChildren(), obj) {
            if (CCNode* newObj = dynamic_cast<CCNode*>(obj)) {
                if (newObj->getZOrder() == 2) {
                    newObj->setID("grd-demon-icon-layer");
                    CCObject* obj2;
                    CCARRAY_FOREACH(newObj->getChildren(), obj2) {
                        if (CCSprite* newObj2 = dynamic_cast<CCSprite*>(obj2)) {
                            if (newObj2->getZOrder() == 3) {
                                originalIcon = newObj2;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (originalIcon == nullptr) {
            return;
        }

            // Add "Top X" label if position is valid
            if (aredlPos >= 0 && aredlPos <= 499) {
                CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, false);
                //CCSprite* newIcon = CCSprite::createWithSpriteFrameName("GrD_demon0.png"_spr);
                auto layer = m_mainLayer->getChildByID("grd-demon-icon-layer");

                auto newPos = originalIcon->getPosition();
                newIcon->setPosition(originalIcon->getPosition());
                newIcon->setZOrder(originalIcon->getZOrder() + 25);

                CCObject* clearObj;
                CCARRAY_FOREACH(originalIcon->getChildren(), clearObj) {
                    if (CCSprite* newObj = dynamic_cast<CCSprite*>(clearObj)) {
                        if (newObj->getTag() == 69420) {
                            newObj->removeFromParentAndCleanup(true);
                        }
                    }
                }

                CCObject* iconObj;
                CCARRAY_FOREACH(originalIcon->getChildren(), iconObj) {
                    if (CCSprite* newObj = dynamic_cast<CCSprite*>(iconObj)) {
                        newObj->setTag(69420);
                        layer->addChild(newObj);
                        newObj->setPosition(newPos);
                    }
                }

                originalIcon->setVisible(false);

                layer->addChild(newIcon);
                if (Mod::get()->getSettingValue<bool>("show-list-position")) {
                    // Default Y position of the difficulty icon
                    float defaultPosY = originalIcon->getPositionY();
                    auto label = CCLabelBMFont::create(fmt::format("#{}", aredlPos + 1).c_str(), "goldFont.fnt");
                    label->setScale(0.4f);
                    label->setAnchorPoint({ 0.5f, 0.5f });

                    if (CCNode* difficultyContainer = m_mainLayer->querySelector("grd-demon-icon-layer")) {
                        if (CCNode* coin1 = difficultyContainer->querySelector("coin-icon-1")) {
                            coin1->setPositionY({ coin1->getPositionY() + 3.f });
                            label->setPosition({ newIcon->getPositionX() +26.f, newIcon->getPositionY() -6.5f});
                        }
                        else {
                            label->setPosition({ newIcon->getPositionX() +26.f, newIcon->getPositionY() +2.5f});
                        }
                        if (CCNode* coin2 = difficultyContainer->querySelector("coin-icon-2")) {
                            coin2->setPositionY({ coin2->getPositionY() + 3.f });
                        }
                        if (CCNode* coin3 = difficultyContainer->querySelector("coin-icon-3")) {
                            coin3->setPositionY({ coin3->getPositionY() + 3.f });
                        }
                    }
                    else {
                        geode::log::info("Difficulty container not found!");
                    }

                    // Set the label's Z-order to be one higher than the icon's Z-order
                    label->setZOrder(newIcon->getZOrder() + 1);

                    // Add the label as a child to the current layer
                    this->addChild(label);
                }
        }
        if (Mod::get()->getSettingValue<bool>("show-extended-list-position")) {
            if (aredlPos >= 500 && aredlPos <= 1148) {
                float defaultPosY = originalIcon->getPositionY();
                auto label = CCLabelBMFont::create(fmt::format("#{}", aredlPos + 1).c_str(), "bigFont.fnt");
                label->setScale(0.3f);
                label->setAnchorPoint({ 0.5f, 0.5f });

                if (CCNode* difficultyContainer = m_mainLayer->querySelector("grd-demon-icon-layer")) {
                    if (CCNode* coin1 = difficultyContainer->querySelector("coin-icon-1")) {
                        coin1->setPositionY({ coin1->getPositionY() + 3.f });
                        label->setPosition({ originalIcon->getPositionX() + 26.f, originalIcon->getPositionY() - 6.5f });
                    }
                    else {
                        label->setPosition({ originalIcon->getPositionX() + 26.f, originalIcon->getPositionY() + 2.5f });
                    }
                    if (CCNode* coin2 = difficultyContainer->querySelector("coin-icon-2")) {
                        coin2->setPositionY({ coin2->getPositionY() + 3.f });
                    }
                    if (CCNode* coin3 = difficultyContainer->querySelector("coin-icon-3")) {
                        coin3->setPositionY({ coin3->getPositionY() + 3.f });
                    }
                }
                else {
                    geode::log::info("Difficulty container not found!");
                }
                label->setZOrder(originalIcon->getZOrder() + 1);
                this->addChild(label);
            }
        }
    }
};