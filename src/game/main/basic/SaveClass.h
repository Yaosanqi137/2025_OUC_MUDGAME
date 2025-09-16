#ifndef SAVECLASS_H
#define SAVECLASS_H

#include <string>
#include <filesystem>
#include <toml.hpp>

// 前向声明 Game 类，避免循环引用
class Game;

/**
 * @class SaveClass
 * @brief 使用单例模式管理游戏的存档和读档功能。
 */
class SaveClass {
public:
    /**
     * @brief 获取SaveClass的全局唯一实例。
     * @return 指向SaveClass实例的指针。
     */
    static SaveClass* getInstance();

    SaveClass(const SaveClass&) = delete;
    void operator=(const SaveClass&) = delete;

    /**
     * @brief 将当前游戏状态保存到指定的存档文件。
     * @param game 对 Game 核心对象的引用。
     * @param filePath 存档文件的路径，默认为 "save.toml"。
     */
    void saveGame(Game& game, const std::string& filePath = "save.toml");

    /**
     * @brief 从指定的存档文件加载游戏状态。
     * @param game 对 Game 核心对象的引用。
     * @param filePath 存档文件的路径，默认为 "save.toml"。
     * @return 如果加载成功，返回 true；否则返回 false。
     */
    bool loadGame(Game& game, const std::string& filePath = "save.toml");

    /**
     * @brief 检查默认的存档文件是否存在。
     * @param filePath 存档文件的路径，默认为 "save.toml"。
     * @return 如果文件存在，返回 true。
     */
    bool doesSaveExist(const std::string& filePath = "save.toml") const;


private:
    SaveClass() = default; // 私有构造函数
    ~SaveClass() = default; // 私有析构函数
};

#endif // SAVECLASS_H