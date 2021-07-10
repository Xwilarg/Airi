#pragma once

# include <Windows.h>
# include "Vector2.hpp"

namespace Airi
{
	class PixelBlock
	{
	public:
		PixelBlock(const Vector2 &_pos) noexcept;
		bool operator==(const PixelBlock& pb) const noexcept;
		bool operator!=(const PixelBlock& pb) const noexcept;
		bool AddPixel(const Vector2& pos) noexcept;
		[[nodiscard]] RECT GetRect() const noexcept;
		[[nodiscard]] Vector2 GetSize() const noexcept;
		[[nodiscard]] bool IsSquared(int margin) const noexcept;
		[[nodiscard]] bool IsCloseX(const PixelBlock &pb) const noexcept;
		[[nodiscard]] const Vector2& GetUpperLeft() const noexcept;

	private:
		[[nodiscard]] bool IsClose(const Vector2& pos, int limit) const noexcept;
		Vector2 _lowerRight, _upperLeft;
		int _id;
		static int id;
	};
}
