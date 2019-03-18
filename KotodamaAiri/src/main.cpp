#include <iostream>
#include <Windows.h>

int main()
{
	// Get screen info
	HWND desktopWindow = GetDesktopWindow();
	HDC desktopDc = GetDC(desktopWindow);
	int width = GetDeviceCaps(desktopDc, HORZRES);
	int height = GetDeviceCaps(desktopDc, VERTRES);
	HDC captureDc = CreateCompatibleDC(desktopDc);
	HBITMAP captureBitmap = CreateCompatibleBitmap(desktopDc, width, height);
	SelectObject(captureDc, captureBitmap);
	BitBlt(captureDc, 0, 0, width, height, desktopDc, 0, 0, SRCCOPY | CAPTUREBLT);

	// Set screen infos
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = GetDeviceCaps(desktopDc, PLANES);
	bmi.bmiHeader.biBitCount = GetDeviceCaps(desktopDc, BITSPIXEL);
	bmi.bmiHeader.biCompression = BI_RGB;
	int screenSize = width * height;
	RGBQUAD* pixels = new RGBQUAD[screenSize];

	int redObjMin = 214 - 5;
	int redObjMax = 214 + 5;
	int greenObjMin = 138 - 5;
	int greenObjMax = 138 + 5;
	int blueObjMin = 52 - 5;
	int blueObjMax = 52 + 5;
	// Get pixels
	GetDIBits(captureDc, captureBitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);
	for (int i = 0; i < screenSize; i++)
	{
		const RGBQUAD& quad = pixels[i];
		int red = quad.rgbRed;
		int green = quad.rgbGreen;
		int blue = quad.rgbBlue;
		if (red >= redObjMin && red <= redObjMax && green >= greenObjMin && green <= greenObjMax && blue >= blueObjMin && blue <= blueObjMax)
			std::cout << "(" << (i % width) << ";" << (i / width) << "): " << "(" << red << ";" << green << ";" << blue << ")" << std::endl;
	}

	// Free
	delete[] pixels;
	ReleaseDC(desktopWindow, desktopDc);
	DeleteDC(captureDc);
	DeleteObject(captureBitmap);
	std::cout << "Enter something to continue..." << std::endl;
	getchar();
	return (0);
}