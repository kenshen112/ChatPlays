#include "PineClient.h"
#include "PS2Math.h"

void PineClient::StartPineThread(Message* q)
{
	PineClient c;
	c.init(q);
}

void PineClient::init(Message* q)
{
	emulator = new PCSX2(0);
	active = true;
	while (active)
	{
		Run(q->dequeue());
	}
}

void PineClient::NewGame()
{
	// Note that this will just make the cursor go to the option
	emulator->Write(0x00343077, 0);

	// Press X on new game. This probably won't be the final logic in here.
	controller.emit(nullptr ,Buttons::A, 0);

	// To Do: initalize all bot data structres and zero it all out to start a fresh session.


}

// ToDo. Write IPC implementation. I need to either find the value that sets these off. Or call the functions directly
// 00205028 - FinderInSet
void PineClient::OpenViewFinder()
{
	// This is probably wrong but is a start
	emulator->Write(0x00205028, 1);
}

// 002052e8 - FinderEndSet
void PineClient::CloseViewFinder()
{
	// This is probably wrong but is a start
	emulator->Write(0x002052e8, 1);
}

// Parse commands from thread in here and execute functions
void PineClient::Run(std::string command)
{

	// A global state.
	if (emulator->Read<unsigned int>(0x00252a90) == 0x110)
	{
		// View Finder
	}

	switch (state)
	{
	case GameState::NORMAL:
		if (command == "NewGame")
		{
		}

		// Current hour or mission
		hour = emulator->Read<int>(0x00252a74);

		// Add hour logic
		switch (hour)
		{
		case 0:
			// Mafuyu
			break;

		default:
			// Miku, unless we need specific logic outside of hours aside from Zero. Most of her logic can be handled in here.
			break;
		}

		break;

	case GameState::BATTLE:
		enemyHealth = emulator->Read<int>(00252e98);
		std::cout << enemyHealth << std::endl;
		
		// A stub to read enemies position for pathfinding later.
		emulator->Read<Vector3>(0x00252ebc);
			break;
	}
}