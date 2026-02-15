#pragma once

static std::unordered_map<std::string_view, std::vector<std::function<void(bool)>>> settingCallbacks;
static std::unordered_map<std::string_view, std::vector<std::function<void(float)>>> floatSettingCallbacks;

void callCallbacks(std::string_view gv, bool);
void callCallbacks(std::string_view gv, float);
