#include <iostream>
#include "Window.h"
#include "../Header/twitch.h"

class TwitchWindow : Window
{

private:
std::string title = "Twitch";
bool isOpen;

Window* window;

public:
void Open(std::string title) override;
void Update();
void Close();
};