#ifndef LISTMANAGER_H
#define LISTMANAGER_H
#include <vector>
#include <string>

using namespace geode::prelude;

class ListManager {
    public:
        inline static std::vector<int> demonIDList;
        inline static bool firstTimeOpen;
        inline static int filterType;
        inline static bool isSupremeSearching;

        inline static void parseRequestString(std::string str) {
            size_t isFound = str.find("_id");

            while (isFound != std::string::npos) {
                str = str.substr(isFound + 5);
                size_t findBracket = str.find("}");

                int id = stoi(str.substr(0, findBracket));
                demonIDList.push_back(id);

                isFound = str.find("_id");
            }
        }

        inline static int getPositionOfID(int id) {
            for (unsigned int i = 0; i < demonIDList.size(); i++) {
                if (demonIDList.at(i) == id) return i;
            }

            return -1;
        }

        inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
            if (pos <= 499 &&
                pos > 249) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("brutal_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("brutal_demon_icon.png"_spr);
                    }
                }

            if (pos <= 249 &&
                pos > 149) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("vicious_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("vicious_demon_icon.png"_spr);
                    }
                }

            if (pos <= 149 &&
                pos > 74) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("twisted_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("twisted_demon_icon.png"_spr);
                    }
                }
            if (pos <= 74 &&
                pos > 24) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("diabolical_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("diabolical_demon_icon.png"_spr);
                    }
                }
            if (pos <= 24 &&
                pos > 0) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("infernal_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("infernal_demon_icon.png"_spr);
                    }
                }
            if (pos == 0) {
                    if (hasText) {
                        return CCSprite::createWithSpriteFrameName("omega_demon.png"_spr);
                    } else {
                        return CCSprite::createWithSpriteFrameName("omega_demon_icon.png"_spr);
                    }
                } 
        }

        inline static GJSearchObject* getSearchObject(int upper, int lower) {
            std::stringstream download;
            bool first = true;
            if (!(upper == 0 && lower == 0)) {
                for (unsigned int i = upper; i > lower; i--) {
                    if (!first) {
                        download << ",";
                    }
                    download << std::to_string(ListManager::demonIDList.at(i));
                    first = false;
                }
            } else {
                download << std::to_string(ListManager::demonIDList.at(0));
            }
            
            download << "&gameVersion=22";
            GJSearchObject* searchObj = GJSearchObject::create(SearchType::Type19, download.str());
            return searchObj;
        }   

        
};

#endif