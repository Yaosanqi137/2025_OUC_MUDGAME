#ifndef INC_2025_OUC_MUDGAME_CONFIGURATION_H
#define INC_2025_OUC_MUDGAME_CONFIGURATION_H

#include <toml.hpp>
#include <string>
#include <filesystem>

/**
 * @class Configuration
 * @brief 使用单例模式管理游戏的全局配置。
 * @details 负责加载、保存和提供对 `config.toml` 文件中各项配置的访问。
 */
class Configuration {
public:
    /**
     * @brief 获取Configuration的全局唯一实例。
     * @return 指向Configuration实例的指针。
     */
    static Configuration* getInstance();

    Configuration(const Configuration&) = delete;
    void operator=(const Configuration&) = delete;

    ~Configuration();

    /**
     * @brief 从指定路径加载配置文件。
     * @param filePath 配置文件的路径。
     * @return 如果加载成功，返回true；否则返回false。
     */
    bool loadConfig(const std::string& filePath);

    /**
     * @brief 将当前配置保存到文件中。
     */
    void saveConfig() const;

    /**
     * @brief 获取当前配置文件的路径。
     * @return 配置文件的路径字符串。
     */
    [[nodiscard]] std::string getConfigFilePath() const;

    // --- Game basic configuration ---
    [[nodiscard]] std::string getGameName() const;
    [[nodiscard]] std::string getGameVersion() const;
    void setGameName(const std::string& name);
    void setGameVersion(const std::string& version);

    // --- Game view configuration ---
    /**
     * @brief 获取打字机效果的速度。
     * @return 每个字符显示的毫秒数。数值越小，速度越快。
     */
    [[nodiscard]] int getTypewriterSpeed() const;

    /**
     * @brief 设置打字机效果的速度。
     * @param speed_ms 每个字符要显示的毫秒数。
     */
    void setTypewriterSpeed(int speed_ms);

    // --- Game difficulty settings ---
    [[nodiscard]] int getDifficultyLevel() const;
    [[nodiscard]] double getEnemyDamageMultiplier() const;
    [[nodiscard]] double getEnemyHealthMultiplier() const;
    void setDifficultyLevel(int level);
    void setEnemyDamageMultiplier(double multiplier);
    void setEnemyHealthMultiplier(double multiplier);

private:
    /**
     * @brief 初始化为默认配置。
     * @details 当配置文件不存在或加载失败时调用。
     */
    void initializeDefaultConfig();

    Configuration();

    toml::value configData;
    std::filesystem::path configFilePath;
};

#endif //INC_2025_OUC_MUDGAME_CONFIGURATION_H