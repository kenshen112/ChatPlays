#include <poll.h>
#include <chrono>
#include <functional>
#include <condition_variable>

#include "Linux/EvDev.h"

Emit::Emit(json j)
{
    from_json(j, *this);
}

void Emit::save(json &j, bool isDefault)
{
    if (isDefault)
    {
        Emit e = Emit();
        j.push_back(e.control);
    }
    else
    {
        control = *this;
        j += control;
    }
}

bool EvDevDevice::pollEvent(uint32_t evType, uint32_t keycode)
{
    int val = 0;

    if (libevdev_fetch_event_value(dev, evType, keycode, &val))
    {
        if (val == 1)
        {
            std::cout << "Event 1" << std::endl;

            if (libevdev_fetch_event_value(dev, evType, keycode, &val))
            {
                std::cout << " Code: " << libevdev_event_code_get_name(evType, keycode) << std::endl;
                std::cout << "Value: " << val << std::endl;
                if (val == 0)
                {
                    std::cout << "Event 2" << std::endl;
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

void Emit::listControllers()
{
    int i = 0;
    int file = 0;

    libevdev* dev;

    // List devices and select one to save.
    for (auto const &entry: fs::directory_iterator("/dev/input"))
    {
        std::string temp = entry.path().filename();

        if (temp.compare(0, 5, "event") == 0)
        {
            // fd doesn't carry through and get's lost. Read is literally polling blank yet somewhat initalized data
            //std::cout << "Temp: " << temp << std::endl;
            file = open(entry.path().c_str(), O_RDONLY);
            int err = libevdev_new_from_fd(file, &dev);
            if (err < 0)
            {
                // If it's just a bad file descriptor, don't bother logging, but otherwise, log it.
                if (err != -9)
                {
                    printf("Failed to connect to device at %s, the error was: %s", entry.path(), strerror(-err));
                    libevdev_free(dev);
                    close(fd);
                }
                else
                {
                    printf("Invalid FD \n");
                    close(fd);
                    return;
                }
            }

            if (libevdev_has_event_type(dev, EV_KEY) && libevdev_has_event_type(dev, EV_ABS))
            {
                fd = file;
                // Since it's being closed anyways. Just tell it to fuck off ourselves. We can reopen in the lower config sections
                close(file);
                libevdev_free(dev);
                controlSelect.push_back(entry.path());
            }
        }
        i += 1;
    }
}

void Emit::PrintControllers()
{
    int i = 0;
    libevdev *dev;
    for(auto &path: controlSelect)
    {
        int fdTemp = open(path.string().c_str(), O_RDONLY);
        if (fdTemp >= 0)
        {
            int err = libevdev_new_from_fd(fdTemp, &dev);
            std::cout << i << ": " << libevdev_get_name(dev) << std::endl;
            close(fdTemp);
            libevdev_free(dev);
            i += 1;
        }
    }
}

EvDevDevice* Emit::selectController()
{
    int j = 0;
    EvDevDevice* control = new EvDevDevice();

    PrintControllers();

    if (controlSelect.size() > 0)
    {
        // We check negative to make sure j only gets entered into once
        // And because index 0 would be a valid index
        std::cout << "> ";
        std::cin >> j;

        control->eventPath = controlSelect[j];

        // Start to create the actual controller device in here
        control->fd = open(control->eventPath.c_str(), O_RDONLY);

        if (control->fd < 0)
        {
            std::cerr << "Err: " << control->fd << std::endl;
        }

        int err = libevdev_new_from_fd(control->fd, &control->dev);

        if (err < 0)
        {
            std::cerr << "Err Generating evdev device" << std::endl;
            return nullptr;
        }

        control->driverVersion = libevdev_get_driver_version(control->dev);
        control->controllerName = libevdev_get_name(control->dev);
        //control->uniqueID = libevdev_get_uniq(control->dev);
    }
    return control;
}

Emit* Emit::InitalConfig()
{
    listControllers();
    controller = selectController();

    bool isMapped = false;
    int button = 0;
    input_event* ev = new input_event();

    // Think about this for a second. There's a constant flow of data.
    // What do you actually NEED from that constant flow to be able to play back the controller when serialized.

    for (int i = 0; i < buttonName.size(); i++)
    {
        std::cout << " Configure: " << buttonName[(Buttons)i] << std::endl;
        isMapped = false;
        while (!isMapped)
        {
            int ret = libevdev_next_event(controller->dev, LIBEVDEV_READ_FLAG_NORMAL, ev);

            if (ret == LIBEVDEV_READ_STATUS_SYNC)
            {
                ret = libevdev_next_event(controller->dev, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_SYNC, ev);
            }

            for (int j = 0; j < button_list.size(); j++)
            {
                isMapped = controller->pollEvent(EV_KEY, button_list[j]);
                if (!isMapped)
                    continue;

                else if (isMapped == true)
                {
                    std::cout << " Code: " << libevdev_event_code_get_name(EV_KEY, button_list[j]) << std::endl;
                    controller->mappedControls.emplace((Buttons)button, button_list[j]);
                    break;
                }
            }
        }
    }
    return this;
}

void Emit::CreateController(bool manual)
{
    if (controller->AttachController())
    {
        if (manual)
        {
            // run manual control loop.
        }
    }
}

bool EvDevDevice::AttachController()
{
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        printf ("Open Error! \n");
        isActive = false;
    }
    else
    {
        if (ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
        {
            printf("IoCtrl EVBit Error\n");
        }
        if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        {
            printf("IoCtrl EVBit Error\n");
        }
        
        for (int i = 0; i < buttonCodes.size(); i++)
        {
            if (ioctl(fd, UI_SET_KEYBIT, buttonCodes[Buttons(i)]) < 0)
            {
                printf("IoCtrl KEY Error\n");
            }
        }
        if (ioctl(fd, UI_DEV_CREATE) < 0)
        {
            std::cout << "ERR Bad FD: " << fd << " : " << strerror(errno) << std::endl;
            isActive = false;
        }
        sleep(1);
        isActive = true;
        // Count of attached players
        //controllerID = ;
    }
    return isActive;
}

void EvDevDevice::DisconnectController()
{
    libevdev_uinput_destroy(uidev);
    libevdev_free(dev);
    close(fd);
}

int EvDevDevice::PressButton(Buttons button)
{
    int emitCode = 0;

    emitCode = libevdev_uinput_write_event(uidev, EV_KEY, buttonCodes[button], 1);
    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    sleep(1);
    ReleaseButton(button);
}

int EvDevDevice::ReleaseButton(Buttons button)
{
    int emitCode = 0;

    emitCode = libevdev_uinput_write_event(uidev, EV_KEY, button, 0);
    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    return emitCode;
}

void EvDevDevice::MoveABS(ABS abs, float moveAxis)
{
    int emitCode = 0;

    emitCode = libevdev_uinput_write_event(uidev, EV_ABS, abs, moveAxis);
    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

    sleep(1);
    ResetABS(abs);
}

void EvDevDevice::ResetABS(ABS abs)
{
    int emitCode = 0;
    emitCode = libevdev_uinput_write_event(uidev, EV_ABS, abs, 0);
    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
}

bool Emit::emit(Buttons keyCode)
{
    int emitCode = 0;
    return emitCode;
}

bool Emit::Close()
{
    controller->DisconnectController();
    return 0;
}

void to_json(nlohmann::json& j, const input_absinfo& abs)
{
    j["flat"] = abs.flat;
    j["fuzz"] = abs.fuzz;
    j["value"] = abs.value;
    j["minimum"] = abs.minimum;
    j["maximum"] = abs.maximum;
    j["resolution"] = abs.resolution;
}

void from_json(const nlohmann::json& j, input_absinfo& abs)
{
    j["flat"].get_to(abs.flat);
    j["fuzz"].get_to(abs.fuzz);
    j["value"].get_to(abs.value);
    j["minimum"].get_to(abs.minimum);
    j["maximum"].get_to(abs.maximum);
    j["resolution"].get_to(abs.resolution);
}

void to_json(nlohmann::json& j, const EvDevDevice& c)
{
    j[1]["abs"] = c.abs;
    j[1]["buttonCodes"] = c.buttonCodes;
    j[1]["controllerName"] = c.controllerName;

}

void from_json(const nlohmann::json& j, EvDevDevice& c)
{
    j[1]["abs"].get_to(c.abs);
    j[1]["buttonCodes"].get_to(c.buttonCodes);
}

void to_json(nlohmann::json& j, const Emit& p)
{
    //j[1]["controller"] = p.controller;
}

void from_json(const nlohmann::json& j, Emit& p)
{
    //j[1]["controller"].get_to(p.controller);
}