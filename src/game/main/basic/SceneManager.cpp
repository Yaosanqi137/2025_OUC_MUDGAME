#include "SceneManager.h"
#include "SceneBase.h"
#include "Terminal.h"

SceneManager& SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::registerScene(const std::string& id, std::unique_ptr<SceneBase> scene) {
    scenes[id] = std::move(scene);
}

void SceneManager::switchScene(const std::string& id) {
    if (scenes.find(id) == scenes.end()) {
        // 场景不存在
        return;
    }

    // 退出当前主场景
    if (currentMainScene) {
        currentMainScene->onExit();
    }

    // 切换到新场景
    currentMainScene = scenes[id].get();
    currentMainScene->onEnter();

    currentSceneID = id;
}

const std::string &SceneManager::getCurrentSceneID() {
    return currentSceneID;
}

SceneBase *SceneManager::getCurrentScene() const
{
    return currentMainScene;
}
