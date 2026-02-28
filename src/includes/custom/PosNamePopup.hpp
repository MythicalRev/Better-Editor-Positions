#pragma once

#include <Geode/Geode.hpp>
#include "EditorPosHandler.hpp"

using namespace geode::prelude;

class PosNamePopup : public Popup {
protected:
    std::string lvlName;
    TextInput* posNameInput;

    bool init(std::string levelName) {
        if (!Popup::init(200, 100))
            return false;
        
        this->m_closeBtn->setVisible(false);
        
        this->setTitle("Name Position");

        auto layout = ColumnLayout::create();
        layout->setGap(5);
        layout->setAxisReverse(true);

        posNameInput = TextInput::create(180, "Position Name");
        posNameInput->setTextAlign(TextInputAlign::Center);
        posNameInput->setString("NewPos");
        posNameInput->setAnchorPoint(ccp(0.5,0.5));
        posNameInput->ignoreAnchorPointForPosition(false);

        auto confirmBtnSpr = ButtonSprite::create("Confirm");
        confirmBtnSpr->setScale(0.7f);

        auto confirmBtn = CCMenuItemSpriteExtra::create(confirmBtnSpr, this, menu_selector(PosNamePopup::onConfirm));
        confirmBtn->setAnchorPoint(ccp(0.5,0.5));
        confirmBtn->ignoreAnchorPointForPosition(false);

        auto menu = CCMenu::create();
        menu->setAnchorPoint(ccp(0.5,1));
        menu->setContentHeight(confirmBtn->getContentHeight());
        menu->ignoreAnchorPointForPosition(false);

        this->m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, ccp(0,30));

        this->m_mainLayer->addChildAtPosition(posNameInput, Anchor::Center);
        menu->addChildAtPosition(confirmBtn, Anchor::Bottom);

        menu->setLayout(layout);
        menu->updateLayout();
        
        lvlName = levelName;

        return true;
    }

    void onConfirm(CCObject* sender) {
        EditorPosHandler posHandler;
        posHandler.updatePositionName(posHandler.getLevelPositions(lvlName).size() - 1, posNameInput->getString(), lvlName);

        auto newAlert = TextAlertPopup::create("Created New Position", 2, .5f, 0, "bigFont.fnt");

        newAlert->setPositionY(120);
        newAlert->setID("newpos-alert"_spr);
        this->getParent()->addChild(newAlert);

        this->onClose(sender);
    }
public:
    static PosNamePopup* create(std::string levelName) {
        auto pRet = new PosNamePopup();
        if (pRet->init(levelName)) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};