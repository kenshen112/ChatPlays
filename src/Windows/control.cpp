#include "Windows/control.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>


VOID CALLBACK notification(
	PVIGEM_CLIENT Client,
	PVIGEM_TARGET Target,
	UCHAR LargeMotor,
	UCHAR SmallMotor,
	UCHAR LedNumber,
	LPVOID UserData
)
{
	static int count = 1;

	// Send Data to Twitch Chat. Rumble Goes BRRRRRRR
	/*std::cout.width(3);
	std::cout << count++ << " ";
	std::cout.width(3);
	std::cout << (int)LargeMotor << " ";
	std::cout.width(3);
	std::cout << (int)SmallMotor << std::endl;*/
}

// Thread needs to switch to this function. Whoops!

void Emit::ControllerThread(Emit settings, bool manualControl)
{
	std::string keyCode;
	if (!isActive)
	{
		// Probably not the best spot for this
		CreateController(true);
	}
	// Recieve commands from chat and press into emit
	if (manualControl)
	{
		while (isActive)
		{
			// I need a better way to poll this.
			std::cout << "Enter a keycode: ";
			std::cin >> keyCode;

			if (keyCode == "Exit")
			{
				isActive = false;
			}
			emit(cmd);
			report = new XUSB_REPORT();
			Sleep(500);
			resetABS();
			releaseBtn(cmd);
			vigem_target_x360_update(driver, xbox, *report);

		}
	}
}

int Emit::CreateController(bool manualControl)
{
	driver = vigem_alloc();
	if (driver == nullptr)
	{
		std::cerr << "Oops! Driver no allocate! Unga Bunga. Me confused!" << std::endl;
		isActive = false;
		return -1;
	}
	else
	{
		// We have driver. Now establish bus connection.
		const auto bus = vigem_connect(driver);
		if (!VIGEM_SUCCESS(bus))
		{
			std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << bus << std::endl;
			isActive = false;
			return -1;
		}

		xbox = vigem_target_x360_alloc();
		const auto plugEv = vigem_target_add(driver, xbox);
		if (!VIGEM_SUCCESS(plugEv))
		{
			std::cerr << "Target plugin failed with error code: 0x" << std::hex << plugEv << std::endl;
			isActive = false;
			return -1;
		}
		vigem_target_x360_register_notification(driver, xbox, notification, nullptr);
		isActive = true;
	}


	// Recieve commands from chat and press into emit
	if (manualControl)
	{
		std::string keyCode;
		while (isActive)
		{
			// I need a better way to poll this.
			std::cout << "Enter a keycode: ";
			std::cin >> keyCode;

			if (keyCode == "Exit")
			{
				isActive = false;
			}
			emit(cmd);
			report = new XUSB_REPORT();
			Sleep(500);
			resetABS();
			releaseBtn(cmd);
			vigem_target_x360_update(driver, xbox, *report);
		}
	}
	else
	{
		return 0;
	}
}

void Emit::emit(Buttons cmd)
{
	report = new XUSB_REPORT();
	axisData axis;

	switch (cmd)
	{
	case Buttons::UP:
		// Set to max values of Xinput
		axis.set(0, 32767, 0, 0, 0, 0);
		break;
	case Buttons::DOWN:
		axis.set(0, -32768, 0, 0, 0, 0);
		break;
	case Buttons::RIGHT:
		axis.set(32767, 0, 0, 0, 0, 0);
		break;
	case Buttons::LEFT:
		axis.set(-32768, 0, 0, 0, 0, 0);
		break;
	case Buttons::R2:
		axis.set(0, 0, 0, 0, 30, 0);
		break;
	case Buttons::L2:
		axis.set(0, 0, 0, 0, 0, 30);
		break;
	}
	if (cmd > Buttons::R2)
	{
		pressBtn(cmd);
	}
	else
	{
		moveABS(axis);
	}
	vigem_target_x360_update(driver, xbox, *report);
	cmd = Buttons::CLEAR;
}

void Emit::pressBtn(Buttons& btn)
{
	if (report == nullptr)
	{
		report = new XUSB_REPORT();
	}
	report->wButtons |= buttonPos[btn];
}

void Emit::releaseBtn(Buttons& btn)
{
	report->wButtons = 0; //&= ~(1 << buttonPos[btn]);
}

void Emit::moveABS(axisData& axis)
{
	report->sThumbLX = axis.get(0);
	report->sThumbLY = axis.get(1);
	report->sThumbRX = axis.get(2);
	report->sThumbRY = axis.get(3);
	report->bRightTrigger = axis.get(4);
	report->bLeftTrigger = axis.get(5);
}

void Emit::resetABS()
{
	report->sThumbLX = 0;
	report->sThumbLY = 0;
	report->sThumbRX = 0;
	report->sThumbRY = 0;
	report->bRightTrigger = 0;
	report->bLeftTrigger = 0;
}