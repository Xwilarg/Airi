#ifndef VECTOR2_HPP_
# define VECTOR2_HPP_

namespace KotodamaAiri
{
	struct Vector2
	{
		Vector2(int x, int y)
			: _x(x), _y(y)
		{ }
		int _x, _y;
	};
}

#endif // !VECTOR2_HPP_