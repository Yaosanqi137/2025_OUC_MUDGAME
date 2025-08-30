#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SceneBase.h"
#include <memory>
#include <map>
#include <stack>
#include <string>

class SceneManager {
public:
    // 单例访问
    static SceneManager& getInstance();

    // 禁止复制和赋值
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // 注册场景
    void registerScene(const std::string& id, std::unique_ptr<SceneBase> scene);

    // 场景切换
    void switchScene(const std::string& id);

    // 获取当前场景
    const std::string& getCurrentSceneID();
    SceneBase* getCurrentScene() const;

private:
    // 私有构造函数
    SceneManager() = default;

    // 场景注册表
    std::map<std::string, std::unique_ptr<SceneBase>> scenes;

    // 当前主场景
    SceneBase* currentMainScene = nullptr;

    std::string currentSceneID;
};

#endif // SCENE_MANAGER_H