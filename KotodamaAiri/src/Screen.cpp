#include <iostream>
#include <climits>
#include "../inc/Screen.hpp"
#include "../inc/PixelBlock.hpp"

namespace KotodamaAiri
{
	Screen::Screen() noexcept
		: _desktopWindow(GetDesktopWindow()), _desktopDc(GetDC(_desktopWindow)), _captureDc(CreateCompatibleDC(_desktopDc)),
		_width(GetDeviceCaps(_desktopDc, HORZRES)), _height(GetDeviceCaps(_desktopDc, VERTRES)),
		_screenSize(_width * _height), _captureBitmap(CreateCompatibleBitmap(_desktopDc, _width, _height)),
		_bmi(), _pixels(new RGBQUAD[_screenSize])
	{
		SelectObject(_captureDc, _captureBitmap);
		_bmi.bmiHeader.biSize = sizeof(_bmi.bmiHeader);
		_bmi.bmiHeader.biWidth = _width;
		_bmi.bmiHeader.biHeight = _height;
		_bmi.bmiHeader.biPlanes = static_cast<WORD>(GetDeviceCaps(_desktopDc, PLANES));
		_bmi.bmiHeader.biBitCount = static_cast<WORD>(GetDeviceCaps(_desktopDc, BITSPIXEL));
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
		std::cout << "Trying to find map...";
		HDC hDC_Desktop = GetDC(0);
		HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
		RECT r;
		do
		{
			r = FindMap();
		} while (r.left == 0);
		Vector2 minV(r.left, r.top);
		Vector2 maxV(r.right, r.bottom);
		std::cout << " OK" << std::endl;
		while (true)
		{
			RECT rect = FindPlayer(minV, maxV);
			if (rect.left != 0)
				FillRect(hDC_Desktop, &rect, blueBrush);
		}
	}

	RECT Screen::FindPlayer(const Vector2& min, const Vector2& max) noexcept
	{
		std::vector<PixelBlock> players = FindObject(255, 250, 250, 50, min, max);
		RECT rect;
		if (players.size() == 1)
		{
			PixelBlock player = players[0];
			const Vector2& pos = player.GetUpperLeft();
			const Vector2& size = player.GetSize();
			rect = { pos._x, pos._y, pos._x + size._x, pos._y + size._y };
		}
		else
			rect = { 0, 0, 0, 0 };
		return (rect);
	}

	RECT Screen::FindMap() noexcept
	{
		std::vector<PixelBlock> pixels = FindObject(214, 138, 52, 5, Vector2(0, 0), Vector2(_width, _height));
		for (int i = static_cast<int>(pixels.size()) - 1; i >= 0; i--)
		{
			Vector2 size = pixels[i].GetSize();
			if (!pixels[i].IsSquared(5) || size._x < 2 || size._y < 2)
				pixels.erase(pixels.begin() + i);
		}
		for (int i = static_cast<int>(pixels.size()) - 1; i >= 0; i--)
		{
			bool haveSameX = false;
			const auto& px = pixels[i];
			for (const auto& p : pixels)
			{
				if (px == p)
					continue;
				if (p.IsCloseX(px))
				{
					haveSameX = true;
					break;
				}
			}
			if (!haveSameX)
				pixels.erase(pixels.begin() + i);
		}
		if (pixels.size() != 4)
		{
			RECT rect = { 0, 0, 0, 0 };
			return (rect);
		}
		int leftPos = INT_MAX;
		int rightPos = 0;
		int upPos = INT_MAX;
		int downPos = 0;
		for (const auto& p : pixels)
		{
			const Vector2& pos = p.GetUpperLeft();
			if (pos._x < leftPos)
				leftPos = pos._x;
			else if (pos._x > rightPos)
				rightPos = pos._x;
			if (pos._y < upPos)
				upPos = pos._y;
			else if (pos._y > downPos)
				downPos = pos._y;
		}
		RECT rect = { leftPos, upPos, rightPos, downPos };
		return (rect);
	}

	std::vector<PixelBlock> Screen::FindObject(int red, int green, int blue, int offset, const Vector2& min, const Vector2& max) noexcept
	{
		UpdatePixels();
		std::vector<PixelBlock> pixels;
		for (const auto& p : GetPixels(red - offset, red + offset, green - offset, green + offset, blue - offset, blue + offset, min, max))
		{
			bool pxFound = false;
			Vector2 pos = Vector2(p._x, p._y);
			for (PixelBlock& px : pixels)
			{
				if (px.AddPixel(pos))
				{
					pxFound = true;
					break;
				}
			}
			if (!pxFound)
				pixels.emplace_back(pos);
		}
		return (pixels);
	}

	std::vector<PixelInfo> Screen::GetPixels(int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax, const Vector2& min, const Vector2& max) const noexcept
	{
		std::vector<PixelInfo> newPixels;
		for (int i = 0; i < _screenSize; i++)
		{
			const RGBQUAD& quad = _pixels[i];
			int red = quad.rgbRed;
			int green = quad.rgbGreen;
			int blue = quad.rgbBlue;
			if (red >= redMin && red <= redMax && green >= greenMin && green <= greenMax && blue >= blueMin && blue <= blueMax)
			{
				int x = i % _width;
				int y = _height - i / _width;
				if (x >= min._x && x <= max._x && y >= min._y && y <= max._y)
					newPixels.emplace_back(x, y, quad);
			}
		}
		return (newPixels);
	}

	void Screen::UpdatePixels() noexcept
	{
		BitBlt(_captureDc, 0, 0, _width, _height, _desktopDc, 0, 0, SRCCOPY | CAPTUREBLT);
		GetDIBits(_captureDc, _captureBitmap, 0, _height, _pixels, &_bmi, DIB_RGB_COLORS);
	}
}