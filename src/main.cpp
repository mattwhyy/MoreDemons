#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>
#include "ListManager.h"
#include <string>

using namespace geode::prelude;

class $modify(CustomMenuLayer, MenuLayer) {

    struct Fields {
        EventListener<web::WebTask> m_listener;
    };

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        if (ListManager::firstTimeOpen) {
            return true;
        }

        this->setupDataFetch();
        return true;
    }

private:
    void setupDataFetch() {
        m_fields->m_listener.bind([this](web::WebTask::Event* event) {
            this->handleAPIResponse(event);
            });

        auto request = web::WebRequest();
        m_fields->m_listener.setFilter(
            request.get("https://api.aredl.net/api/aredl/list")
        );
    }

    void handleAPIResponse(web::WebTask::Event * event) {
        if (auto response = event->getValue()) {
            auto responseString = response->string().unwrapOr("RequestFailed");

            if (this->isInvalidResponse(response, responseString)) {
                this->showConnectionError(responseString);
                return;
            }

            this->processValidResponse(responseString);
        }
    }

    bool isInvalidResponse(web::WebResponse * response, const std::string & content) {
        return response->code() != 200 ||
            content == "RequestFailed" ||
            content == "-1";
    }

    void showConnectionError(const std::string & serverMessage) {
        ListManager::firstTimeOpen = true;
        ListManager::filterType = -2;

        FLAlertLayer::create(
            "Connection Issues",
            fmt::format(
                "<cj>Failed to retrieve demon list data.</c>\n"
                "The AREDL servers might be temporarily unavailable, "
                "or you might have network connectivity problems.\n"
                "<co>Please try restarting Geometry Dash.</c>\n\n"
                "<cg>Server response: {}</c>",
                serverMessage
            ),
            "OK"
        )->show();
    }

    void processValidResponse(const std::string & data) {
        ListManager::parseRequestString(data);
        ListManager::firstTimeOpen = true;
        ListManager::filterType = -1;
    }
};