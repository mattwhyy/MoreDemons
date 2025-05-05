#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <vector>
#include <string>
#include "ListManager.h"
#include "EffectsManager.h"
#include "ParticleManager.h"

using namespace geode::prelude;

class $modify(MDInfoLayer, LevelInfoLayer) {

    struct Fields {
        bool m_hasBeenOpened = false;
    };
    
    void updateDifficultyFace() {

        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);
        if (aredlPos == -1) {
            return;
        }

        // Get the original difficulty icon
        CCSprite* originalIcon = nullptr;
        bool iconFound = false;

        // Iterate through every object that is a direct child of the layer to find the difficulty face.
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            if (CCSprite* newObj = dynamic_cast<CCSprite*>(obj)) {
                if (newObj->getPosition() == m_difficultySprite->getPosition() && newObj->getZOrder() == 3) {
                    originalIcon = newObj;
                    iconFound = true;
                    break;
                }
            }
        }

        if (originalIcon == nullptr || !iconFound) {
            auto alert = FLAlertLayer::create("Error", "There was a problem loading the demon difficulty face.\nYour screen resolution may not be supported.\n\n<cb>-Grandpa Demon</c>", "OK");
            alert->m_scene = this;
            alert->show();
            return;
        }

        // Add custom sprite and effects only for positions 0–499
        if (aredlPos <= 499) {
            CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, true);
            newIcon->setID("MD-difficulty");

            auto newPos = originalIcon->getPosition();
            newIcon->setPosition(newPos);
            newIcon->setZOrder(originalIcon->getZOrder() + 10);

            // Clear any extra child objects from the original icon
            CCObject* clearObj;
            CCARRAY_FOREACH(originalIcon->getChildren(), clearObj) {
                if (CCSprite* newObj = dynamic_cast<CCSprite*>(clearObj)) {
                    if (newObj->getTag() == 69420) {
                        newObj->removeFromParentAndCleanup(true);
                    }
                }
            }

            // Transfer children to the new icon
            CCObject* iconObj;
            CCARRAY_FOREACH(originalIcon->getChildren(), iconObj) {
                if (CCSprite* newObj = dynamic_cast<CCSprite*>(iconObj)) {
                    newObj->setTag(69420);
                    this->addChild(newObj);
                    newObj->setPosition(newPos);
                }
            }

            originalIcon->setVisible(false);
            this->addChild(newIcon);

            // Add effects for positions 0–499
            if (Mod::get()->getSettingValue<bool>("show-list-position")) {
                if (aredlPos >= 0 && aredlPos <= 499) {
                    float defaultPosY = originalIcon->getPositionY();
                    auto label = CCLabelBMFont::create(fmt::format("#{}", aredlPos + 1).c_str(), "goldFont.fnt");
                    label->setScale(0.4f);
                    label->setAnchorPoint({ 0.5f, 0.5f });
                    label->setPosition({ newIcon->getPositionX() + 26.f, newIcon->getPositionY() - 6.5f });
                    label->setZOrder(newIcon->getZOrder() + 1);
                    this->addChild(label);
                }
            }
        }

        // Always show white label for positions 500–1148
        if (aredlPos >= 500 && aredlPos <= 1148) {
            if (Mod::get()->getSettingValue<bool>("show-extended-list-position")) {
                float defaultPosY = originalIcon->getPositionY();
                auto label = CCLabelBMFont::create(fmt::format("#{}", aredlPos + 1).c_str(), "bigFont.fnt");
                label->setScale(0.3f);
                label->setAnchorPoint({ 0.5f, 0.5f });
                label->setPosition({ originalIcon->getPositionX() + 26.f, originalIcon->getPositionY() - 6.5f });
                label->setZOrder(originalIcon->getZOrder() + 1);
                this->addChild(label);
            }
        }

        // Handling background effects
        if (aredlPos <= 24) {
            EffectsManager::infinityBackground(this, aredlPos);
            if (!Mod::get()->getSettingValue<bool>("disable-bg")) {
                bool isGrandpa = (aredlPos == 0);
                auto particle1 = ParticleManager::infiniteParticles1(50, isGrandpa);
                particle1->setPosition({ originalIcon->getPositionX(), originalIcon->getPositionY() + 5.f });
                this->addChild(particle1);

                auto particle2 = ParticleManager::infiniteParticles2(50);
                particle2->setPosition({ originalIcon->getPositionX(), originalIcon->getPositionY() + 5.f });
                this->addChild(particle2);
            }
        }

        if (aredlPos <= 74 && aredlPos > 24) {
            EffectsManager::mythicalBackground(this, aredlPos);
            if (!Mod::get()->getSettingValue<bool>("disable-bg")) {
                auto particle = ParticleManager::mythicalParticles(50);
                particle->setPosition({ originalIcon->getPositionX(), originalIcon->getPositionY() + 5.f });
                this->addChild(particle);
            }
        }

        if (aredlPos <= 149 && aredlPos > 74) {
            EffectsManager::legendaryBackground(this, aredlPos);
            if (!Mod::get()->getSettingValue<bool>("disable-bg")) {
                auto particle = ParticleManager::legendaryParticles(50);
                particle->setPosition({ originalIcon->getPositionX(), originalIcon->getPositionY() + 5.f });
                this->addChild(particle);
            }
        }

        m_fields->m_hasBeenOpened = true;
        return;
    }


    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        if (ListManager::demonIDList.size() == 0) {
            return;
        }

        if (this->m_level->m_stars != 10) {
            return;
        }
        
        updateDifficultyFace();
    }
};