#ifndef PIXELBLOCK_HPP_
# define PIXELBLOCK_HPP_

# include <Windows.h>
# include "Vector2.hpp"

namespace KotodamaAiri
{
	class PixelBlock
	{
	public:
		PixelBlock(const Vector2 &_pos) noexcept;
		bool AddPixel(const Vector2& pos) noexcept;
		[[nodiscard]] RECT GetRect() const noexcept;

	private:
		[[nodiscard]] bool IsClose(const Vector2& pos, int limit) const noexcept;
		[[nodiscard]] int Distance(const Vector2& a, const Vector2& b) const noexcept;
		[[nodiscard]] int PowTwo(int x) const noexcept;
		Vector2 _lowerRight, _upperLeft;
	};
}

#endif // !PIXELBLOCK_HPP_