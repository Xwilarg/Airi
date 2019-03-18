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
		int _x, _y;
		const RGBQUAD& _color;
	};
}

#endif // !PIXELINFO_HPP_