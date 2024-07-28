#include <Geode/Geode.hpp>
#include <Geode/modify/OptionsLayer.hpp>

#include "./SettingsLayer.hpp"

using namespace geode::prelude;

class $modify(OptionsLayer) {
	void onOptions(CCObject* sender) {
		SettingsLayer::create()->show();
	}
};
