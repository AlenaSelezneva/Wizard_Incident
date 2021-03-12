#pragma once

namespace Tile
{
	enum Type
	{
		None = 0,			// 000000000
		Floor = 1 << 0,		// 000000001
		Wall = 1 << 1,
		InvisibleWall = 1 << 2,
	};
}

