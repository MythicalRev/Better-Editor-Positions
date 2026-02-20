#pragma once

#include <Geode/Geode.hpp>
#include "EditorPosHandler.hpp"
#include "EditorPosCell.hpp"

using namespace geode::prelude;

class EditorPosPopup : public Popup {
protected:
    bool init(LevelEditorLayer* lvlEditorLayer) {
        if (!Popup::init(350, 260))
            return false;

        this->setTitle("Saved Positions");

        scrollLayer = ScrollLayer::create(ccp(313, 210));
        scrollLayer->ignoreAnchorPointForPosition(false);
        scrollLayer->setAnchorPoint(ccp(0.5, 0.5));

        m_mainLayer->addChildAtPosition(scrollLayer, Anchor::Center, ccp(0, -10));

        auto layout = ColumnLayout::create();
        layout->setAxisReverse(true);
        layout->setGap(0);
        layout->setAxisAlignment(AxisAlignment::Start);

        scrollLayer->m_contentLayer->setLayout(layout);

        reloadList(lvlEditorLayer);

        return true;
    }

    void reloadList(LevelEditorLayer* lvlEditorLayer) {        
        EditorPosHandler editorPosHandler;
        auto positions = editorPosHandler.getLevelPositions(lvlEditorLayer->m_level->m_levelName);

        scrollLayer->m_contentLayer->setContentSize(CCSize(313, 35 * positions.size()));

        int index = 0;
        for (auto& [name, pos, zoom] : positions) {
            auto newCell = EditorPosCell::create(name, index, lvlEditorLayer->m_level->m_levelName, lvlEditorLayer->getChildByID("main-node")->getChildByID("batch-layer"));
            newCell->setContentSize(ccp(313, 35));
            newCell->ignoreAnchorPointForPosition(false);
            newCell->setAnchorPoint(ccp(0, 1));
            newCell->onDelete = [this, lvlEditorLayer]() {
                scrollLayer->m_contentLayer->removeAllChildren();
                reloadList(lvlEditorLayer);
            };
            newCell->onGo = [this, newCell]() {
                this->onClose(newCell->gotoBtn);
            };

            scrollLayer->m_contentLayer->addChild(newCell);
            ++index;
        }

        scrollLayer->m_contentLayer->updateLayout();
        scrollLayer->moveToTop();
    }
public:
    ScrollLayer* scrollLayer;

    static EditorPosPopup* create(LevelEditorLayer* lvlEditorLayer) {
        auto pRet = new EditorPosPopup();
        if (pRet->init(lvlEditorLayer)) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
};