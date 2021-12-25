#include "PineClient.h"

void PineClient::StartPineThread()
{

}

// ToDo. Write IPC implementation
// 00205028 - FinderInSet
void PineClient::OpenViewFinder()
{
	emulator->Write(0x00205028, 1);
}

void PineClient::CloseViewFinder()
{

}

// Parse commands from thread in here and execute functions
void PineClient::Run()
{

}