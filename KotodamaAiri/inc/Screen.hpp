#ifndef SCREEN_HPP_
# define SCREEN_HPP_

# include <vector>
# include "PixelInfo.hpp"

namespace KotodamaAiri
{
	class Screen
	{
	public:
		Screen() noexcept;
		~Screen() noexcept;
		void CheckPixels() noexcept;

	private:
		std::vector<PixelInfo> GetPixels(int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax) const noexcept;
		void UpdatePixels() noexcept;
		HWND _desktopWindow;
		HDC _desktopDc, _captureDc;
		const int _width, _height;
		const int _screenSize;
		HBITMAP _captureBitmap;
		BITMAPINFO _bmi;
		RGBQUAD *_pixels;
	};
}

#endif // !SCREEN_HPP_