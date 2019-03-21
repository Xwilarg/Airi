#ifndef UTILS_HPP_
# define UTILS_HPP_

# include <vector>
# include "PixelInfo.hpp"
# include "PixelBlock.hpp"

namespace KotodamaAiri
{
	class Utils
	{
	public:
		[[nodiscard]] static int Distance(const Vector2& a, const Vector2& b) noexcept;
		[[nodiscard]] static int PowTwo(int x) noexcept;
	};
}

#endif // !UTILS_HPP_