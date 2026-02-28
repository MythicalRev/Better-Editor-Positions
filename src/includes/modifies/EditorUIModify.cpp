#include <Geode/Geode.hpp>

#include <Geode/modify/EditorUI.hpp>
#include "../custom/EditorPosHandler.hpp"
#include "../custom/EditorPosPopup.hpp"
#include "../custom/PosNamePopup.hpp"

using namespace geode::prelude;

class $modify(BEPEditorUI, EditorUI) {
    struct Fields {
        CCMenuItemSpriteExtra* m_addPosBtn;
        CCMenuItemSpriteExtra* m_posMenuBtn;
    };

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", Priority::VeryLatePost);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        
        auto addEditorPosSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        addEditorPosSpr->setScale(.8f);
        m_fields->m_addPosBtn = CCMenuItemSpriteExtra::create(addEditorPosSpr, this, menu_selector(BEPEditorUI::onPosAdded));
        m_fields->m_addPosBtn->setID("addpos-btn"_spr);

        auto posMenuSpr = CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png");
        posMenuSpr->setScale(.6f);
        m_fields->m_posMenuBtn = CCMenuItemSpriteExtra::create(posMenuSpr, this, menu_selector(BEPEditorUI::onMenuOpened));
        m_fields->m_posMenuBtn->setID("posmenu-btn"_spr);

        this->getChildByID("settings-menu")->addChild(m_fields->m_addPosBtn);
        this->getChildByID("settings-menu")->addChild(m_fields->m_posMenuBtn);
        this->getChildByID("settings-menu")->updateLayout();

        return true;
    }

    void onPlaytest(CCObject* sender) {
        EditorUI::onPlaytest(sender);

        m_fields->m_addPosBtn->setVisible(false);
        m_fields->m_posMenuBtn->setVisible(false);
    }

    void playtestStopped() {
        EditorUI::playtestStopped();

        m_fields->m_addPosBtn->setVisible(true);
        m_fields->m_posMenuBtn->setVisible(true);
    }

    void onPosAdded(CCObject* sender) {
        EditorPosHandler editorPosHandler;

        auto currentPos = m_editorLayer->getChildByID("main-node")->getChildByID("batch-layer")->getPosition();
        auto zoom = m_editorLayer->getChildByID("main-node")->getChildByID("batch-layer")->getScaleX();

        editorPosHandler.createNewPosition(currentPos, zoom, m_editorLayer->m_level->m_levelName);

        auto namePopup = PosNamePopup::create(m_editorLayer->m_level->m_levelName);
        namePopup->show();
        namePopup->setID("rename-menu"_spr);
    };

    void onMenuOpened(CCObject* sender) {
        auto posMenu = EditorPosPopup::create(m_editorLayer);
        posMenu->setID("pos-menu"_spr);
        posMenu->show();
    }
};