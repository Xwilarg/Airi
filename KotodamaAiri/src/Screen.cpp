#include <iostream>
#include "../inc/Screen.hpp"

namespace KotodamaAiri
{
	Screen::Screen() noexcept
		: _desktopWindow(GetDesktopWindow()), _desktopDc(GetDC(_desktopWindow)), _captureDc(CreateCompatibleDC(_desktopDc)),
		_width(GetDeviceCaps(_desktopDc, HORZRES)), _height(GetDeviceCaps(_desktopDc, VERTRES)),
		_screenSize(_width * _height), _captureBitmap(CreateCompatibleBitmap(_desktopDc, _width, _height)),
		_bmi(), _pixels(new RGBQUAD[_screenSize])
	{
		SelectObject(_captureDc, _captureBitmap);
		BitBlt(_captureDc, 0, 0, _width, _height, _desktopDc, 0, 0, SRCCOPY | CAPTUREBLT);
		_bmi.bmiHeader.biSize = sizeof(_bmi.bmiHeader);
		_bmi.bmiHeader.biWidth = _width;
		_bmi.bmiHeader.biHeight = _height;
		_bmi.bmiHeader.biPlanes = GetDeviceCaps(_desktopDc, PLANES);
		_bmi.bmiHeader.biBitCount = GetDeviceCaps(_desktopDc, BITSPIXEL);
		_bmi.bmiHeader.biCompression = BI_RGB;
	}

	Screen::~Screen() noexcept
	{
		delete[] _pixels;
		ReleaseDC(_desktopWindow, _desktopDc);
		DeleteDC(_captureDc);
		DeleteObject(_captureBitmap);
	}

	void Screen::CheckPixels() noexcept
	{
		int redObjMin = 214 - 5;
		int redObjMax = 214 + 5;
		int greenObjMin = 138 - 5;
		int greenObjMax = 138 + 5;
		int blueObjMin = 52 - 5;
		int blueObjMax = 52 + 5;
		GetDIBits(_captureDc, _captureBitmap, 0, _height, _pixels, &_bmi, DIB_RGB_COLORS);
		for (int i = 0; i < _screenSize; i++)
		{
			const RGBQUAD& quad = _pixels[i];
			int red = quad.rgbRed;
			int green = quad.rgbGreen;
			int blue = quad.rgbBlue;
			if (red >= redObjMin && red <= redObjMax && green >= greenObjMin && green <= greenObjMax && blue >= blueObjMin && blue <= blueObjMax)
				std::cout << "(" << (i % _width) << ";" << (_height - i / _width) << "): " << "(" << red << ";" << green << ";" << blue << ")" << std::endl;
		}
	}
}