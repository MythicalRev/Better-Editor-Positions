#pragma once

using namespace geode::prelude;

class EditorPosHandler {
public:
    void createNewPosition(CCPoint newPoint, float zoomVal, std::string levelName);
    void removePositionByIndex(int index, std::string levelName);
    void moveToPosition(int posIndex, std::string levelName);
    std::vector<std::tuple<std::string, CCPoint, float>> getLevelPositions(std::string levelName);
};