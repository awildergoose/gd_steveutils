#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/binding/GJSearchObject.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LeaderboardsLayer.hpp>
#include <Geode/modify/GauntletSelectLayer.hpp>

using namespace geode::prelude;
using namespace keybinds;

// #define dbg

#ifdef dbg
class $modify(LevelBrowserLayer) {
	bool init(GJSearchObject* p0) {
		if(!LevelBrowserLayer::init(p0))
			return false;
		
		log::info("LevelBrowserLayer.key = {};", p0->getKey());

		return true;
	}
};
#endif

char const* savedLevelsKey = "99__-_-_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0";
char const* creatorLevelsKey = "98__-_-_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0_0";

void switchScene(cocos2d::CCLayer* newLayer) {
	CCDirector::sharedDirector()->pushScene(
		CCTransitionFade::create(0.5f,
			cocos::switchToScene(newLayer)
		)
	);
}

void applyShortcuts(CCLayer* layer) {
	// quick nav
	layer->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
		if (event->isDown())
			switchScene(LevelSearchLayer::create(0));
		return ListenerResult::Propagate;
	}, "steve.steveutils/gosearch");

	layer->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
		if (event->isDown())
			switchScene(LevelBrowserLayer::create(GJSearchObject::createFromKey(savedLevelsKey)));
		return ListenerResult::Propagate;
	}, "steve.steveutils/gosaved");

	layer->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
		if (event->isDown())
			switchScene(LevelBrowserLayer::create(GJSearchObject::createFromKey(creatorLevelsKey)));
		return ListenerResult::Propagate;
	}, "steve.steveutils/gocreator");
}

// Do not question.
#define shortcut_hook(layer, args, args2) class $modify(layer) { bool init args { if(!layer::init args2) return false; applyShortcuts(this); return true; } }

shortcut_hook(MenuLayer, (), ());
shortcut_hook(LevelBrowserLayer, (GJSearchObject* p0), (p0));
shortcut_hook(LevelInfoLayer, (GJGameLevel* p0, bool p1), (p0, p1));
shortcut_hook(GJGarageLayer, (), ());
shortcut_hook(GJShopLayer, (ShopType p0), (p0));
shortcut_hook(LevelSelectLayer, (int p0), (p0));
shortcut_hook(CreatorLayer, (), ());
shortcut_hook(LeaderboardsLayer, (LeaderboardState p0), (p0));
shortcut_hook(GauntletSelectLayer, (int p0), (p0));
shortcut_hook(LevelSearchLayer, (int p0), (p0));

$execute {
	BindManager::get()->registerBindable({
        "steve.steveutils/gosearch",
        "Go to search",
        "Quickly navigate to the search menu",
        { Keybind::create(KEY_F, Modifier::Shift) },
        "Quick Navigation"
	});

	BindManager::get()->registerBindable({
        "steve.steveutils/gosaved",
        "Go to saved levels",
        "Quickly navigate to your saved levels",
        { Keybind::create(KEY_S, Modifier::Shift) },
        "Quick Navigation"
	});

	BindManager::get()->registerBindable({
        "steve.steveutils/gocreator",
        "Go to my levels",
        "Quickly navigate to your levels (editor)",
        { Keybind::create(KEY_C, Modifier::Shift) },
        "Quick Navigation"
	});
}
