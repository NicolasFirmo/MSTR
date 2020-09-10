#pragma once
#include "Window.h"

class MSTR
{
public:
	static bool Init();
	static void ShutDown();

	static void CreateWindow() { s_Instance.m_CreatingWindow = true; }

public:
	MSTR() = default;
	MSTR(const MSTR&) = delete;
	MSTR& operator=(const MSTR&) = delete;

private:
	static MSTR s_Instance;

	std::vector<Window> m_Windows;
	bool m_Running = false;

	bool m_CreatingWindow = false;
};