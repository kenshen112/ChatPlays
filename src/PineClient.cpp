#include "PineClient.h"

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
	std::map<std::string, function>::iterator it = commands.find(command);
	if (it != commands.end())
	{
		commands[command]();
	}
}