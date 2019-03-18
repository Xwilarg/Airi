#ifndef SCREEN_HPP_
# define SCREEN_HPP_

# include <Windows.h>

namespace KotodamaAiri
{
	class Screen
	{
	public:
		Screen() noexcept;
		~Screen() noexcept;
		void CheckPixels() noexcept;

	private:
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
