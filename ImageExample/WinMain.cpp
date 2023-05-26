#include <Windows.h>

#include "ImageExaple.h"

ImageExaple myFramework;


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	int result;

	try
	{
		myFramework.Initialize(hInstance);

		result = myFramework.GameLoop();
	}
	catch (com_exception& e)
	{
		OutputDebugStringA(e.what());
	}
	catch (std::exception e)
	{

	}

	return result;

}