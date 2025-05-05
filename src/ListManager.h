#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class ListManager {
public:
    inline static std::vector<int> demonIDList;
    inline static bool firstTimeOpen;
    inline static int filterType;
    inline static bool Searching;

private:
    enum class DemonType {
        Brutal,
        Vicious, 
        Twisted,
        Diabolical,
        Infernal,
        Omega,
        Unknown
    };

    static void addIDFromSubstring(std::string& str) {
        size_t idStart = str.find("_id");
        if (idStart == std::string::npos) return;

        str = str.substr(idStart + 5);
        size_t bracketPos = str.find("}");
        demonIDList.push_back(std::stoi(str.substr(0, bracketPos)));
    }

    static DemonType getDemonTypeForPosition(int pos) {
        if (pos == 0) return DemonType::Omega;
        if (pos <= 24) return DemonType::Infernal;
        if (pos <= 74) return DemonType::Diabolical;
        if (pos <= 149) return DemonType::Twisted;
        if (pos <= 249) return DemonType::Vicious;
        if (pos <= 499) return DemonType::Brutal;
        return DemonType::Unknown;
    }

    static const char* getSpriteName(DemonType type, bool hasText) {
        switch (type) {
            case DemonType::Brutal:    return hasText ? "brutal_demon.png"_spr       : "brutal_demon_icon.png"_spr;
            case DemonType::Vicious:   return hasText ? "vicious_demon.png"_spr      : "vicious_demon_icon.png"_spr;
            case DemonType::Twisted:   return hasText ? "twisted_demon.png"_spr      : "twisted_demon_icon.png"_spr;
            case DemonType::Diabolical:return hasText ? "diabolical_demon.png"_spr   : "diabolical_demon_icon.png"_spr;
            case DemonType::Infernal:  return hasText ? "infernal_demon.png"_spr     : "infernal_demon_icon.png"_spr;
            case DemonType::Omega:     return hasText ? "omega_demon.png"_spr        : "omega_demon_icon.png"_spr;
            default: return "";
        }
    }

public:
    static void parseRequestString(const std::string& str) {
        std::string tempStr = str;
        while (tempStr.find("_id") != std::string::npos) {
            addIDFromSubstring(tempStr);
        }
    }

    static int getPositionOfID(int id) {
        auto it = std::find(demonIDList.begin(), demonIDList.end(), id);
        return it != demonIDList.end() ? std::distance(demonIDList.begin(), it) : -1;
    }

    static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
        DemonType type = getDemonTypeForPosition(pos);
        if (type == DemonType::Unknown) return nullptr;
        const char* frameName = getSpriteName(type, hasText);
        return frameName[0] ? CCSprite::createWithSpriteFrameName(frameName) : nullptr;
    }

    static GJSearchObject* getSearchObject(int upper, int lower) {
        std::stringstream download;
        bool firstEntry = true;

        if (upper == 0 && lower == 0) {
            download << demonIDList.at(0);
        } else {
            for (int i = upper; i > lower; i--) {
                if (!firstEntry) download << ",";
                download << demonIDList.at(i);
                firstEntry = false;
            }
        }

        download << "&gameVersion=22";
        return GJSearchObject::create(SearchType::Type19, download.str());
    }
};

#endif