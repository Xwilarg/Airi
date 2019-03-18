#ifndef SCREEN_HPP_
# define SCREEN_HPP_

# include <vector>
# include <Windows.h>

namespace KotodamaAiri
{
	class Screen
	{
	private:
		struct PixelInfo
		{
			PixelInfo(int x, int y, const RGBQUAD &color)
				: _x(x), _y(y), _color(color)
			{ }
			int _x, _y;
			const RGBQUAD& _color;
		};

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
