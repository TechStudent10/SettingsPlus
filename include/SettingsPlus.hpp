#pragma once

#include <Geode/loader/Dispatch.hpp>
#ifdef MY_MOD_ID
    #undef MY_MOD_ID
#endif

#define MY_MOD_ID "techstudent10.settings_plus"

namespace settings_plus {
    inline void onSettingChanged(std::string_view gv, std::function<void(bool)> callback) GEODE_EVENT_EXPORT_NORES(&onSettingChanged, (gv, callback));
    inline void onFloatSettingChanged(std::string_view gv, std::function<void(float)> callback) GEODE_EVENT_EXPORT_NORES(&onFloatSettingChanged, (gv, callback));
} // namespace settings_plus
