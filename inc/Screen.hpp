#pragma once

# include <vector>
# include "PixelInfo.hpp"
# include "PixelBlock.hpp"

namespace Airi
{
	class Screen
	{
	public:
		Screen() noexcept;
		~Screen() noexcept;
		void CheckPixels() noexcept;

	private:
		enum Direction
		{
			LEFT,
			RIGHT,
			UNDEFINED
		};
		void RightClick() const noexcept;
		[[nodiscard]] RECT GetClosest(const std::vector<RECT> allRects, int& minDist, const Vector2& playerDistRef) const noexcept;
		[[nodiscard]] RECT FindPlayer(const Vector2 &min, const Vector2 &max, const POINT& center) noexcept;
		[[nodiscard]] std::vector<RECT> FindAllies(const Vector2& min, const Vector2& max) noexcept;
		[[nodiscard]] std::vector<RECT> FindEnnemies(const Vector2& min, const Vector2& max) noexcept;
		[[nodiscard]] std::vector<PixelBlock> FindObject(int red, int green, int blue, int offset, const Vector2& min, const Vector2& max) noexcept;
		[[nodiscard]] std::vector<PixelInfo> GetPixels(int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax, const Vector2 &min, const Vector2 &max) noexcept;
		[[nodiscard]] std::vector<PixelInfo> GetPixels(const Vector2 &min, const Vector2 &max) noexcept;
		[[nodiscard]] std::string GetText(const Vector2 &min, const Vector2 &max) noexcept;
		[[nodiscard]] bool IsClose(int value, int refValue, int margin) const noexcept;
		void UpdatePixels() noexcept;
		HWND _desktopWindow;
		HDC _desktopDc, _captureDc;
		const int _width, _height;
		const int _screenSize;
		HBITMAP _captureBitmap;
		BITMAPINFO _bmi;
		RGBQUAD *_pixels;
		WORD _nbColors;
		WORD _nbPlanes;
	};
}