#include "SkillFactory.h"
#include "Skill.h"
#include <memory>
#include <vector>
#include <map>
#include <functional>

// ==================== 基础攻击技能 ====================
std::shared_ptr<Skill> SkillFactory::createPunch() {
    return std::make_shared<Skill>(1, "直拳", "快速直拳攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 1, 1.0, 0.7, 70.0, 20.0, 0.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createKick() {
    return std::make_shared<Skill>(2, "踢腿", "基础踢腿攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 1, 3.0, 0.3, 30.0, 30.0, 0.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createHighKick() {
    return std::make_shared<Skill>(3, "高踢腿", "高位踢腿攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 4.0, 0.7, 15.0, 40.0, 1.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createHeavyPunch() {
    return std::make_shared<Skill>(4, "重拳", "强力拳击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 3.0, 0.9, 60.0, 20.0, 2.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createUppercut() {
    return std::make_shared<Skill>(5, "上钩拳", "上勾拳攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 1.5, 1.7, 65.0, 25.0, 1.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createBackhandHeavyPunch() {
    return std::make_shared<Skill>(6, "反手重拳", "反手强力拳击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 3.0, 2.0, 20.0, 60.0, 2.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createBackhandPunch() {
    return std::make_shared<Skill>(7, "反手直拳", "反手直拳攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 2.0, 1.5, 45.0, 35.0, 2.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createCrossPunch() {
    return std::make_shared<Skill>(8, "交叉拳", "交叉组合拳", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 2, 2.5, 2.0, 65.0, 25.0, 1.5, 1.6);
}

std::shared_ptr<Skill> SkillFactory::createKarateKick() {
    return std::make_shared<Skill>(9, "空手道踢腿", "空手道踢技", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 3.0, 1.0, 20.0, 50.0, 1.5, 0.8);
}

std::shared_ptr<Skill> SkillFactory::createKarateHighKick() {
    return std::make_shared<Skill>(10, "空手道高踢腿", "空手道高踢", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 5.0, 1.0, 10.0, 55.0, 2.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createClawAttack() {
    return std::make_shared<Skill>(11, "爪击", "利爪攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 8.0, 1.3, 40.0, 20.0, 4.0, 1.3);
}

std::shared_ptr<Skill> SkillFactory::createBackBreak() {
    return std::make_shared<Skill>(12, "折背", "折背攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 4.0, 1.5, 40.0, 35.0, 3.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createBoxingStraight() {
    return std::make_shared<Skill>(13, "拳术直拳", "拳术直拳", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 0.0, 1.7, 70.0, 20.0, 1.0, 0.7);
}

std::shared_ptr<Skill> SkillFactory::createChargedUppercut() {
    return std::make_shared<Skill>(14, "蓄力上勾拳", "蓄力上勾拳", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 3, 0.0, 2.0, 60.0, 25.0, 1.0, 0.8);
}

std::shared_ptr<Skill> SkillFactory::createKarateChop() {
    return std::make_shared<Skill>(15, "空手道劈斩", "空手道劈砍", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 4, 2.0, 1.5, 20.0, 55.0, 2.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createQuickStrike() {
    return std::make_shared<Skill>(16, "迅击", "迅捷攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 4, 4.0, 1.5, 60.0, 20.0, 2.0, 0.6);
}

std::shared_ptr<Skill> SkillFactory::createCloseCombat() {
    return std::make_shared<Skill>(17, "近战缠斗", "近身缠斗", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 4, 1.0, 2.5, 60.0, 20.0, 1.0, 0.7);
}

std::shared_ptr<Skill> SkillFactory::createLongPunch() {
    return std::make_shared<Skill>(18, "长拳", "基础长拳攻击", SkillEffectType::BASIC_ATTACK, 
                                  TargetType::ENEMY, 0, 2.0, 0.0, 80.0, 0.0, 3.0, 0.0);
}

// ==================== 属性增益技能 ====================
std::shared_ptr<Skill> SkillFactory::createStandFirm1() {
    return std::make_shared<Skill>(19, "屹立不倒", "敏捷不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 1, 0, 0, 0, 0, 0, 0, 4.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner1() {
    return std::make_shared<Skill>(20, "肉食跑者", "耐力不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 1, 0, 0, 0, 0, 0, 0, 4.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory1() {
    return std::make_shared<Skill>(21, "肌肉记忆", "力量不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 1, 0, 0, 0, 0, 0, 0, 4.0, "strength");
}

std::shared_ptr<Skill> SkillFactory::createStandFirm2() {
    return std::make_shared<Skill>(22, "屹立不倒2", "敏捷不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 2, 0, 0, 0, 0, 0, 0, 8.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner2() {
    return std::make_shared<Skill>(23, "肉食跑者2", "耐力不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 2, 0, 0, 0, 0, 0, 0, 8.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory2() {
    return std::make_shared<Skill>(24, "肌肉记忆2", "力量不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 2, 0, 0, 0, 0, 0, 0, 8.0, "strength");
}

std::shared_ptr<Skill> SkillFactory::createStandFirm3() {
    return std::make_shared<Skill>(25, "屹立不倒3", "敏捷不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 13.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner3() {
    return std::make_shared<Skill>(26, "肉食跑者3", "耐力不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 13.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory3() {
    return std::make_shared<Skill>(27, "肌肉记忆3", "力量不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 13.0, "strength");
}

// ==================== 训练效率技能 ====================
std::shared_ptr<Skill> SkillFactory::createSwiftness() {
    return std::make_shared<Skill>(28, "迅速", "敏捷训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 0, "agility", 1.5);
}

std::shared_ptr<Skill> SkillFactory::createWillpower() {
    return std::make_shared<Skill>(29, "意志", "耐力训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 0, "stamina", 1.5);
}

std::shared_ptr<Skill> SkillFactory::createMotivation() {
    return std::make_shared<Skill>(30, "激励", "力量训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 
                                  TargetType::SELF, 3, 0, 0, 0, 0, 0, 0, 0, "strength", 1.5);
}

// ==================== 特殊效果技能 ====================
// Finished in enemy
std::shared_ptr<Skill> SkillFactory::createFlashStrike() {
    return std::make_shared<Skill>(31, "闪击", "25%概率反伤25%对面造成的伤害", SkillEffectType::DAMAGE_REFLECT, 
                                  TargetType::ENEMY, 2, 0, 0, 0, 0, 0, 0, 0, "", 0, 0.25);
}

// Finished in player and enemy
std::shared_ptr<Skill> SkillFactory::createSuicideAttack() {
    return std::make_shared<Skill>(32, "自杀式袭击", "命中率降低10%，能量消耗降低20%", SkillEffectType::HIT_RATE_MODIFIER, 
                                  TargetType::SELF, 2, 0, 0, 0, 0, 0, 0, 0, "", 0, -0.1);
}

// Finished in Player
std::shared_ptr<Skill> SkillFactory::createThousandHands() {
    return std::make_shared<Skill>(33, "千手不破", "受到对面上肢攻击降低30%，下肢攻击增加20%", SkillEffectType::ATTACK_TYPE_DEFENSE, 
                                  TargetType::SELF, 2, 0, 0, 0, 0, 0, 0, 0, "", 0, 0.3);
}

// Finished in Player and Enemy
std::shared_ptr<Skill> SkillFactory::createHumanHammer() {
    return std::make_shared<Skill>(34, "人身重锤", "25%概率降低对面10耐力", SkillEffectType::ENEMY_ATTRIBUTE_MOD, 
                                  TargetType::ENEMY, 2, 0, 0, 0, 0, 0, 0, 0, "", 0, 10.0);
}

// Finished in player and enemy
std::shared_ptr<Skill> SkillFactory::createBluntWeapon() {
    return std::make_shared<Skill>(35, "钝兵挫锐", "免伤20%", SkillEffectType::DAMAGE_REDUCTION, 
                                  TargetType::SELF, 4, 0, 0, 0, 0, 0, 0, 0, "", 0, 0.2);
}

// Finished in player and enemy
std::shared_ptr<Skill> SkillFactory::createInfiniteEnergy() {
    return std::make_shared<Skill>(36, "无限能量", "如果体力低于50%，能量消耗降低15%", SkillEffectType::STAMINA_MODIFIER, 
                                  TargetType::SELF, 4, 0, 0, 0, 0, 0, 0, 0, "", 0, 0.15);
}

// Finished in player and enemy
std::shared_ptr<Skill> SkillFactory::createBoxer() {
    return std::make_shared<Skill>(37, "拳击手", "命中率在原本基础上提高10%", SkillEffectType::GLOBAL_HIT_RATE_BOOST, 
                                  TargetType::SELF, 4, 0, 0, 0, 0, 0, 0, 0, "", 0, 0.1);
}

// ==================== 工具方法 ====================
std::shared_ptr<Skill> SkillFactory::createSkillByName(const std::string& skillName) {
    static const std::map<std::string, std::function<std::shared_ptr<Skill>()>> skillMap = {
        {"直拳", createPunch},
        {"踢腿", createKick},
        {"高踢腿", createHighKick},
        {"重拳", createHeavyPunch},
        {"上钩拳", createUppercut},
        {"反手重拳", createBackhandHeavyPunch},
        {"反手直拳", createBackhandPunch},
        {"交叉拳", createCrossPunch},
        {"空手道踢腿", createKarateKick},
        {"空手道高踢腿", createKarateHighKick},
        {"爪击", createClawAttack},
        {"折背", createBackBreak},
        {"拳术直拳", createBoxingStraight},
        {"蓄力上勾拳", createChargedUppercut},
        {"空手道劈斩", createKarateChop},
        {"迅击", createQuickStrike},
        {"近战缠斗", createCloseCombat},
        {"长拳", createLongPunch},
        {"屹立不倒", createStandFirm1},
        {"肉食跑者", createCarnivorousRunner1},
        {"肌肉记忆", createMuscleMemory1},
        {"屹立不倒2", createStandFirm2},
        {"肉食跑者2", createCarnivorousRunner2},
        {"肌肉记忆2", createMuscleMemory2},
        {"屹立不倒3", createStandFirm3},
        {"肉食跑者3", createCarnivorousRunner3},
        {"肌肉记忆3", createMuscleMemory3},
        {"迅速", createSwiftness},
        {"意志", createWillpower},
        {"激励", createMotivation},
        {"闪击", createFlashStrike},
        {"自杀式袭击", createSuicideAttack},
        {"千手不破", createThousandHands},
        {"人身重锤", createHumanHammer},
        {"钝兵挫锐", createBluntWeapon},
        {"无限能量", createInfiniteEnergy},
        {"拳击手", createBoxer}
    };
    
    auto it = skillMap.find(skillName);
    if (it != skillMap.end()) {
        return it->second();
    }
    return nullptr;
}

std::vector<std::string> SkillFactory::getAllSkillNames() {
    return {
        "直拳", "踢腿", "高踢腿", "重拳", "上钩拳", "反手重拳", "反手直拳", "交叉拳",
        "空手道踢腿", "空手道高踢腿", "爪击", "折背", "拳术直拳", "蓄力上勾拳",
        "空手道劈斩", "迅击", "近战缠斗", "长拳",
        "屹立不倒", "肉食跑者", "肌肉记忆", "屹立不倒2", "肉食跑者2", "肌肉记忆2",
        "屹立不倒3", "肉食跑者3", "肌肉记忆3",
        "迅速", "意志", "激励",
        "闪击", "自杀式袭击", "千手不破", "人身重锤", "钝兵挫锐", "无限能量", "拳击手"
    };
}

std::map<std::string, int> SkillFactory::getSkillNameToIdMap() {
    return {
        {"直拳", 1}, {"踢腿", 2}, {"高踢腿", 3}, {"重拳", 4}, {"上钩拳", 5},
        {"反手重拳", 6}, {"反手直拳", 7}, {"交叉拳", 8}, {"空手道踢腿", 9},
        {"空手道高踢腿", 10}, {"爪击", 11}, {"折背", 12}, {"拳术直拳", 13},
        {"蓄力上勾拳", 14}, {"空手道劈斩", 15}, {"迅击", 16}, {"近战缠斗", 17},
        {"长拳", 18}, {"屹立不倒", 19}, {"肉食跑者", 20}, {"肌肉记忆", 21},
        {"屹立不倒2", 22}, {"肉食跑者2", 23}, {"肌肉记忆2", 24}, {"屹立不倒3", 25},
        {"肉食跑者3", 26}, {"肌肉记忆3", 27}, {"迅速", 28}, {"意志", 29}, {"激励", 30},
        {"闪击", 31}, {"自杀式袭击", 32}, {"千手不破", 33}, {"人身重锤", 34},
        {"钝兵挫锐", 35}, {"无限能量", 36}, {"拳击手", 37}
    };
}
