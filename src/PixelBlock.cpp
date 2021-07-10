#include "PixelBlock.hpp"
#include "Utils.hpp"

namespace Airi
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
		if (pos.X < _upperLeft.X)
			_upperLeft.X = pos.X;
		else if (pos.X > _lowerRight.X)
			_lowerRight.X = pos.X;
		if (pos.Y < _upperLeft.Y)
			_upperLeft.Y = pos.Y;
		else if (pos.Y > _lowerRight.Y)
			_lowerRight.Y = pos.Y;
		return (true);
	}

	bool PixelBlock::IsCloseX(const PixelBlock& pb) const noexcept
	{
		return (_upperLeft.X == pb._upperLeft.X);
	}

	RECT PixelBlock::GetRect() const noexcept
	{
		RECT r { _upperLeft.X, _upperLeft.Y, _lowerRight.X, _lowerRight.Y };
		return (r);
	}

	Vector2 PixelBlock::GetSize() const noexcept
	{
		return (Vector2(_lowerRight.X - _upperLeft.X, _lowerRight.Y - _upperLeft.Y));
	}

	bool PixelBlock::IsSquared(int margin) const noexcept
	{
		Vector2 size = GetSize();
		int sizeInt = size.X - size.Y;
		if (sizeInt < 0)
			sizeInt = -sizeInt;
		return (sizeInt < margin);
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