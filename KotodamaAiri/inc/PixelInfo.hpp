#ifndef PIXELINFO_HPP_
# define PIXELINFO_HPP_

# include <Windows.h>

namespace KotodamaAiri
{
	struct PixelInfo
	{
		PixelInfo(int x, int y, const RGBQUAD& color)
			: _x(x), _y(y), _color(color)
		{ }
		bool operator==(const PixelInfo& pl) const noexcept
		{
			return (_x == pl._x && _y == pl._y
				&& _color.rgbRed == pl._color.rgbRed
				&& _color.rgbGreen == pl._color.rgbGreen
				&& _color.rgbBlue == pl._color.rgbBlue);
		}
		int _x, _y;
		const RGBQUAD& _color;
	};
}

#endif // !PIXELINFO_HPP_