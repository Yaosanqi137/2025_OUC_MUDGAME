#include "SkillFactory.h"
#include <memory>
#include <vector>
#include <map>

// ==================== 基础攻击技能 ====================
std::shared_ptr<Skill> SkillFactory::createPunch() {
    return std::make_shared<Skill>("直拳", "快速直拳攻击", SkillEffectType::BASIC_ATTACK, 1,
        1.0, 0.7, 70.0, 20.0, 0.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createKick() {
    return std::make_shared<Skill>("踢腿", "基础踢腿攻击", SkillEffectType::BASIC_ATTACK, 1,
        3.0, 0.3, 30.0, 30.0, 0.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createHighKick() {
    return std::make_shared<Skill>("高踢腿", "高位踢腿攻击", SkillEffectType::BASIC_ATTACK, 2,
        4.0, 0.7, 15.0, 40.0, 1.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createHeavyPunch() {
    return std::make_shared<Skill>("重拳", "强力拳击", SkillEffectType::BASIC_ATTACK, 2,
        3.0, 0.9, 60.0, 20.0, 2.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createUppercut() {
    return std::make_shared<Skill>("上钩拳", "上勾拳攻击", SkillEffectType::BASIC_ATTACK, 2,
        1.5, 1.7, 65.0, 25.0, 1.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createBackhandHeavyPunch() {
    return std::make_shared<Skill>("反手重拳", "反手强力拳击", SkillEffectType::BASIC_ATTACK, 2,
        3.0, 2.0, 20.0, 60.0, 2.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createBackhandPunch() {
    return std::make_shared<Skill>("反手直拳", "反手直拳攻击", SkillEffectType::BASIC_ATTACK, 2,
        2.0, 1.5, 45.0, 35.0, 2.0, 1.5);
}

std::shared_ptr<Skill> SkillFactory::createCrossPunch() {
    return std::make_shared<Skill>("交叉拳", "交叉组合拳", SkillEffectType::BASIC_ATTACK, 2,
        2.5, 2.0, 65.0, 25.0, 1.5, 1.6);
}

std::shared_ptr<Skill> SkillFactory::createKarateKick() {
    return std::make_shared<Skill>("空手道踢腿", "空手道踢技", SkillEffectType::BASIC_ATTACK, 3,
        3.0, 1.0, 20.0, 50.0, 1.5, 0.8);
}

std::shared_ptr<Skill> SkillFactory::createKarateHighKick() {
    return std::make_shared<Skill>("空手道高踢腿", "空手道高踢", SkillEffectType::BASIC_ATTACK, 3,
        5.0, 1.0, 10.0, 55.0, 2.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createClawAttack() {
    return std::make_shared<Skill>("爪击", "利爪攻击", SkillEffectType::BASIC_ATTACK, 3,
        8.0, 1.3, 40.0, 20.0, 4.0, 1.3);
}

std::shared_ptr<Skill> SkillFactory::createBackBreak() {
    return std::make_shared<Skill>("折背", "折背攻击", SkillEffectType::BASIC_ATTACK, 3,
        4.0, 1.5, 40.0, 35.0, 3.0, 1.0);
}

std::shared_ptr<Skill> SkillFactory::createBoxingStraight() {
    return std::make_shared<Skill>("拳术直拳", "拳术直拳", SkillEffectType::BASIC_ATTACK, 3,
        0.0, 1.7, 70.0, 20.0, 1.0, 0.7);
}

std::shared_ptr<Skill> SkillFactory::createChargedUppercut() {
    return std::make_shared<Skill>("蓄力上勾拳", "蓄力上勾拳", SkillEffectType::BASIC_ATTACK, 3,
        0.0, 2.0, 60.0, 25.0, 1.0, 0.8);
}

std::shared_ptr<Skill> SkillFactory::createKarateChop() {
    return std::make_shared<Skill>("空手道劈斩", "空手道劈砍", SkillEffectType::BASIC_ATTACK, 4,
        2.0, 1.5, 20.0, 55.0, 2.0, 1.2);
}

std::shared_ptr<Skill> SkillFactory::createQuickStrike() {
    return std::make_shared<Skill>("迅击", "迅捷攻击", SkillEffectType::BASIC_ATTACK, 4,
        4.0, 1.5, 60.0, 20.0, 2.0, 0.6);
}

std::shared_ptr<Skill> SkillFactory::createCloseCombat() {
    return std::make_shared<Skill>("近战缠斗", "近身缠斗", SkillEffectType::BASIC_ATTACK, 4,
        1.0, 2.5, 60.0, 20.0, 1.0, 0.7);
}

// ==================== 属性增益技能 ====================
std::shared_ptr<Skill> SkillFactory::createStandFirm1() {
    return std::make_shared<Skill>("屹立不倒", "敏捷不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner1() {
    return std::make_shared<Skill>("肉食跑者", "耐力不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory1() {
    return std::make_shared<Skill>("肌肉记忆", "力量不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "strength");
}

std::shared_ptr<Skill> SkillFactory::createStandFirm2() {
    return std::make_shared<Skill>("屹立不倒2", "敏捷不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner2() {
    return std::make_shared<Skill>("肉食跑者2", "耐力不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory2() {
    return std::make_shared<Skill>("肌肉记忆2", "力量不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "strength");
}

std::shared_ptr<Skill> SkillFactory::createStandFirm3() {
    return std::make_shared<Skill>("屹立不倒3", "敏捷不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "agility");
}

std::shared_ptr<Skill> SkillFactory::createCarnivorousRunner3() {
    return std::make_shared<Skill>("肉食跑者3", "耐力不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "stamina");
}

std::shared_ptr<Skill> SkillFactory::createMuscleMemory3() {
    return std::make_shared<Skill>("肌肉记忆3", "力量不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "strength");
}

// ==================== 训练效率技能 ====================
std::shared_ptr<Skill> SkillFactory::createSwiftness() {
    return std::make_shared<Skill>("迅速", "敏捷训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "agility", 1.5);
}

std::shared_ptr<Skill> SkillFactory::createWillpower() {
    return std::make_shared<Skill>("意志", "耐力训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "stamina", 1.5);
}

std::shared_ptr<Skill> SkillFactory::createMotivation() {
    return std::make_shared<Skill>("激励", "力量训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "strength", 1.5);
}



// ==================== 特殊效果技能 ====================
std::shared_ptr<Skill> SkillFactory::createFlashStrike() {
    return std::make_shared<Skill>("闪击", "25%概率反伤25%对面造成的伤害", SkillEffectType::DAMAGE_REFLECT, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.25);
}

std::shared_ptr<Skill> SkillFactory::createSuicideAttack() {
    return std::make_shared<Skill>("自杀式袭击", "命中率降低10%，能量消耗降低20%", SkillEffectType::HIT_RATE_MODIFIER, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, -0.1);
}

std::shared_ptr<Skill> SkillFactory::createThousandHands() {
    return std::make_shared<Skill>("千手不破", "受到对面上肢攻击降低30%，下肢攻击增加20%", SkillEffectType::ATTACK_TYPE_DEFENSE, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.3);
}

std::shared_ptr<Skill> SkillFactory::createHumanHammer() {
    return std::make_shared<Skill>("人身重锤", "25%概率降低对面10耐力", SkillEffectType::ENEMY_ATTRIBUTE_MOD, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 10.0);
}

std::shared_ptr<Skill> SkillFactory::createBluntWeapon() {
    return std::make_shared<Skill>("钝兵挫锐", "免伤20%", SkillEffectType::DAMAGE_REDUCTION, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.2);
}

std::shared_ptr<Skill> SkillFactory::createInfiniteEnergy() {
    return std::make_shared<Skill>("无限能量", "如果体力低于50%，能量消耗降低15%", SkillEffectType::STAMINA_MODIFIER, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.15);
}

std::shared_ptr<Skill> SkillFactory::createBoxer() {
    return std::make_shared<Skill>("拳击手", "命中率在原本基础上提高10%", SkillEffectType::GLOBAL_HIT_RATE_BOOST, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.1);
}

// ==================== 工具方法 ====================
std::shared_ptr<Skill> SkillFactory::createSkillByName(const std::string& skillName) {
    // 可以使用映射表来按名称创建技能
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
        "空手道劈斩", "迅击", "近战缠斗",
        "屹立不倒", "肉食跑者", "肌肉记忆", "屹立不倒2", "肉食跑者2", "肌肉记忆2",
        "屹立不倒3", "肉食跑者3", "肌肉记忆3",
        "迅速", "意志", "激励",
        "闪击", "自杀式袭击", "千手不破", "人身重锤", "钝兵挫锐", "无限能量", "拳击手"
    };
}