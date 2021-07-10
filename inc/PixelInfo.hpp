#pragma once

# include <Windows.h>

namespace Airi
{
	struct PixelInfo
	{
		PixelInfo(int x, int y, const RGBQUAD& color)
			: X(x), Y(y), Color(color)
		{ }
		bool operator==(const PixelInfo& pl) const noexcept
		{
			return (X == pl.X && Y == pl.Y
				&& Color.rgbRed == pl.Color.rgbRed
				&& Color.rgbGreen == pl.Color.rgbGreen
				&& Color.rgbBlue == pl.Color.rgbBlue);
		}
		int X, Y;
		const RGBQUAD& Color;
	};
}