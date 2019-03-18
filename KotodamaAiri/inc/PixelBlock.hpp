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
		[[nodiscard]] int Distance(const Vector2& a, const Vector2& b) const noexcept;
		[[nodiscard]] int PowTwo(int x) const noexcept;
		Vector2 _lowerRight, _upperLeft;
		int _id;
		static int id;
	};
}

#endif // !PIXELBLOCK_HPP_