#include "./SettingsLayer.hpp"

SearchPopup* SearchPopup::create(SearchCB callback) {
    auto ret = new SearchPopup();
    if (ret && ret->initAnchored(210.f, 130.f, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SearchPopup::setup(SearchCB callback) {
    m_callback = callback;

    this->setTitle("Search");

    m_input = TextInput::create(150.f, "Query here");
    m_mainLayer->addChildAtPosition(m_input, Anchor::Center, {0, 7.5f});

    auto btn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Search"), this, menu_selector(SearchPopup::onSearch)
    );
    auto menu = CCMenu::create();
    menu->addChild(btn);

    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, {0, 25});

    return true;
}

void SearchPopup::onSearch(CCObject* sender) {
    m_callback(
        m_input->getString()
    );
    m_closeBtn->activate();
}

SettingCell* SettingCell::create(std::string name, std::string gv, SettingCellType type) {
    auto ret = new SettingCell();
    if (ret && ret->init(name, gv, type)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SettingCell::init(std::string name, std::string gv, SettingCellType type) {
    m_name = name;
    m_gameVariable = gv;
    m_type = type;

    auto nameLabel = CCLabelBMFont::create(
        name.c_str(), "bigFont.fnt"
    );
    // nameLabel->setScale(0.9f);
    nameLabel->limitLabelWidth(15.f, 0.9f, 0.5f);
    
    auto menu = CCMenu::create();

    // needed so that the switch statement isn't fussy about variables
    CCSprite* spr;
    CCMenuItemSpriteExtra* btn;
    TextInput* input;
    CCLabelBMFont* text;

    auto fmodEngine = FMODAudioEngine::sharedEngine();

    switch (type) {
        case Default:
            m_toggler = CCMenuItemToggler::createWithStandardSprites(
                this,
                menu_selector(SettingCell::onCheckboxToggled),
                0.75f
            );
            m_toggler->toggle(
                GameManager::get()->getGameVariable(gv.c_str())
            );

            spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            spr->setScale(0.65f);
            btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(SettingCell::onInfo)
            );
            btn->setPositionX(-35.f);
            
            menu->addChild(btn);
            menu->addChild(m_toggler);
            break;
        case FMODDebug:
            spr = ButtonSprite::create("Debug", "goldFont.fnt", "GJ_button_05.png");
            spr->setScale(0.5f);
            btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(SettingCell::onFMODDebug)
            );
            btn->setPositionX(-10.f);
            menu->addChild(btn);
            break;
        case SongSelect:
            spr = CCSprite::createWithSpriteFrameName("GJ_savedSongsBtn_001.png");
            spr->setScale(0.4f);
            menu->addChild(CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(SettingCell::onSongSelect)
            ));
            break;
        case SongOffset:
            input = TextInput::create(100.f, "Offset");
            input->getInputNode()->setAllowedChars("0123456789");
            if (fmodEngine->m_musicOffset != 0) {
                input->setString(std::to_string(fmodEngine->m_musicOffset));
            }
            input->setCallback([this, fmodEngine](std::string offset) {
                fmodEngine->m_musicOffset = 0;

                std::stringstream ss;
                ss << offset;
                int num = 0;
                ss >> num;

                if (ss.good()) {
                    fmodEngine->m_musicOffset = std::stoi(offset);
                }
            });
            input->setPositionX(-25.f);
            input->setScale(0.75f);
            menu->addChild(input);
            break;
        case Separator:
            nameLabel->setOpacity(0.f);
            text = CCLabelBMFont::create(
                name.c_str(),
                "goldFont.fnt"
            );
            text->limitLabelWidth(300.f, 0.75f, 0.1);
            this->addChildAtPosition(text, Anchor::Center);
            break;
    }

    this->addChildAtPosition(nameLabel, Anchor::Left, ccp(10.f, 0.f));
    this->addChildAtPosition(menu, Anchor::Right, ccp(-25.f, 0.f));

    nameLabel->setAnchorPoint({ 0.f, 0.5f });

    this->setContentSize({
        365.f,
        30.f
    });
    
    return true;
}

void SettingCell::onFMODDebug(CCObject* sender) {
    auto mol = MoreOptionsLayer::create();
    mol->onFMODDebug(sender);
}

void SettingCell::onSongSelect(CCObject* sender) {
    auto mol = MoreOptionsLayer::create();
    mol->onSongBrowser(sender);
}

void SettingCell::onCheckboxToggled(CCObject* sender) {
    GameManager::get()->setGameVariable(m_gameVariable.c_str(), !m_toggler->isOn());
    log::debug("set gv_{} to {}", m_gameVariable, !m_toggler->isOn());
}

std::string descForGV(std::string gv) {
    std::map<std::string, std::string> descriptions = {
        {"0026", "Restarts level upon death automatically."},
        {"0052", "Restarts in 0.5 s instead of 1.0 s upon death."},
        {"0128", "Locks and hides cursor during gameplay."},
        {"0010", "Flips which side controls which player during 2-player mode."},
        {"0011", "Limits player 1 controls to one side even when dual mode is inactive."},
        {"0028", "Disables mouse movement when using a controller thumbstick."},
        {"0163", "Enables some quick temporary bindings until full customization later. Use 'R' for reset, 'CTRL + R' for full reset, and 'P' to toggle hitboxes in Practice mode."},
        {"0024", "Shows cursor and pause button during gameplay."},
        {"0135", "Hides the attempt counter when playing levels."},
        {"0015", "Flips the location of the pause button."},
        {"0129", "Disables extra indicators on portals."},
        {"0130", "Enables extra indicators on orbs."},
        {"0140", "Disables the scaling effect on all orbs."},
        {"0141", "Disables the scaling effect on only trigger orbs."},
        {"0172", "Disables shake effects."},
        {"0014", "Disables the shake effect that happens upon death."},
        {"0072", "Disables the effect that happens upon changing gravity."},
        {"0060", "Sets player icon in mini mode to default."},
        {"0061", "Toggles between main and secondary color for the teleport effect in spider mode."},
        {"0062", "Toggles between main and secondary color for the fire effect from dash orbs."},
        {"0096", "Toggles between main and secondary color for the trail in wave mode."},
        {"0174", "Hides text in the top left when using start positions or ignore damage."},
        {"0071", "Hides the checkpoint buttons shown in practice mode."},
        {"0134", "Hides the attempt counter when playing levels in practice mode."},
        {"0027", "Places checkpoints automatically in practice mode."},
        {"0068", "Tries to place checkpoints more often in practice mode."},
        {"0100", "Shows death effects in practice mode."},
        {"0125", "Plays normal music in sync to editor levels in practice mode."},
        {"0166", "Shows hitboxes while in practice mode."},
        {"0171", "Disables the player's hitbox in practice mode (if hitboxes are shown)."},
        {"0066", "Increases draw capacity for batch nodes at level start. Can improve performance on some levels, but may cause issues on low-end devices."},
        {"0108", "Enables low detail mode on levels that support it automatically."},
        {"0082", "Removes the alert shown when starting levels with a high object count."},
        {"0136", "Removes glow and enter effects while in low detail mode. Levels without LDM show LDM Lite."},
        {"0042", "Increases maximum locally saved levels from 10 to 100. This refers to level data, not statistics. Enabling this can make your save file considerably larger, so keeping the option off is recommended for quicker saving."},
        {"0119", "Saves level statistics as usual, but levels need to be redownloaded every time you restart the game. Makes saving and loading faster."},
        {"0127", "Saves gauntlet levels locally so they do not have to be redownloaded. Increases save time but helpful if you have poor connection."},
        {"0155", "Disables anti-aliasing on shader effects."},
        {"0033", "Saves custom songs in a different directory. May fix custom songs not working."},
        {"0083", "Removes the alert shown when starting levels without the song downloaded."},
        {"0018", "Stops automatic deletion of custom songs. This is done by default to save space."},
        {"0142", "Lowers audio sampling rate from 44100 Hz to 24000 Hz. Requires restarting to take effect."},
        {"0159", "Increases the audio buffer size, which may fix certain issues. Do not enable if audio is working fine. Causes a slight more audio delay. Requires restarting to take effect."},
        {"0094", "Shows more comments per page. Why not?"},
        {"0090", "Loads comments automatically."},
        {"0073", "Makes completed levels filter based only on percentage from update 2.1. Useful to rebeat levels for Mana Orbs."},
        {"0093", "Increases created and saved levels per page from 10 to 20."},
        {"0084", "Places new levels last in the saved levels list. Useful if you want to manually move levels to the top."},
        {"0126", "Shows decimals in level progress."},
        {"0099", "Toggles viewing the leaderboard percentage you have on levels. To upload your level progress to the level leaderboard, you need to replay levels completed before 2.11."},
        {"0095", "Does not do anything... Well, nothing useful."},
        {"0167", "Adds an extra confirmation window when exiting levels."},
        {"0168", "Makes transitions between menu pages faster."},
        {"0040", "Toggles the percent label in game"},
        {"0074", "Toggles the restart button on the pause menu"},
        {"0109", "Toggles the extra info/debug label in game"},
        {"0113", "Flips the platformer controls"},
        {"0153", "Whether the player should explode on death"},
        {"0019", "Loads audio into memory. Used to be available in 2.1, not available in base 2.2"},
        {"0022", "Whether the game should use higher audio quality"},
        {"0075", "(Parental Controls) Disables comments <cy>(known to be buggy)</c>"},
        {"0076", "(Parental Controls) Disables account posts"},
        {"0077", "(Parental Controls) Removes the search button in the creator menu"},
        {"0023", "Toggles smooth fix"},
        {"0065", "Toggles move optimization"},
        {"0101", "Forces smooth fix to be on"},
        {"0102", "Toggles smooth fix in the editor"},
        {"0056", "Disables the high object alert"},
        {"0081", "Disables the shake effect in levels"},
        {"0067", "Increases the accuracy of start positions"},
    };

    return descriptions.at(gv);
}

void SettingCell::onInfo(CCObject* sender) {
    FLAlertLayer::create(
        m_name.c_str(),
        descForGV(m_gameVariable).c_str(),
        "OK"
    )->show();
}

SettingsLayer* SettingsLayer::create() {
    auto ret = new SettingsLayer();
    if (ret && ret->initAnchored(500.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCSprite* createCategoryBtnSprite(std::string name, bool isSelected = false) {
    auto sprite = CCSprite::createWithSpriteFrameName(
        isSelected ? "GJ_longBtn02_001.png" : "GJ_longBtn01_001.png"
    );
    auto text = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    text->limitLabelWidth(75.f, 0.7f, 0.1f);
    sprite->setScale(1.f);
    sprite->addChildAtPosition(text, Anchor::Center);
    text->setPositionY(text->getPositionY() + 1.5f);
    return sprite;
}

CCMenuItemSpriteExtra* createCategoryBtn(std::string name, CCObject* target, SettingPage page, SEL_MenuHandler callback) {
    auto btn = CCMenuItemSpriteExtra::create(
        createCategoryBtnSprite(name), target, callback
    );
    btn->setUserObject(CCInteger::create(page));
    btn->setID(name);
    return btn;
}

bool SettingsLayer::setup() {
    m_noElasticity = true;
    auto bg = CCScale9Sprite::create("square02b_001.png");
    bg->setContentSize({
    	100.f, 245.f
    });
    bg->setColor(ccc3(0,0,0));
    bg->setOpacity(75);

    auto menu = CCMenu::create();

    #define CATEGORY_BTN(name, page) menu->addChild( \
        createCategoryBtn(name, this, page, menu_selector(SettingsLayer::onCategoryBtn)) \
    );
    
    // auto sprite = CCSprite::createWithSpriteFrameName("GJ_longBtn01_001.png");
    // auto text = CCLabelBMFont::create("test", "bigFont.fnt");
    // text->setScale(0.7f);
    // sprite->setScale(0.8f);
    // sprite->addChildAtPosition(text, Anchor::Center);
    // auto btn = CCMenuItemSpriteExtra::create(
    //     sprite, this, nullptr
    // );

    CATEGORY_BTN("Gameplay", SettingPage::Gameplay)
    CATEGORY_BTN("Practice", SettingPage::Practice)
    CATEGORY_BTN("Performance", SettingPage::Performance)
    CATEGORY_BTN("Audio", SettingPage::Audio)
    CATEGORY_BTN("Misc", SettingPage::Misc)
    CATEGORY_BTN("Keys", SettingPage::Keybinds)

    menu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Even)
            ->setAxisReverse(true)
    );
    menu->setContentSize(bg->getContentSize());
    menu->setPosition(bg->getPosition());
    menu->setAnchorPoint({ 0.f, 0.f });
    menu->updateLayout();
    menu->setPositionX(
        bg->getContentWidth() / 2 - menu->getContentWidth() / 2
    );
    bg->addChild(menu);

    m_mainLayer->addChildAtPosition(bg, Anchor::Left, ccp(65.f, 0.f));

    switchPage(SettingPage::Gameplay, true, typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("Gameplay")));

    auto searchBtnSpr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
    auto searchBtn = CCMenuItemSpriteExtra::create(
        searchBtnSpr, this, menu_selector(SettingsLayer::onSearchBtn)
    );

    auto searchBtnOffSpr = CCSprite::createWithSpriteFrameName("gj_findBtnOff_001.png");
    m_searchClearBtn = CCMenuItemSpriteExtra::create(
        searchBtnOffSpr, this, menu_selector(SettingsLayer::onClearSearch)
    );

    m_searchClearBtn->setVisible(false);

    auto searchMenu = CCMenu::create();
    searchMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAxisReverse(true)
    );
    searchMenu->setContentSize({70, 35});
    searchMenu->setAnchorPoint({1, 0.5});
    searchMenu->addChild(searchBtn);
    searchMenu->addChild(m_searchClearBtn);
    searchMenu->updateLayout();
    m_mainLayer->addChildAtPosition(searchMenu, Anchor::TopRight, {15, 0});

    return true;
}

void SettingsLayer::onClearSearch(CCObject* sender) {
    
    auto page = static_cast<SettingPage>(static_cast<CCInteger*>(
            m_currentBtn->getUserObject()
        )->getValue());

    switchPage(page, false, m_currentBtn);
    m_searchClearBtn->setVisible(false);
}

void SettingsLayer::onSearchBtn(CCObject* sender) {
    SearchPopup::create([this](std::string query) {
        auto newList = CCArray::create();

        using namespace geode::utils::string;

        // lazily getting the original list back
        // lazy as in I am being lazy

        auto page = static_cast<SettingPage>(static_cast<CCInteger*>(
            m_currentBtn->getUserObject()
        )->getValue());

        switchPage(page, false, m_currentBtn);

        if (query == "") {
            m_searchClearBtn->setVisible(false);
            return;
        }

        m_searchClearBtn->setVisible(true);

        for (auto cell : CCArrayExt<SettingCell*>(m_listItems)) {
            if (toLower(cell->m_name).find(toLower(query)) != std::string::npos) {
                if (cell->m_type == SettingCellType::Separator) continue;
                newList->addObject(
                    SettingCell::create(cell->m_name, cell->m_gameVariable, cell->m_type)
                );
            }
        }

        m_listItems = newList;
        m_border->removeFromParent();
        this->refreshList();
    })->show();
}

void SettingsLayer::onCategoryBtn(CCObject* sender) {
    auto node = static_cast<CCMenuItemSpriteExtra*>(sender);
    int page = static_cast<CCInteger*>(
        node->getUserObject()
    )->getValue();

    switchPage(static_cast<SettingPage>(page), false, node);
}

void SettingsLayer::switchPage(SettingPage page, bool isFirstRun, CCMenuItemSpriteExtra* btn) {
    m_listItems = CCArray::create();
    if(m_searchClearBtn){
        m_searchClearBtn->setVisible(false);
    }
    #define SETTING(name, gv) m_listItems->addObject( \
        SettingCell::create(name, gv) \
    );
    #define SETTING_WITH_TYPE(name, type) m_listItems->addObject( \
        SettingCell::create(name, "", type) \
    );
    #define SEPARATOR(text) m_listItems->addObject( \
        SettingCell::create(text, "", SettingCellType::Separator) \
    );

    switch (page) {
        case Gameplay:
            SETTING("Auto Retry", "0026")
            SETTING("Flip 2P controls", "0010")
            SETTING("Always Limit Controls", "0011")
            SETTING("Show Cursor In-Game", "0024")
            SETTING("Disable Thumbstick", "0028")
            SETTING("Show Percent", "0040")
            SETTING("Fast Reset", "0052")
            SETTING("Restart Button", "0074")
            SETTING("Extra Info", "0109")
            SETTING("Flip Plat. Controls", "0113")
            SETTING("Decimal Percent", "0126")
            SETTING("Orb Labels", "0130")
            SETTING("Hide Attempts", "0134")
            SETTING("Quick Keys", "0163")
            SETTING("Confirm Exit", "0167")
            SETTING("Hide Playtest Text", "0174")
            SETTING("Flip Pause Button", "0015")
            SEPARATOR("Icon Options")
            SETTING("Explode Player on Death", "0153")
            SETTING("Default Mini Icon", "0060")
            SETTING("Switch Spider Teleport Color", "0061")
            SETTING("Switch Dash Fire Color", "0062")
            SETTING("Switch Wave Trail Color", "0096")
            break;
        case Audio:
            SETTING("Load Songs into Memory", "0019")
            SETTING("Higher Audio Quality", "0022")
            SETTING("Change Song Path", "0033")
            SETTING("No Song Limit", "0018")
            SETTING("Normal Music in Editor", "0125")
            SETTING("Reduce Audio Quality", "0142")
            SETTING("Audio Fix 01", "0159")
            SETTING_WITH_TYPE("FMOD Debug", SettingCellType::FMODDebug)
            SETTING_WITH_TYPE("Local Songs", SettingCellType::SongSelect)
            SETTING_WITH_TYPE("Song Offset (MS)", SettingCellType::SongOffset)
            break;
        case Misc:
            SETTING("Do Not...", "0095")
            SETTING("Disable Gravity Effect", "0072")
            SETTING("New Completed Filter", "0073")
            SETTING("Disable Song Alert", "0083")
            SETTING("Manual Level Order", "0084")
            SETTING("Show Leaderboard Percent", "0099")
            SETTING("Fast Menu", "0168")
            SEPARATOR("Comments")
            SETTING("Autoload Comments", "0090")
            SETTING("More Comments Mode", "0094")
            SEPARATOR("Parent Controls")
            SETTING("Disable Comments", "0075")
            SETTING("Disable Account Comments", "0076")
            SETTING("Featured Levels Only", "0077")
            break;
        case Performance:
            SETTING("Smooth Fix", "0023")
            SETTING("Move Optimization", "0065")
            SETTING("Force Smooth Fix", "0101")
            SETTING("Smooth Fix in the Editor", "0102")
            SETTING("Lock Cursor In-Game", "0128")
            SEPARATOR("LDM")
            SETTING("Auto LDM", "0108")
            SETTING("Extra LDM", "0136")
            SEPARATOR("Enhancements")
            SETTING("Increase Max Levels", "0042")
            SETTING("Disable Object Alert", "0056")
            SETTING("Save Gauntlet Levels", "0127")
            SETTING("Increase Local Levels Per Page", "0093")
            SETTING("High Capacity Mode", "0066")
            SEPARATOR("Disable Toggles")
            SETTING("Disable Explosion Shake", "0014")
            SETTING("Disable Orb Scale", "0140")
            SETTING("Disable Shake Effect", "0081")
            SETTING("Disable High Object Alert", "0082")
            SETTING("Disable Shader Anti-Aliasing", "0155")
            SETTING("Disable Trigger Orb Scale", "0141")
            break;
        case Practice:
            SETTING("Auto Checkpoints", "0027")
            SETTING("High Start Position Accuracy", "0067")
            SETTING("Quick Checkpoint Mode", "0068")
            SETTING("Hide Practice Button", "0071")
            SETTING("Practice Death Effect", "0100")
            SETTING("Hide Attempts in Practice", "0135")
            break;
        case Keybinds:
            // I need to manually create (but not show) the original
            // MoreOptionsLayer in order to do this so that I don't
            // accidently cause incompats with Custom Keybinds
            auto mol = MoreOptionsLayer::create();
            mol->onKeybindings(btn);

            // auto dummyNode = CCNode::create();
            // auto text = CCLabelBMFont::create("There's nothing here!", "goldFont.fnt");
            // dummyNode->addChildAtPosition(text, Anchor::Center);

            // m_listItems->addObject(dummyNode);
            SEPARATOR("There's nothing here!")

            break;
    }
    if (!isFirstRun) {
        m_border->removeFromParent();
    }
    
    this->refreshList();

    if (m_currentBtn) {
        m_currentBtn->setSprite(
            createCategoryBtnSprite(m_currentBtn->getID())
        );
    }
    btn->setSprite(
        createCategoryBtnSprite(btn->getID(), true)
    );
    m_currentBtn = btn;
}

void SettingsLayer::refreshList() {
    ListView* listView = ListView::create(m_listItems, 30.f, 365.f, 245.f);

    m_border = Border::create(listView, {0, 0, 0, 75}, {365.f, 245.f});
    if(CCScale9Sprite* borderSprite = typeinfo_cast<CCScale9Sprite*>(m_border->getChildByID("geode.loader/border_sprite"))) {
        float scaleFactor = 1.5f;
        borderSprite->setContentSize(CCSize{borderSprite->getContentSize().width, borderSprite->getContentSize().height + 1} / scaleFactor);
        borderSprite->setScale(scaleFactor);
        borderSprite->setPositionY(-0.5);
    }
    m_border->ignoreAnchorPointForPosition(false);
    
    for (auto cell : CCArrayExt<CCNode*>(m_listItems)) {
        cell->setContentSize({
            365.f,
            30.f
        });
        cell->updateLayout();
    }
    
    m_mainLayer->addChildAtPosition(m_border, Anchor::Right, ccp(-195.f, 0.f));
}
