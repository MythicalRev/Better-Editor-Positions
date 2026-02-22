#pragma once

#include <Geode/Geode.hpp>
#include "EditorPosHandler.hpp"

using namespace geode::prelude;

class EditorPosCell : public CCLayerColor {
public:
    CCMenuItemSpriteExtra* gotoBtn;
    CCMenuItemSpriteExtra* deleteBtn;
    TextInput* nameInput;

    std::string cellPosName;
    std::string lvlName;
    int cellIndex;
    CCNode* batLayer;

    std::function<void()> onDelete;
    std::function<void()> onGo;

    bool init(std::string posName, int posIndex, std::string levelName, CCNode* batchLayer) {
        if (!CCLayerColor::init()) return false;

        if (posIndex == 0 || (posIndex % 2) == 0) {
            this->setOpacity(100);
        } else {
            this->setOpacity(50);    
        }

        this->setContentSize(ccp(315, 35));
        this->setAnchorPoint(ccp(0, 1));
        this->setPositionY(207);

        nameInput = TextInput::create(100, posName);

        nameInput->setTextAlign(TextInputAlign::Left);
        nameInput->setString(posName);
        nameInput->setAnchorPoint(ccp(0,0.5f));
        nameInput->setPosition(ccp(5, this->getContentHeight() / 2));
        nameInput->setCallback([this](const std::string& value) {
            onPosNameChange(value);
        });

        auto deleteBtnSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        deleteBtnSpr->setScale(0.65f);
        deleteBtn = CCMenuItemSpriteExtra::create(deleteBtnSpr, this, menu_selector(EditorPosCell::onDeletePosBtn));

        auto goBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        goBtnSpr->setScale(0.65f);
        goBtnSpr->setFlipX(true);
        gotoBtn = CCMenuItemSpriteExtra::create(goBtnSpr, this, menu_selector(EditorPosCell::onGoToBtn));

        auto menuLayout = RowLayout::create();
        menuLayout->setAxisAlignment(AxisAlignment::End);

        auto leftMenu = CCMenu::createWithItem(gotoBtn);
        leftMenu->setAnchorPoint(ccp(1, 0.5f));
        leftMenu->ignoreAnchorPointForPosition(false);
        leftMenu->setPosition(CCPoint(this->getContentWidth() - 5,this->getContentHeight()/ 2));
        leftMenu->setContentSize(CCSize(100, 30));

        leftMenu->addChild(deleteBtn);

        menuLayout->apply(leftMenu);

        leftMenu->updateLayout();
        
        this->addChild(nameInput);
        this->addChild(leftMenu);

        lvlName = levelName;
        cellIndex = posIndex;
        batLayer = batchLayer;
        cellPosName = posName;

        return true;
    }

    static EditorPosCell* create(std::string posName, int posIndex, std::string levelName, CCNode* batchLayer) {
        EditorPosCell* pRet = new EditorPosCell();
        if (pRet && pRet->init(posName, posIndex, levelName, batchLayer)) {
            pRet->autorelease();
            return pRet;
        } else {
            CC_SAFE_DELETE(pRet);
            return nullptr;
        }
    }
protected:
    void onDeletePosBtn(CCObject* sender) {
        EditorPosHandler editorPosHandler;

        editorPosHandler.removePositionByIndex(cellIndex, lvlName);

        if (onDelete) onDelete();
    }

    void onPosNameChange(const std::string& value) {
        EditorPosHandler editorPosHandler;

        editorPosHandler.updatePositionName(cellIndex, value, lvlName);
    }

    void onGoToBtn(CCObject* sender) {
        EditorPosHandler editorPosHandler;

        editorPosHandler.moveToPosition(cellIndex, lvlName, batLayer);

        if (onGo) onGo();
    }
};