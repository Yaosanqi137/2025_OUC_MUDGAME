#include "Configuration.h"
#include <iostream>
#include <fstream>

const     std::string DEFAULT_GAME_NAME                 =    "拳王之路"          ;
const     std::string DEFAULT_GAME_VERSION              =    "1.0.0"            ;
constexpr int         DEFAULT_TYPEWRITER_SPEED_MS       =    50                 ; // 默认打字机速度：50毫秒/字符
constexpr int         DEFAULT_DIFFICULTY                =    2                  ; // 默认难度：普通

Configuration::Configuration() : configFilePath("config.toml") {
    if (!std::filesystem::exists(configFilePath) || !loadConfig(configFilePath.string())) {
        std::cout << "无法加载配置文件，将使用默认配置。" << std::endl;
        initializeDefaultConfig();
        saveConfig();
    }
}

Configuration::~Configuration() {
    saveConfig();
}

Configuration* Configuration::getInstance() {
    static Configuration instance;
    return &instance;
}

bool Configuration::loadConfig(const std::string& filePath) {
    try {
        configFilePath = filePath;
        configData = toml::parse(filePath);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "加载配置文件时发生错误: " << e.what() << std::endl;
        return false;
    }
}

void Configuration::saveConfig() const {
    try {
        if (configFilePath.has_parent_path() && !std::filesystem::exists(configFilePath.parent_path())) {
            std::filesystem::create_directories(configFilePath.parent_path());
        }

        std::ofstream file(configFilePath.string());
        if (!file.is_open()) {
            std::cerr << "无法打开配置文件进行写入: " << configFilePath.string() << std::endl;
            return ;
        }

        file << toml::format(configData);
    } catch (const std::exception& e) {
        std::cerr << "保存配置文件时发生错误: " << e.what() << std::endl;
    }
}

std::string Configuration::getConfigFilePath() const {
    return configFilePath.string();
}

// Game basic configuration
std::string Configuration::getGameName() const {
    return toml::find_or<std::string>(configData, "game", "name", DEFAULT_GAME_NAME);
}

std::string Configuration::getGameVersion() const {
    return toml::find_or<std::string>(configData, "game", "version", DEFAULT_GAME_VERSION);
}

void Configuration::setGameName(const std::string& name) {
    configData["game"]["name"] = name;
}

void Configuration::setGameVersion(const std::string& version) {
    configData["game"]["version"] = version;
}

// --- Game view configuration ---
int Configuration::getTypewriterSpeed() const {
    return toml::find_or<int>(configData, "view", "typewriter_speed_ms", DEFAULT_TYPEWRITER_SPEED_MS);
}

void Configuration::setTypewriterSpeed(int speed_ms) {
    // 确保速度值在一个合理的范围内 -> (5, 200)
    if (speed_ms < 5) speed_ms = 5;
    if (speed_ms > 200) speed_ms = 200;
    configData["view"]["typewriter_speed_ms"] = speed_ms;
}

// Game difficulty settings
int Configuration::getDifficultyLevel() const {
    return toml::find_or<int>(configData, "difficulty", "level", DEFAULT_DIFFICULTY);
}

double Configuration::getEnemyDamageMultiplier() const {
    return toml::find_or<double>(configData, "difficulty", "enemy_damage_multiplier", 1.0);
}

double Configuration::getEnemyHealthMultiplier() const {
    return toml::find_or<double>(configData, "difficulty", "enemy_health_multiplier", 1.0);
}

void Configuration::setDifficultyLevel(int level) {
    configData["difficulty"]["level"] = level;
    switch (level) {
        case 1: // Easy
            setEnemyDamageMultiplier(0.8);
            setEnemyHealthMultiplier(0.8);
            break;
        case 2: // Normal
            setEnemyDamageMultiplier(1.0);
            setEnemyHealthMultiplier(1.0);
            break;
        case 3: // Hard
            setEnemyDamageMultiplier(1.2);
            setEnemyHealthMultiplier(1.2);
            break;
        default: // Default to Normal
            configData["difficulty"]["level"] = 2;
            setEnemyDamageMultiplier(1.0);
            setEnemyHealthMultiplier(1.0);
            break;
    }
}

void Configuration::setEnemyDamageMultiplier(double multiplier) {
    configData["difficulty"]["enemy_damage_multiplier"] = multiplier;
}

void Configuration::setEnemyHealthMultiplier(double multiplier) {
    configData["difficulty"]["enemy_health_multiplier"] = multiplier;
}


void Configuration::initializeDefaultConfig() {
    configData = toml::table{};

    // Game basic configuration
    setGameName(DEFAULT_GAME_NAME);
    setGameVersion(DEFAULT_GAME_VERSION);

    // Game view configuration
    setTypewriterSpeed(DEFAULT_TYPEWRITER_SPEED_MS);

    // Game difficulty settings
    setDifficultyLevel(DEFAULT_DIFFICULTY);
}
