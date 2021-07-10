#include "Utils.hpp"

namespace Airi
{
	// Sqrt is actually an expensive operation so we don't do it (we just use distance to compare them anyway)
	int Utils::Distance(const Vector2 & a, const Vector2 & b) noexcept
	{
		return PowTwo(a.X - b.X) + PowTwo(a.Y - b.Y);
	}

	int Utils::PowTwo(int x) noexcept
	{
		return x * x;
	}
}