#include "../inc/PixelBlock.hpp"

namespace KotodamaAiri
{
	PixelBlock::PixelBlock(const Vector2 &pos) noexcept
		: _lowerRight(pos), _upperLeft(pos)
	{ }

	bool PixelBlock::AddPixel(const Vector2& pos) noexcept
	{
		if (!IsClose(pos, 100))
			return (false);
		if (pos._x < _upperLeft._x)
			_upperLeft._x = pos._x;
		else if (pos._x > _lowerRight._x)
			_lowerRight._x = pos._x;
		if (pos._y < _upperLeft._y)
			_upperLeft._y = pos._y;
		else if (pos._y > _lowerRight._y)
			_lowerRight._y = pos._y;
		return (true);
	}

	RECT PixelBlock::GetRect() const noexcept
	{
		RECT r { _upperLeft._x, _upperLeft._y, _lowerRight._x, _lowerRight._y };
		return (r);
	}

	bool PixelBlock::IsClose(const Vector2 &pos, int limit) const noexcept
	{
		return (Distance(pos, _upperLeft) < limit || Distance(pos, _lowerRight) < limit);
	}

	int PixelBlock::Distance(const Vector2& a, const Vector2& b) const noexcept
	{
		return (PowTwo(a._x - b._x) + PowTwo(a._y - b._y));
	}

	int PixelBlock::PowTwo(int x) const noexcept
	{
		return (x * x);
	}
}