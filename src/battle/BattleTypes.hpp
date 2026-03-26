#pragma once
#include <vector>
#include <stdint.h>

namespace Battle {
	using ActorFlags = uint32_t;
	using ActorId = uint8_t;
	using BuffDebuffId = uint8_t;
	using ActionAnimationId = uint16_t;

	enum FighterId : uint8_t {
		Fighter0 = 0,
		Fighter1 = 1,
		Fighter2 = 2,
		Fighter3 = 3,
		MaxFighters = 4
	};

	enum MonsterId : uint8_t {
		Monster0 = 0,
		Monster1 = 1,
		Monster2 = 2,
		Monster3 = 3,
		Monster4 = 4,
		Monster5 = 5,
		Monster6 = 6,
		Monster7 = 7,
		MaxMonsters = 8
	};

	static const ActorId MAX_ACTORS = static_cast<uint8_t>(FighterId::MaxFighters) + static_cast<uint8_t>(MonsterId::MaxMonsters);
	static const uint8_t MAX_NAME_LENGTH = 16;
	static const uint8_t MAX_ACTIONS = 8;

	enum PageId : uint8_t {
		MainPage = 0,
		ItemPage = 1,
		SkillPage = 2
	};

	enum InputId : uint8_t {
		Flee = 0,
		Attack = 1,
		Item = 2,
		Skill = 3,
		ItemDown = 4,
		ItemUp = 5,
		ItemSelect = 6,
		ItemToMain = 7,
		Skill3 = 8,
		Skill1 = 9,
		SkillToMain = 10,
		Skill2 = 11,
		InputIdSize = 12
	};

	enum DirectionId : uint8_t {
		Down = 0,
		Up = 1,
		Left = 2,
		Right = 3,
		DirectionIdSize = 4
	};

	enum ActorFlag : uint8_t {
		ACTOR_EXISTS = 0,
		ACTOR_ALIVE = 1,
		ACTOR_IDLE = 2
	};

	enum AttackElement : uint8_t {
		NO_ELEMENT = 0,
		FIRE_ELEMENT = 1,
		WATER_ELEMENT = 2,
		ELECTRIC_ELEMENT = 3,
		ATTACK_ELEMENTS_SIZE = 4
	};

	enum ActionType : uint8_t {
		NO_ACTION = 0,
		WAIT_ACTION = 1,
		ATTACK = 2,
		HEAL = 3,
		BUFF_DEBUFF = 4,
		MULTI_ATTACK = 5,
		MULTI_HEAL = 6,
		MULTI_BUFF_DEBUFF = 7,
		ANIMATION = 8
	};

	struct PowerFormula {};

	struct NoAction {
		ActionType type = ActionType::NO_ACTION;
		ActorId source = 0;
		uint8_t stance = 0;
		BuffDebuffId buffDebuffId = 0;
		double time = 0; /* Time until the action executes. */
	};

	struct WaitAction {
		ActionType type = ActionType::WAIT_ACTION;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
	};

	struct AttackAction {
		ActionType type = ActionType::ATTACK;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId target;
		uint8_t power;
		PowerFormula formula;
		AttackElement element = AttackElement::NO_ELEMENT;
	};

	struct HealAction {
		ActionType type = ActionType::HEAL;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId target;
		uint8_t power;
		PowerFormula formula;
	};

	struct BuffDebuffAction {
		ActionType type = ActionType::BUFF_DEBUFF;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId target;
		BuffDebuffId appliedBuffDebuffId;
	};

	struct MultiAttackAction {
		ActionType type = ActionType::MULTI_ATTACK;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId targets[MAX_ACTORS];
		uint8_t power;
		PowerFormula formula;
		AttackElement element = AttackElement::NO_ELEMENT;
	};

	struct MultiHealAction {
		ActionType type = ActionType::MULTI_HEAL;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId targets[MAX_ACTORS];
		uint8_t power;
		PowerFormula formula;
	};

	struct MultiBuffDebuffAction {
		ActionType type = ActionType::MULTI_BUFF_DEBUFF;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId targets[MAX_ACTORS];
		BuffDebuffId appliedBuffDebuffId;
	};

	struct AnimationAction {
		ActionType type = ActionType::ANIMATION;
		ActorId source;
		uint8_t stance;
		BuffDebuffId buffDebuffId;
		double time;
		ActorId target;
		ActionAnimationId animationId;
	};

	union Action {
		NoAction data = NoAction();
		WaitAction waitAction;
		AttackAction attackAction;
		HealAction healAction;
		BuffDebuffAction buffDebuffAction;
		MultiAttackAction multiAttackAction;
		MultiHealAction multiHealAction;
		MultiBuffDebuffAction multiBuffDebuffAction;
	};

	struct Actors {
		char names[MAX_ACTORS][MAX_NAME_LENGTH] = { 0 };
		ActorFlags flags[MAX_ACTORS] = { 0 };
		double timers[MAX_ACTORS] = { 0 };
		Action executingActions[MAX_ACTORS];
		std::vector<Action> actionLists[MAX_ACTORS]; /* The current Action of an actor is actionLists[actor].back(). */
	};
}
