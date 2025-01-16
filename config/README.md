## Setting the default controls
The default controls can be set within controls.json.

Format:
```
{
	"<VIRTUAL_BUTTON_NAME>" : {
		"Primary" : { "code" : <SDL_Scancode/GRY_Mousecode>, ["mouse" = true] }[,
		"Secondary" : { "code" : <SDL_Scancode/GRY_Mousecode>, ["mouse" = true] }]
	},

	...
}
```
Specifying a secondary binding is optional.

For mouse buttons, `"mouse" = true` must be included.
Otherwise, it will be treated as an SDL_Scancode.

Example:
```
{
    "GAME_A" : {
        "Primary" : { "code" : 8 },
        "Secondary" : { "code" : 1, "mouse" : true }
    },

    "GAME_B" : {
        "Primary" : { "code" : 43 }
    }
}
```

A list of SDL_Scancode identifiers can be found here:
https://wiki.libsdl.org/SDL3/SDL_Scancode

#### GRY_Mousecodes:
GRY_MOUSECODE_LEFT = 1
GRY_MOUSECODE_MIDDLE = 2
GRY_MOUSECODE_RIGHT = 3
GRY_MOUSECODE_X1 = 4
GRY_MOUSECODE_X2 = 5

The list of GRY_Mousecode identifiers can also be found in include/GRY_Mousecode.hpp.

#### Virtual Buttons:

GAME_A
GAME_B
GAME_X
GAME_Y
GAME_UP
GAME_DOWN
GAME_LEFT
GAME_RIGHT
GAME_LT
GAME_RT
GAME_LB
GAME_RB
GAME_START
GAME_SELECT

The list of Virtual Button names can also be found in include/VirtualButton.hpp.