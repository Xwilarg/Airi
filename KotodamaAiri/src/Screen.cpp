#include <iostream>
#include <climits>
#include <string>
#include <ctime>
#include "../inc/Screen.hpp"
#include "../inc/PixelBlock.hpp"
#include "../inc/Utils.hpp"

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
		// Load brush to draw information on screen
		HDC hDC_Desktop = GetDC(0);
		HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));

	retry:
		std::cout << "To begin, left click on the center of the minimap to locate it." << std::endl;
		while (true) // Wait for player to do a left click somewhere
		{
			if (GetKeyState(VK_LBUTTON) < 0)
				break;
		}
		POINT p;
		GetCursorPos(&p);
		RECT r { p.x - 125, p.y - 125, p.x + 125, p.y + 125 }; // The mini map is around 500*500px
		std::cout << "Press Y to confirm or N to retry." << std::endl;
		while (true)
		{
			if (GetKeyState('Y') & 0x8000)
				break;
			else if (GetKeyState('N') & 0x8000)
				goto retry;
			FillRect(hDC_Desktop, &r, blueBrush);
		}

		// minV and maxV are the bounds of the mini map
		// that allow us to limitate our search to a smaller part of the screen
		Vector2 minV(r.left, r.top);
		Vector2 maxV(r.right, r.bottom);
		std::cout << "Map located, you can press Q at any time to exit." << std::endl;
		std::cout << "Locating player...";
		RECT rect;
		do
		{
			// The player is at the center of the mini map, we just locate it once to be sure of it exact position
			rect = FindPlayer(minV, maxV, p);
		} while (rect.left == 0);
		std::cout << " OK" << std::endl;
		std::string finalMsg = "";
		Vector2 playerDistRef = Vector2(rect.left, rect.top);
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		const int baseCamSpeed = 100;
		int currCamSpeed = baseCamSpeed;
		Direction goLeft = UNDEFINED;
		srand(static_cast<unsigned int>(time(nullptr)));
		while (true)
		{
			std::cout << std::string(finalMsg.length(), '\b');
			finalMsg = "Player position: (" + std::to_string((rect.right + rect.left) / 2) + " ; " + std::to_string((rect.top + rect.bottom) / 2) + ")";
			std::vector<RECT> allAllies = FindAllies(minV, maxV);
			finalMsg += ", Allies found: " + std::to_string(allAllies.size());

			// Get the closest allie
			int minDist;
			RECT closest;

			if (allAllies.size() > 0)
			{
				closest = GetClosest(allAllies, minDist, playerDistRef);

				finalMsg += " (" + std::to_string((closest.right + closest.left) / 2) + " ; " + std::to_string((closest.top + closest.bottom) / 2) + ")";
				GetCursorPos(&p);
				if (closest.top > playerDistRef._y) // If allie is behind us we flip the camera
					SetCursorPos(p.x - 500, p.y);
				else if (closest.left < playerDistRef._x)
				{
					// If the camera is going too fast to locate an allie, we divide it speed by two
					if (goLeft == RIGHT)
					{
						currCamSpeed /= 2;
						if (currCamSpeed == 0)
							currCamSpeed = 1;
					}
					goLeft = LEFT;
					SetCursorPos(p.x - currCamSpeed, p.y);
					// If the bot is "more or less" looking to an allie, we go forward
					if (currCamSpeed <= 25)
						input.ki.dwFlags = 0;
					else
						input.ki.dwFlags = KEYEVENTF_KEYUP;
				}
				else if (closest.left > playerDistRef._x) // Same thing as above for when right
				{
					if (goLeft == LEFT)
					{
						currCamSpeed /= 2;
						if (currCamSpeed == 0)
							currCamSpeed = 1;
					}
					goLeft = RIGHT;
					SetCursorPos(p.x + currCamSpeed, p.y);
					if (currCamSpeed <= 25)
						input.ki.dwFlags = 0;
					else
						input.ki.dwFlags = KEYEVENTF_KEYUP;
				}
				else // The allie is right forward
				{
					input.ki.dwFlags = 0;
					goLeft = UNDEFINED;
					currCamSpeed = baseCamSpeed;
				}
				input.ki.wVk = 0x57; // W
				// If we begin to move forward, we press W twice to run
				if (input.ki.dwFlags == 0 && GetKeyState('W') >= 0)
				{
					SendInput(1, &input, sizeof(INPUT));
					input.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &input, sizeof(INPUT));
					input.ki.dwFlags = 0;
				}
				SendInput(1, &input, sizeof(INPUT));

				// Spam '3' to use Stella heal when it can
				input.ki.wVk = 0x33; // 3
				input.ki.dwFlags = 0;
				SendInput(1, &input, sizeof(INPUT));
				input.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(INPUT));
			}
			else // If we can't find an allie
			{
				goLeft = UNDEFINED;
				currCamSpeed = baseCamSpeed;
			}

			std::vector<RECT> allEnnemies = FindEnnemies(minV, maxV);
			finalMsg += ", Ennemies found: " + std::to_string(allEnnemies.size());
			if (allEnnemies.size() > 0)
			{
				closest = GetClosest(allEnnemies, minDist, playerDistRef);
				finalMsg += " (" + std::to_string((closest.right + closest.left) / 2) + " ; " + std::to_string((closest.top + closest.bottom) / 2) + ")";
				finalMsg += " (Distance: " + std::to_string(minDist) + ")";
				if (minDist < 400) // If enemies are too close we launch offensive competencies
				{
					input.ki.dwFlags = 0;
					if (rand() % 2 == 0)
						input.ki.wVk = 0x31; // 1
					else
						input.ki.wVk = 0x32; // 2
					SendInput(1, &input, sizeof(INPUT));
					input.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &input, sizeof(INPUT));
				}
			}
			std::cout << finalMsg;
			if (GetKeyState('Q') & 0x8000)
				break;
		}
		DeleteObject(blueBrush);
	}

	RECT Screen::GetClosest(std::vector<RECT> _allRects, int& minDist, const Vector2& playerDistRef) const noexcept
	{
		minDist = -1;
		RECT closest;
		if (_allRects.size() == 1)
		{
			closest = _allRects[0];
			minDist = 0;
		}
		else if (_allRects.size() > 1)
		{
			for (const auto& pos : _allRects)
			{
				int dist = Utils::Distance(playerDistRef, Vector2(pos.left, pos.top));
				if (minDist == -1 || dist < minDist)
				{
					closest = pos;
					minDist = dist;
				}
			}
		}
		return (closest);
	}

	std::vector<RECT> Screen::FindAllies(const Vector2& min, const Vector2& max) noexcept
	{
		std::vector<PixelBlock> players = FindObject(200, 110, 210, 15, min, max); // Purple
		std::vector<PixelBlock> tmp = FindObject(65, 180, 190, 15, min, max); // Blue
		std::vector<PixelBlock> tmp2 = FindObject(74, 215, 218, 15, min, max); // Blue (lighter)
		players.insert(players.end(), tmp.begin(), tmp.end());
		players.insert(players.end(), tmp2.begin(), tmp2.end());
		std::vector<RECT> allRects;
		for (const auto& player : players)
		{
			const Vector2& pos = player.GetUpperLeft();
			const Vector2& size = player.GetSize();
			allRects.push_back({ pos._x, pos._y, pos._x + size._x, pos._y + size._y });
		}
		return (allRects);
	}

	std::vector<RECT> Screen::FindEnnemies(const Vector2& min, const Vector2& max) noexcept
	{
		std::vector<PixelBlock> ennemies = FindObject(185, 110, 100, 15, min, max); // Red
		std::vector<PixelBlock> tmp = FindObject(240, 50, 50, 15, min, max); // Red (lighter)
		ennemies.insert(ennemies.end(), tmp.begin(), tmp.end());
		std::vector<RECT> allRects;
		for (const auto& ennemy : ennemies)
		{
			const Vector2& pos = ennemy.GetUpperLeft();
			const Vector2& size = ennemy.GetSize();
			allRects.push_back({ pos._x, pos._y, pos._x + size._x, pos._y + size._y });
		}
		return (allRects);
	}

	RECT Screen::FindPlayer(const Vector2& min, const Vector2& max, const POINT& center) noexcept
	{
		std::vector<PixelBlock> players = FindObject(230, 250, 250, 30, min, max);
		RECT closest;
		if (players.size() == 0)
		{
			closest = { 0, 0, 0, 0 };
			return (closest);
		}

		// In case we seam to find many player, we take the closest to the center
		int minDist = -1;
		Vector2 centerVec = Vector2(center.x, center.y);
		for (const auto& player : players)
		{
			const Vector2& pos = player.GetUpperLeft();
			int dist = Utils::Distance(pos, centerVec);
			if (minDist == -1 || dist < minDist)
			{
				const Vector2& size = player.GetSize();
				closest = { pos._x, pos._y, pos._x + size._x, pos._y + size._y };
				minDist = dist;
			}
		}
		return (closest);
	}

	// We pack pixels of similar color together
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