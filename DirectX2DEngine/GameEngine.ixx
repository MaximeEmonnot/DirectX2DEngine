export module GameEngine;

export import :Actors;
export import :Controller;
export import :Fighters;
export import :Levels;
export import :UI;

export namespace Commands
{
	constexpr unsigned char LEFT = 'Q';
	constexpr unsigned char DOWN = 'S';
	constexpr unsigned char RIGHT = 'D';

	constexpr unsigned char JUMP = 'Z';

	constexpr unsigned char PUNCH = 'Y';
	constexpr unsigned char KICK = 'U';
	constexpr unsigned char SLASH = 'I';
	constexpr unsigned char HARDSLASH = 'O';
}