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

typedef void (*function)(void);

using namespace PINE;

enum GameState {NORMAL, BATTLE};

// ToDo: Write your own implementation of PINE for whatever game or emulator software you're using
class PineClient
{

private:
	std::map<std::string, function> commands; /*{
		std::make_pair("ViewfinderOpen", &OpenViewFinder),
		std::make_pair("ViewfinderClose", &CloseViewFinder)
	};*/

	Emit controller;
	Shared *emulator;
	bool active;

	int enemyHealth; // 00252e98
	int night; // To check for possibly active ghosts!
	int missionHour; // To check for hour zero!

	GameState state;

	// cam_type

public:
	PineClient() = default;
	static void StartPineThread(Message* q);
	void init(Message* q);
	void NewGame();
	void CloseViewFinder();
	void OpenViewFinder();
	void Run(std::string command);
};

