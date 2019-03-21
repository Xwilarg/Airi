#include "../inc/PixelBlock.hpp"
#include "../inc/Utils.hpp"

namespace KotodamaAiri
{
	PixelBlock::PixelBlock(const Vector2 &pos) noexcept
		: _lowerRight(pos), _upperLeft(pos), _id(id++)
	{ }

	bool PixelBlock::operator==(const PixelBlock& pb) const noexcept
	{
		return (_id == pb._id);
	}

	bool PixelBlock::operator!=(const PixelBlock & pb) const noexcept
	{
		return (_id != pb._id);
	}

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

	Vector2 PixelBlock::GetSize() const noexcept
	{
		return (Vector2(_lowerRight._x - _upperLeft._x, _lowerRight._y - _upperLeft._y));
	}

	bool PixelBlock::IsSquared(int margin) const noexcept
	{
		Vector2 size = GetSize();
		int sizeInt = size._x - size._y;
		if (sizeInt < 0)
			sizeInt = -sizeInt;
		return (sizeInt < margin);
	}

	bool PixelBlock::IsCloseX(const PixelBlock& pb) const noexcept
	{
		return (_upperLeft._x == pb._upperLeft._x);
	}

	const Vector2& PixelBlock::GetUpperLeft() const noexcept
	{
		return (_upperLeft);
	}

	bool PixelBlock::IsClose(const Vector2 &pos, int limit) const noexcept
	{
		return (Utils::Distance(pos, _upperLeft) < limit || Utils::Distance(pos, _lowerRight) < limit);
	}

	int PixelBlock::id = 0;
}