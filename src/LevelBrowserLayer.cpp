#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "ListManager.h"

class $modify(MDLevelBrowserLayer, LevelBrowserLayer) {

    struct Fields {
        int m_currentPage;
        int m_furthestLoadedPage;
        int m_lowIdx;
    };

    bool init(GJSearchObject * p0) {
        if (shouldUseDefaultBehavior(p0)) {
            return LevelBrowserLayer::init(p0);
        }

        setupCustomInitialState();
        return LevelBrowserLayer::init(ListManager::getSearchObject(499, 489));
    }

    void loadLevelsFinished(CCArray * p0, char const* p1, int p2) {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);

        if (!shouldProcessCustomBehavior()) return;

        updateNavigationUI();
        validatePageBounds();
    }

    void onNextPage(CCObject * sender) {
        LevelBrowserLayer::onNextPage(sender);
        if (!shouldProcessCustomBehavior()) return;

        handlePageNavigation(true);
    }

    void onPrevPage(CCObject * sender) {
        LevelBrowserLayer::onPrevPage(sender);
        if (!shouldProcessCustomBehavior()) return;

        handlePageNavigation(false);
    }

private:
    bool shouldUseDefaultBehavior(GJSearchObject * obj) const {
        return !ListManager::Searching ||
            obj->m_searchType != SearchType::Type19;
    }

    bool shouldProcessCustomBehavior() const {
        return ListManager::Searching &&
            this->m_searchObject->m_searchType == SearchType::Type19;
    }

    void setupCustomInitialState() {
        this->m_fields->m_currentPage = 0;
        this->m_fields->m_lowIdx = 0;
    }

    void updateNavigationUI() {
        this->m_leftArrow->setVisible(this->m_fields->m_currentPage > 0);
        this->m_rightArrow->setVisible(this->m_fields->m_currentPage < 24);
        this->m_pageBtn->setVisible(false);

        updateCountText();
    }

    void updateCountText() {
        int start = this->m_fields->m_currentPage * 10 + 1;
        this->m_countText->setString(
            fmt::format("{} to {} of 250", start, start + 9).c_str()
        );
    }

    void validatePageBounds() {
        if (this->m_fields->m_currentPage <= 0) {
            this->m_leftArrow->setVisible(false);
        }
        else if (this->m_fields->m_currentPage >= 24) {
            this->m_rightArrow->setVisible(false);
        }
    }

    void handlePageNavigation(bool forward) {
        if (forward) {
            if (this->m_fields->m_currentPage < 24) {
                this->m_fields->m_currentPage++;
            }
        }
        else {
            if (this->m_fields->m_currentPage > 0) {
                this->m_fields->m_currentPage--;
            }
        }

        reloadCurrentPage();
    }

    void reloadCurrentPage() {
        const int offset = this->m_fields->m_currentPage * 10;
        LevelBrowserLayer::loadPage(
            ListManager::getSearchObject(499 - offset, 489 - offset)
        );
        updateNavigationUI();
    }
};