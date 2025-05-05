#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelCell.hpp>
#include "ListManager.h"
#include "EffectsManager.h"

using namespace geode::prelude;

class $modify(CustomLevelCell, LevelCell) {

    CCSprite* findDifficultyIcon() {
        CCObject* layerObj;
        CCARRAY_FOREACH(m_mainLayer->getChildren(), layerObj) {
            if (auto layer = typeinfo_cast<CCNode*>(layerObj)) {
                if (layer->getZOrder() == 2) {
                    layer->setID("difficulty-icon-layer");
                    CCObject* iconObj;
                    CCARRAY_FOREACH(layer->getChildren(), iconObj) {
                        if (auto sprite = typeinfo_cast<CCSprite*>(iconObj)) {
                            if (sprite->getZOrder() == 3) {
                                return sprite;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    void handleTopDemon(CCSprite * originalIcon, int position) {
        CCSprite* newIcon = ListManager::getSpriteFromPosition(position, false);
        auto layer = m_mainLayer->getChildByID("difficulty-icon-layer");
        const auto iconPos = originalIcon->getPosition();
        CCObject* childObj;
        CCARRAY_FOREACH(originalIcon->getChildren(), childObj) {
            if (auto sprite = typeinfo_cast<CCSprite*>(childObj)) {
                sprite->setTag(69420);
                layer->addChild(sprite);
                sprite->setPosition(iconPos);
            }
        }

        originalIcon->setVisible(false);
        newIcon->setPosition(iconPos);
        newIcon->setZOrder(originalIcon->getZOrder() + 25);
        layer->addChild(newIcon);

        if (Mod::get()->getSettingValue<bool>("show-list-position")) {
            auto label = CCLabelBMFont::create(fmt::format("#{}", position + 1).c_str(), "goldFont.fnt");
            label->setScale(0.4f);
            label->setAnchorPoint({ 0.5f, 0.5f });

            if (auto container = m_mainLayer->getChildByID("difficulty-icon-layer")) {
                if (container->getChildByID("coin-icon-1")) {
                    container->getChildByID("coin-icon-1")->setPositionY(container->getChildByID("coin-icon-1")->getPositionY() + 3.f);
                    label->setPosition({ newIcon->getPositionX() + 26.f, newIcon->getPositionY() - 6.5f });
                }
                else {
                    label->setPosition({ newIcon->getPositionX() + 26.f, newIcon->getPositionY() + 2.5f });
                }
                if (auto coin2 = container->getChildByID("coin-icon-2")) {
                    coin2->setPositionY(coin2->getPositionY() + 3.f);
                }
                if (auto coin3 = container->getChildByID("coin-icon-3")) {
                    coin3->setPositionY(coin3->getPositionY() + 3.f);
                }
            }
            label->setZOrder(newIcon->getZOrder() + 1);
            this->addChild(label);
        }
    }

    void handleExtendedPosition(CCSprite * originalIcon, int position) {
        if (Mod::get()->getSettingValue<bool>("show-extended-list-position")) {
            auto label = CCLabelBMFont::create(fmt::format("#{}", position + 1).c_str(), "bigFont.fnt");
            label->setScale(0.3f);
            label->setAnchorPoint({ 0.5f, 0.5f });

            if (auto container = m_mainLayer->getChildByID("difficulty-icon-layer")) {
                if (container->getChildByID("coin-icon-1")) {
                    container->getChildByID("coin-icon-1")->setPositionY(container->getChildByID("coin-icon-1")->getPositionY() + 3.f);
                    label->setPosition({ originalIcon->getPositionX() + 26.f, originalIcon->getPositionY() - 6.5f });
                }
                else {
                    label->setPosition({ originalIcon->getPositionX() + 26.f, originalIcon->getPositionY() + 2.5f });
                }
                if (auto coin2 = container->getChildByID("coin-icon-2")) {
                    coin2->setPositionY(coin2->getPositionY() + 3.f);
                }
                if (auto coin3 = container->getChildByID("coin-icon-3")) {
                    coin3->setPositionY(coin3->getPositionY() + 3.f);
                }
            }
            label->setZOrder(originalIcon->getZOrder() + 1);
            this->addChild(label);
        }
    }

    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        if (ListManager::demonIDList.empty() || m_level->m_stars != 10) {
            return;
        }

        int position = ListManager::getPositionOfID(m_level->m_levelID);
        if (position == -1 || position > 1148) {
            return;
        }

        CCSprite* originalIcon = this->findDifficultyIcon();
        if (!originalIcon) return;

        if (position <= 499) {
            this->handleTopDemon(originalIcon, position);
        }
        else {
            this->handleExtendedPosition(originalIcon, position);
        }
    }
};