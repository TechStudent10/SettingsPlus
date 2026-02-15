#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/SettingsPlus.hpp"
#include "API.hpp"

void callCallbacks(std::string_view gv, bool newValue) {
    for (auto cb : settingCallbacks[gv]) {
        cb(newValue);
    }
}

void callCallbacks(std::string_view gv, float newValue) {
    for (auto cb : floatSettingCallbacks[gv]) {
        cb(newValue);
    }
}

namespace settings_plus {

void onSettingChanged(std::string_view gv, std::function<void(bool)> callback) {
    if (!settingCallbacks.contains(gv)) {
        settingCallbacks[gv] = {};
    }

    settingCallbacks[gv].push_back(callback);
}

void onFloatSettingChanged(std::string_view gv, std::function<void(float)> callback) {
    if (!floatSettingCallbacks.contains(gv)) {
        floatSettingCallbacks[gv] = {};
    }

    floatSettingCallbacks[gv].push_back(callback);
}

}
