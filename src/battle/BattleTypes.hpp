#pragma once
#include <vector>
#include <stdint.h>

namespace Battle {
	using ActorFlags = uint32_t;
	using ActorId = uint8_t;
	using ActionAnimationId = uint16_t;

	struct FX {
		/**
		 * @brief Index of the frame to render.
		 * 
		 */
		uint32_t index = 0;

		/**
		 * @brief Index of the FXResource to use.
		 *
		 */
		uint32_t fx = 0;

		/**
		 * @brief Time that each frame should be displayed for, in seconds. 0 indicates no FX.
		 * 
		 */
		double duration = 0;

		/**
		 * @brief Time left until the next frame, in seconds.
		 * 
		 */
		double timer = 0;
	};

	/**
	 * @brief Index used for fighters.
	 * 
	 */
	enum FighterId : uint8_t {
		Fighter0 = 0,
		Fighter1 = 1,
		Fighter2 = 2,
		Fighter3 = 3,
		MaxFighters = 4
	};

	/**
	 * @brief Index used for monsters.
	 * 
	 */
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

	/**
	 * @brief The total number of possible fighters and monsters.
	 * 
	 */
	static const ActorId MAX_ACTORS = static_cast<uint8_t>(FighterId::MaxFighters) + static_cast<uint8_t>(MonsterId::MaxMonsters);
	static const uint8_t MAX_NAME_LENGTH = 16;
	static const uint8_t MAX_ACTIONS = 8;

	enum PageId : uint8_t {
		MainPage = 0,
		ItemPage = 1,
		SkillPage = 2
	};

	enum InputId : uint8_t {
		InputPass = 0,
		InputAttack = 1,
		InputItem = 2,
		InputSkillPage = 3,
		InputItemDown = 4,
		InputItemUp = 5,
		InputItemSelect = 6,
		InputItemToMainPage = 7,
		InputSkill3 = 8,
		InputSkill1 = 9,
		InputSkillToMainPage = 10,
		InputSkill2 = 11,
		InputIdSize = 12
	};

	enum DirectionId : uint8_t {
		Down = 0,
		Up = 1,
		Left = 2,
		Right = 3,
		DirectionIdSize = 4
	};

	/**
	 * @brief Index used for fighter moves. A move is a list of actions.
	 * 
	 */
	enum FighterMove : uint8_t {
		FIGHTER_ATTACK = 0,
		FIGHTER_SKILL1 = 1,
		FIGHTER_SKILL2 = 2,
		FIGHTER_SKILL3 = 3,
		FIGHTER_NUM_SKILLS = 3,
		FIGHTER_NUM_MOVES = 4
	};

	enum ActorFlag : ActorFlags {
		ACTOR_EXISTS = 1 << 0, /* There is an actor using this ActorId. */
		ACTOR_ALIVE = 1 << 1, /* The actor is alive. */
		ACTOR_IDLE = 1 << 2 /* The actor is not currently in the middle of a move. */
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
		ATTACK_ACTION = 2,
		HEAL_ACTION = 3,
		STATUS_EFFECT_ACTION = 4,
		MULTI_ATTACK_ACTION = 5,
		MULTI_HEAL_ACTION = 6,
		MULTI_STATUS_EFFECT_ACTION = 7,
		ANIMATION_ACTION = 8
	};

	enum StatusEffectId : uint8_t {
		STATUS_EFFECT_NONE = 0
	};

	struct PowerFormula {};

	struct StatusEffect {
		double timer = 0;
		StatusEffectId id = StatusEffectId::STATUS_EFFECT_NONE;
	};

	struct CommonActionData {
		ActionType type = ActionType::NO_ACTION;
		ActorId source = 0;
		uint8_t stance = 0;
		StatusEffectId statusEffectId = StatusEffectId::STATUS_EFFECT_NONE;
		double time = 0; /* Time until the action executes. */
	};

	struct NoAction {
		CommonActionData data{ActionType::NO_ACTION};
	};

	struct WaitAction {
		CommonActionData data{ActionType::WAIT_ACTION};
	};

	struct AttackAction {
		CommonActionData data{ActionType::ATTACK_ACTION};
		ActorId target;
		uint8_t power;
		PowerFormula formula;
		AttackElement element = AttackElement::NO_ELEMENT;
	};

	struct HealAction {
		CommonActionData data{ActionType::HEAL_ACTION};
		ActorId target;
		uint8_t power;
		PowerFormula formula;
	};

	struct StatusEffectAction {
		CommonActionData data{ActionType::STATUS_EFFECT_ACTION};
		ActorId target;
		StatusEffect appliedStatusEffect;
	};

	struct MultiAttackAction {
		CommonActionData data{ActionType::MULTI_ATTACK_ACTION};
		ActorId targets[MAX_ACTORS];
		uint8_t power;
		PowerFormula formula;
		AttackElement element = AttackElement::NO_ELEMENT;
	};

	struct MultiHealAction {
		CommonActionData data{ActionType::MULTI_HEAL_ACTION};
		ActorId targets[MAX_ACTORS];
		uint8_t power;
		PowerFormula formula;
	};

	struct MultiStatusEffectAction {
		CommonActionData data{ActionType::MULTI_STATUS_EFFECT_ACTION};
		ActorId targets[MAX_ACTORS];
		StatusEffect appliedStatusEffect;
	};

	struct AnimationAction {
		CommonActionData data{ActionType::ANIMATION_ACTION};
		ActorId target;
		ActionAnimationId animationId;
	};

	union Action {
		NoAction common = NoAction();
		WaitAction waitAction;
		AttackAction attackAction;
		HealAction healAction;
		StatusEffectAction statusEffectAction;
		MultiAttackAction multiAttackAction;
		MultiHealAction multiHealAction;
		MultiStatusEffectAction multiStatusEffectAction;
		AnimationAction animationAction;
	};

	struct Actors {
		const char* names[MAX_ACTORS] = { 0 };
		ActorFlags flags[MAX_ACTORS] = { 0 };
		double timers[MAX_ACTORS] = { 0 };
		Action executingActions[MAX_ACTORS];
		std::vector<Action> actionLists[MAX_ACTORS]; /* The current Action of an actor is actionLists[actor].back(). */
		std::vector<StatusEffect> statusEffects[MAX_ACTORS];
		FX fx[MAX_ACTORS];
		int healthPoints[MAX_ACTORS] = { 0 };
	};

	struct Fighters {
		std::vector<Action> moves[FighterId::MaxFighters][FighterMove::FIGHTER_NUM_MOVES];
		std::vector<char> skillNames[FighterId::MaxFighters][FighterMove::FIGHTER_NUM_SKILLS];
	};
}
