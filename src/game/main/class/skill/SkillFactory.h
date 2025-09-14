#ifndef SKILLFACTORY_H
#define SKILLFACTORY_H

#include <vector>
#include <memory>
#include <string>
#include <map> 
#include <unordered_map>
#include "../skill/Skill.h"

class SkillFactory {
public:
    // 基础攻击技能
    static std::shared_ptr<Skill> createPunch();
    static std::shared_ptr<Skill> createKick();
    static std::shared_ptr<Skill> createHighKick();
    static std::shared_ptr<Skill> createHeavyPunch();
    static std::shared_ptr<Skill> createUppercut();
    static std::shared_ptr<Skill> createBackhandHeavyPunch();
    static std::shared_ptr<Skill> createBackhandPunch();
    static std::shared_ptr<Skill> createCrossPunch();
    static std::shared_ptr<Skill> createKarateKick();
    static std::shared_ptr<Skill> createKarateHighKick();
    static std::shared_ptr<Skill> createClawAttack();
    static std::shared_ptr<Skill> createBackBreak();
    static std::shared_ptr<Skill> createBoxingStraight();
    static std::shared_ptr<Skill> createChargedUppercut();
    static std::shared_ptr<Skill> createKarateChop();
    static std::shared_ptr<Skill> createQuickStrike();
    static std::shared_ptr<Skill> createCloseCombat();

    static std::shared_ptr<Skill> createLongPunch();    // 第一个人机对手专属 玩家不可学习

    // 属性增益技能
    static std::shared_ptr<Skill> createStandFirm1();
    static std::shared_ptr<Skill> createCarnivorousRunner1();
    static std::shared_ptr<Skill> createMuscleMemory1();
    static std::shared_ptr<Skill> createStandFirm2();
    static std::shared_ptr<Skill> createCarnivorousRunner2();
    static std::shared_ptr<Skill> createMuscleMemory2();
    static std::shared_ptr<Skill> createStandFirm3();
    static std::shared_ptr<Skill> createCarnivorousRunner3();
    static std::shared_ptr<Skill> createMuscleMemory3();

    // 训练效率技能
    static std::shared_ptr<Skill> createSwiftness();
    static std::shared_ptr<Skill> createWillpower();
    static std::shared_ptr<Skill> createMotivation();

    // 特殊效果技能
    static std::shared_ptr<Skill> createFlashStrike();
    static std::shared_ptr<Skill> createSuicideAttack();
    static std::shared_ptr<Skill> createThousandHands();
    static std::shared_ptr<Skill> createHumanHammer();
    static std::shared_ptr<Skill> createBluntWeapon();
    static std::shared_ptr<Skill> createInfiniteEnergy();
    static std::shared_ptr<Skill> createBoxer();

    // 按名称创建技能
    static std::shared_ptr<Skill> createSkillByName(const std::string& skillName);
    
    // 获取所有技能名称（用于UI显示等）
    static std::vector<std::string> getAllSkillNames();
    
    // 获取技能ID映射
    static std::map<std::string, int> getSkillNameToIdMap();
};

#endif // SKILLFACTORY_H