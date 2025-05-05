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

    CCSprite* findDifficultyIcon() {
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            if (auto sprite = typeinfo_cast<CCSprite*>(obj)) {
                if (sprite->getPosition() == m_difficultySprite->getPosition() &&
                    sprite->getZOrder() == 3) {
                    return sprite;
                }
            }
        }
        return nullptr;
    }

    void showErrorAlert() {
        auto alert = FLAlertLayer::create(
            "Error",
            "There was a problem loading the demon difficulty face.\n",
            "OK"
        );
        alert->m_scene = this;
        alert->show();
    }

    void replaceDemonIcon(int position) {
        CCSprite* originalIcon = this->findDifficultyIcon();
        if (!originalIcon) {
            this->showErrorAlert();
            return;
        }

        CCSprite* newIcon = ListManager::getSpriteFromPosition(position, true);
        newIcon->setID("grd-difficulty");
        CCPoint iconPos = originalIcon->getPosition();
        this->transferIconChildren(originalIcon, iconPos);

        newIcon->setPosition(iconPos);
        newIcon->setZOrder(originalIcon->getZOrder() + 10);
        originalIcon->setVisible(false);
        this->addChild(newIcon);

        this->createPositionLabel(newIcon, position);
    }

    void transferIconChildren(CCSprite * originalIcon, const CCPoint & position) {
        CCObject* child;
        CCARRAY_FOREACH(originalIcon->getChildren(), child) {
            if (auto sprite = typeinfo_cast<CCSprite*>(child)) {
                if (sprite->getTag() == 69420) {
                    sprite->removeFromParentAndCleanup(true);
                }
                else {
                    sprite->setTag(69420);
                    this->addChild(sprite);
                    sprite->setPosition(position);
                }
            }
        }
    }

    void createPositionLabel(CCSprite * icon, int position) {
        const bool isTopList = (position <= 499);
        const bool showPosition = Mod::get()->getSettingValue<bool>(
            isTopList ? "show-list-position" : "show-extended-list-position"
        );

        if (!showPosition) return;

        auto label = CCLabelBMFont::create(
            fmt::format("#{}", position + 1).c_str(),
            isTopList ? "goldFont.fnt" : "bigFont.fnt"
        );
        label->setScale(isTopList ? 0.4f : 0.3f);
        label->setAnchorPoint({ 0.5f, 0.5f });
        label->setPosition({
            icon->getPositionX() + 26.f,
            icon->getPositionY() - 6.5f
            });
        label->setZOrder(icon->getZOrder() + 1);
        this->addChild(label);
    }

    void applyBackgroundEffects(int position) {
        if (Mod::get()->getSettingValue<bool>("disable-bg")) return;

        CCSprite* icon = this->findDifficultyIcon();
        if (!icon) return;

        CCPoint particlePos = { icon->getPositionX(), icon->getPositionY() + 5.f };

        if (position <= 24) {
            EffectsManager::infernalBackground(this, position);
            this->addChild(ParticleManager::infernalParticles1(50, position == 0));
            this->addChild(ParticleManager::infernalParticles2(50));
        }
        else if (position <= 74) {
            EffectsManager::diabolicalBackground(this, position);
            this->addChild(ParticleManager::diabolicalParticles(50));
        }
        else if (position <= 149) {
            EffectsManager::twistedBackground(this, position);
            this->addChild(ParticleManager::twistedParticles(50));
        }
    }

    void updateDifficultyFace() {
        int position = ListManager::getPositionOfID(m_level->m_levelID);
        if (position == -1) return;

        if (position <= 499) {
            this->replaceDemonIcon(position);
        }
        else if (position <= 1148 && Mod::get()->getSettingValue<bool>("show-extended-list-position")) {
            this->createPositionLabel(this->findDifficultyIcon(), position);
        }

        this->applyBackgroundEffects(position);
        m_fields->m_hasBeenOpened = true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();

        if (!ListManager::demonIDList.empty() &&
            this->m_level->m_stars == 10) {
            this->updateDifficultyFace();
        }
    }
};