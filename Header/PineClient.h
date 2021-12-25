#pragma once
#include <iostream>

#include "settings.h"
#include "pine.h"
#include "twitch.h"

#ifdef __linux__ 
#include "Linux/control.h"
#elif _WIN32
#include "Windows/control.h"
#endif

using namespace PINE;

// ToDo: Write your own implementation of PINE for whatever game or emulator software you're using
class PineClient
{

private:
	std::map<const char*, void*> commands {
		std::make_pair("ViewfinderOpen", &OpenViewFinder),
		std::make_pair("ViewfinderClose", &CloseViewFinder)
	};

	Shared *emulator;

public:
	void StartPineThread();
	void CloseViewFinder();
	void OpenViewFinder();
	void Run();
};

