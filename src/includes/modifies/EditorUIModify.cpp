#include <Geode/Geode.hpp>

#include <Geode/modify/EditorUI.hpp>
#include "../custom/EditorPosHandler.hpp"
#include "../custom/EditorPosPopup.hpp"

using namespace geode::prelude;

class $modify(BEPEditorUI, EditorUI) {
    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", Priority::VeryLatePost);
    }
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        
        auto addEditorPosSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        addEditorPosSpr->setScale(.8f);
        auto addEditorPosBtn = CCMenuItemSpriteExtra::create(addEditorPosSpr, this, menu_selector(BEPEditorUI::onPosAdded));
        addEditorPosBtn->setID("addpos-btn"_spr);

        auto posMenuSpr = CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png");
        posMenuSpr->setScale(.7f);
        auto posMenuBtn = CCMenuItemSpriteExtra::create(posMenuSpr, this, menu_selector(BEPEditorUI::onMenuOpened));
        posMenuBtn->setID("posmenu-btn"_spr);

        this->getChildByID("settings-menu")->addChild(addEditorPosBtn);
        this->getChildByID("settings-menu")->addChild(posMenuBtn);
        this->getChildByID("settings-menu")->updateLayout();

        return true;
    }

    void onPosAdded(CCObject* sender) {
        EditorPosHandler editorPosHandler;

        auto currentPos = m_editorLayer->getChildByID("main-node")->getChildByID("batch-layer")->getPosition();
        auto zoom = m_editorLayer->getChildByID("main-node")->getChildByID("batch-layer")->getScaleX();

        editorPosHandler.createNewPosition(currentPos, zoom, m_editorLayer->m_level->m_levelName);

        auto newAlert = TextAlertPopup::create("Created New Position", 2, .5f, 0, "bigFont.fnt");

        newAlert->setPositionY(120);
        newAlert->setID("newpos-alert"_spr);
        this->addChild(newAlert);
    };

    void onMenuOpened(CCObject* sender) {
        auto posMenu = EditorPosPopup::create(m_editorLayer);
        posMenu->setID("pos-menu"_spr);
        posMenu->show();
    }
};