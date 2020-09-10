#include "MSTR.h"

int main()
{
	MSTR::Init();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	MSTR::CreateWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MSTR::CreateWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MSTR::CreateWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MSTR::CreateWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	MSTR::CreateWindow();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	std::cin.ignore();

	MSTR::ShutDown();

	std::cin.ignore();
}