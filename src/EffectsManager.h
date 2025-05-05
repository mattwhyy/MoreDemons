#ifndef EFFECTSMANAGER_H
#define EFFECTSMANAGER_H

#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

class EffectsManager {
public:
    inline static void infernalBackground(LevelInfoLayer* layer, int aredlPos) {
        if (!shouldProcessBackground(layer, "grd-infinity")) return;

        CCSprite* bg = locateMainBackground(layer);
        if (!bg) return;

        configureBaseBackground(bg, { 75, 75, 0 }, -10);
        processChildElements(layer);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        ccBlendFunc blending = { GL_ONE, GL_ONE };

        CCSprite* bg1 = createAnimatedBackground(
            layer,
            "background_gradient.png"_spr,
            blending,
            { 100, 100, 100 },
            winSize,
            1.0f,
            -5,
            20,
            -0.15f,
            50.f,
            50.f,
            1.0f
        );

        CCSprite* bg2 = createAnimatedBackground(
            layer,
            "background_gradient.png"_spr,
            blending,
            { 100, 100, 100 },
            winSize,
            2.0f,
            -4,
            30,
            -0.75f,
            50.f,
            50.f,
            1.5f
        );

        if (aredlPos == 0) {
            bg1->setColor({ 0, 0, 0 });
            bg2->setColor({ 95, 0, 0 });
            bg->setColor({ 22, 22, 22 });
        }
    }

    inline static void diabolicalBackground(LevelInfoLayer* layer, int aredlPos) {
        if (!shouldProcessBackground(layer)) return;

        CCSprite* bg = locateMainBackground(layer);
        if (!bg) return;

        configureBaseBackground(bg, { 65, 41, 142 }, -10);
        processChildElements(layer);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        ccBlendFunc blending = { GL_ONE, GL_ONE };

        createAnimatedBackground(
            layer,
            "background_gradient.png"_spr,
            blending,
            { 76, 63, 118 },
            winSize,
            1.0f,
            -5,
            20,
            -0.35f,
            50.f,
            130.f,
            1.0f
        );
    }

    inline static void twistedBackground(LevelInfoLayer* layer, int aredlPos) {
        if (!shouldProcessBackground(layer)) return;

        CCSprite* bg = locateMainBackground(layer);
        if (!bg) return;

        configureBaseBackground(bg, { 125, 0, 200 }, -10);
        processChildElements(layer);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        ccBlendFunc blending = { GL_ONE, GL_ONE };

        createAnimatedBackground(
            layer,
            "background_gradient.png"_spr,
            blending,
            { 50, 0, 180 },
            winSize,
            1.0f,
            -5,
            20,
            -0.25f,
            40.f,
            100.f,
            1.0f
        );
    }

private:
    static bool shouldProcessBackground(LevelInfoLayer* layer, const char* id = nullptr) {
        if (Mod::get()->getSettingValue<bool>("disable-bg")) return false;
        if (id && layer->getChildByID(id)) return false;
        return true;
    }

    static CCSprite* locateMainBackground(LevelInfoLayer* layer) {
        const float tolerance = 5.f;
        const auto winWidth = CCDirector::sharedDirector()->getWinSize().width;

        CCObject* obj;
        CCARRAY_FOREACH(layer->getChildren(), obj) {
            if (auto spr = dynamic_cast<CCSprite*>(obj)) {
                if (spr->getScaledContentSize().width >= winWidth - tolerance) {
                    return spr;
                }
            }
        }
        return nullptr;
    }

    static void configureBaseBackground(CCSprite* bg, ccColor3B color, int zOrder) {
        bg->setZOrder(zOrder);
        bg->setColor(color);
    }

    static void processChildElements(LevelInfoLayer* layer) {
        CCObject* rObj;
        CCARRAY_FOREACH(layer->getChildren(), rObj) {
            if (auto spr = dynamic_cast<CCSprite*>(rObj)) {
                if (spr->getPositionY() < 1) {
                    spr->setOpacity(50);
                }
            }
        }
    }

    static CCSprite* createAnimatedBackground(
        LevelInfoLayer* layer,
        const char* spriteFrame,
        ccBlendFunc blending,
        ccColor3B color,
        CCSize winSize,
        float scaleFactor,
        int zOrder,
        float moveDuration,
        float yOffset,
        float maxAlpha,
        float offsetAlpha,
        float fadeDuration
    ) {
        CCSprite* bg = CCSprite::create(spriteFrame);
        bg->setBlendFunc(blending);
        float scale = winSize.width / bg->getContentSize().width * scaleFactor;

        bg->setOpacity(0);
        bg->setScale(scale);
        bg->setColor(color);
        layer->addChild(bg);

        bg->setAnchorPoint({ 0.5, 0 });
        bg->setPosition({ winSize.width / 2, 0 });
        bg->setZOrder(zOrder);

        float height = bg->getScaledContentSize().height;
        auto moveDown = CCMoveTo::create(moveDuration, { winSize.width / 2, height * yOffset });
        auto moveUp = CCMoveTo::create(0, { winSize.width / 2, 0 });
        bg->runAction(CCRepeatForever::create(CCSequence::create(moveDown, moveUp, nullptr)));

        CCArray* fadeActions = CCArray::create();
        for (int i = 0; i < 19; i++) {
            fadeActions->addObject(CCFadeTo::create(
                fadeDuration,
                CCRANDOM_MINUS1_1() * maxAlpha + offsetAlpha
            ));
        }
        fadeActions->addObject(CCFadeTo::create(fadeDuration, 0.f));
        bg->runAction(CCRepeatForever::create(CCSequence::create(fadeActions)));

        return bg;
    }
};

#endif