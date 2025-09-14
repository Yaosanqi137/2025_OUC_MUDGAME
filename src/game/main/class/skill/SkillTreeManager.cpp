#include "SkillTreeManager.h"
#include <algorithm>

// 技能前置关系定义
const std::map<std::string, std::vector<std::string>> SkillTreeManager::skillPrerequisites = {
    // 初始技能（无前置）
    {"直拳", {}},
    {"踢腿", {}},
    
    // 第一层分支
    {"高踢腿", {"直拳", "踢腿"}},
    {"重拳", {"直拳", "踢腿"}},
    {"上钩拳", {"直拳", "踢腿"}},
    
    // 高踢腿分支
    {"屹立不倒", {"高踢腿"}},
    {"反手重拳", {"屹立不倒"}},
    {"空手道踢腿", {"反手重拳"}},
    {"空手道高踢腿", {"反手重拳"}},
    {"闪击", {"空手道踢腿"}},
    {"空手道劈斩", {"空手道高踢腿"}},
    {"屹立不倒2", {"闪击"}},
    {"迅速", {"屹立不倒2", "空手道劈斩"}},
    {"钝兵挫锐", {"迅速"}},
    {"屹立不倒3", {"钝兵挫锐"}},
    
    // 重拳分支
    {"肉食跑者", {"重拳"}},
    {"反手直拳", {"肉食跑者"}},
    {"爪击", {"反手直拳"}},
    {"自杀式袭击", {"反手直拳"}},
    {"折背", {"爪击"}},
    {"千手不破", {"自杀式袭击"}},
    {"肉食跑者2", {"折背"}},
    {"迅击", {"千手不破"}},
    {"意志", {"肉食跑者2", "迅击"}},
    {"无限能量", {"意志"}},
    {"肉食跑者3", {"无限能量"}},
    
    // 上钩拳分支
    {"肌肉记忆", {"上钩拳"}},
    {"交叉拳", {"肌肉记忆"}},
    {"拳术直拳", {"交叉拳"}},
    {"肌肉记忆2", {"交叉拳"}},
    {"蓄力上钩拳", {"交叉拳"}},
    {"近战缠斗", {"拳术直拳"}},
    {"人身重锤", {"近战缠斗"}},
    {"激励", {"人身重锤", "肌肉记忆2", "蓄力上钩拳"}},
    {"拳击手", {"激励"}},
    {"肌肉记忆3", {"拳击手"}}
};

SkillTreeManager::SkillTreeManager() {}

void SkillTreeManager::setLearnedSkills(const std::vector<std::string>& learnedSkills) {
    learnedSkills_ = learnedSkills;
}

void SkillTreeManager::addLearnedSkill(const std::string& skillName) {
    if (std::find(learnedSkills_.begin(), learnedSkills_.end(), skillName) == learnedSkills_.end()) {
        learnedSkills_.push_back(skillName);
    }
}

bool SkillTreeManager::checkPrerequisites(const std::string& skillName) const {
    auto it = skillPrerequisites.find(skillName);
    if (it == skillPrerequisites.end()) {
        return false;
    }
    
    const auto& prerequisites = it->second;
    if (prerequisites.empty()) {
        return true;
    }
    
    for (const auto& prereq : prerequisites) {
        if (std::find(learnedSkills_.begin(), learnedSkills_.end(), prereq) == learnedSkills_.end()) {
            return false;
        }
    }
    
    return true;
}

bool SkillTreeManager::canLearnSkill(const std::string& skillName) const {
    // 检查是否已学习
    if (std::find(learnedSkills_.begin(), learnedSkills_.end(), skillName) != learnedSkills_.end()) {
        return false;
    }
    
    // 检查前置条件
    return checkPrerequisites(skillName);
}

std::vector<std::string> SkillTreeManager::getAvailableSkills() const {
    std::vector<std::string> availableSkills;
    
    for (const auto& skillPair : skillPrerequisites) {
        const auto& skillName = skillPair.first;
        if (canLearnSkill(skillName)) {
            availableSkills.push_back(skillName);
        }
    }
    
    return availableSkills;
}

std::vector<std::string> SkillTreeManager::getPrerequisites(const std::string& skillName) const {
    auto it = skillPrerequisites.find(skillName);
    if (it != skillPrerequisites.end()) {
        return it->second;
    }
    return {};
}

bool SkillTreeManager::tryLearnSkill(const std::string& skillName, int availableSkillPoints) {
    auto skill = SkillFactory::createSkillByName(skillName);
    if (!skill) {
        return false;
    }
    
    // 检查前置条件和是否已学习
    if (!canLearnSkill(skillName)) {
        return false;
    }
    
    // 检查技能点
    if (availableSkillPoints < skill->getUnlockCost()) {
        return false;
    }
    

    // 添加到已学技能
    addLearnedSkill(skillName);
    return true;
}

const std::vector<std::string>& SkillTreeManager::getLearnedSkills() const {
    return learnedSkills_;
}

