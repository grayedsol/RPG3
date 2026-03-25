#pragma once
#include <stdint.h>

namespace Battle {
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
}
