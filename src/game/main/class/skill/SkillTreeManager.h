#ifndef SKILLTREE_MANAGER_H
#define SKILLTREE_MANAGER_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "Skill.h"
#include "SkillFactory.h"

class SkillTreeManager {
public:
    SkillTreeManager();
    
    // 设置玩家已学技能
    void setLearnedSkills(const std::vector<std::string>& learnedSkills);
    
    // 添加已学技能
    void addLearnedSkill(const std::string& skillName);
    
    // 检查是否可以学习技能
    bool canLearnSkill(const std::string& skillName) const;
    
    
    // 获取技能前置要求
    std::vector<std::string> getPrerequisites(const std::string& skillName) const;
    
    // 尝试学习技能（返回是否成功）
    bool tryLearnSkill(const std::string& skillName, int availableSkillPoints);
    
    // 获取已学技能列表
    const std::vector<std::string>& getLearnedSkills() const;

    std::vector<std::string> getAvailableSkills() const;

private:
    std::vector<std::string> learnedSkills_;
    
    // 技能前置关系映射
    static const std::map<std::string, std::vector<std::string>> skillPrerequisites;
    
    // 检查是否满足前置条件
    bool checkPrerequisites(const std::string& skillName) const;
};

#endif // SKILLTREE_MANAGER_H