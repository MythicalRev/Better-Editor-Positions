#include "EditorPosHandler.hpp"
#include <Geode/Geode.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

void EditorPosHandler::createNewPosition(CCPoint newPoint, float zoomVal, std::string levelName) {
    log::debug("Created Position at {}", newPoint);
    log::debug("With zoom {}", zoomVal);

    auto savedPositions = getMod()->getSavedValue<matjson::Value>(levelName);

    if (!savedPositions.isArray()) {
        savedPositions = matjson::Value(std::vector<matjson::Value>{});
    }

    auto& arr = savedPositions.asArray().unwrap();

    auto entry = matjson::makeObject({
        {"name", "NewPos"},
        { "x", newPoint.x },
        { "y", newPoint.y },
        { "zoom", zoomVal }
    });

    arr.push_back(entry);

    getMod()->setSavedValue(levelName, arr);
}

std::vector<std::tuple<std::string, CCPoint, float>> EditorPosHandler::getLevelPositions(std::string levelName) {
    auto savedPositions = getMod()->getSavedValue<matjson::Value>(levelName);
    std::vector<std::tuple<std::string, CCPoint, float>> positions;

    if (!savedPositions.isArray()) return positions;

    for (auto& obj : savedPositions.asArray().unwrap()) {
        std::string name = obj["name"].asString().unwrap();
        float x = obj["x"].asDouble().unwrap();
        float y = obj["y"].asDouble().unwrap();
        float zoom = obj["zoom"].asDouble().unwrap();
        positions.push_back({name, CCPoint(x, y), zoom });
    }

    return positions;
}