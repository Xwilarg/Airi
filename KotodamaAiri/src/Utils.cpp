#include "../inc/Utils.hpp"

namespace KotodamaAiri
{
	// Sqrt is actually an expensive operation so we don't do it (we just use distance to compare them anyway)
	int Utils::Distance(const Vector2 & a, const Vector2 & b) noexcept
	{
		return (PowTwo(a._x - b._x) + PowTwo(a._y - b._y));
	}

	int Utils::PowTwo(int x) noexcept
	{
		return (x * x);
	}
}