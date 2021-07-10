#pragma once

# include <vector>
# include "PixelInfo.hpp"
# include "PixelBlock.hpp"

namespace Airi
{
	class Utils
	{
	public:
		[[nodiscard]] static int Distance(const Vector2& a, const Vector2& b) noexcept;
		[[nodiscard]] static int PowTwo(int x) noexcept;
	};
}