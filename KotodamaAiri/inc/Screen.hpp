#ifndef SCREEN_HPP_
# define SCREEN_HPP_

# include <vector>
# include "PixelInfo.hpp"
# include "PixelBlock.hpp"

namespace KotodamaAiri
{
	class Screen
	{
	public:
		Screen() noexcept;
		~Screen() noexcept;
		void CheckPixels() noexcept;

	private:
		[[nodiscard]] RECT FindPlayer(const Vector2 &min, const Vector2 &max) noexcept;
		[[nodiscard]] std::vector<RECT> FindAllies(const Vector2& min, const Vector2& max) noexcept;
		[[nodiscard]] std::vector<PixelBlock> FindObject(int red, int green, int blue, int offset, const Vector2& min, const Vector2& max) noexcept;
		std::vector<PixelInfo> GetPixels(int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax, const Vector2 &min, const Vector2 &max) const noexcept;
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