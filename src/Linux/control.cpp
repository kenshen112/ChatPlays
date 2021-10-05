#include <poll.h>
#include <chrono>
#include <functional>
#include <condition_variable>

#include "Linux/control.h"

Controller::Controller()
{
    fd = 0;
    dev = nullptr;
    driverVersion = 0;
    uniqueID = std::string();
    controllerName = std::string();
}

Controller::Controller(const Controller& c)
{
    fd = c.fd;
    dev = c.dev;
    abs = c.abs;
    uniqueID = c.uniqueID;
    eventPath = c.eventPath;
    driverVersion = c.driverVersion;
    mappedControls = c.mappedControls;
    controllerName = c.controllerName;
}

Emit::Emit(json j)
{
    //from_json(j, *this);
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
        j.push_back(control);
    }
}

input_event Controller::pollEvent()
{
    int err = 0;
    input_event event;
    err = read(fd, &event, sizeof(event));

    //err = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_SYNC, &ev);
    if (err < 0)
    {
        if (err != EINVAL)
        {
            std::cout << " Failed read: " << err << "FD: " << fd << " "<< strerror(errno)  << std::endl;
            return input_event();
        }
    }
    //printf("ev: 0x%02x", ev.code + ' \n');
    return event;
}

void Emit::listControllers()
{
    // List devices and select one to save.
    for (auto &entry: fs::directory_iterator("/dev/input"))
    {
        std::string temp = entry.path().filename();

        int i = 0;
        int file = 0;

        libevdev* dev;

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
                std::cout << i << " Dev: " << libevdev_get_name(dev) << std::endl;
                // Since it's being closed anyways. Just tell it to fuck off ourselves. We can reopen in the lower config sections
                close(file);
                libevdev_free(dev);
                controlSelect.push_back(entry.path());
            }
        }
        i += 1;
    }
}


Controller Emit::selectController()
{
    listControllers();
    fds = new pollfd();
    Controller control;
    int j = 0;

    if (controlSelect.size() > 0)
    {
        // We check negative to make sure j only gets entered into once
        // And because index 0 would be a valid index

        std::cout << "> ";
        std::cin >> j;

        // Start to create the actual controller device in here
        control.fd = open(controlSelect[j].c_str(), O_RDONLY);
        libevdev_new_from_fd(control.fd, &control.dev);

        fds[0].fd = control.fd;

        control.driverVersion = libevdev_get_driver_version(control.dev);
        control.controllerName = libevdev_get_name(control.dev);
        control.uniqueID = libevdev_get_uniq(control.dev);
    }
    return control;
}

void Emit::initalConfig()
{
    controller = selectController();
    int i = 0;
    // This is where the car crash happens kiddos
    while (i < controlNames.size())
    {
        std::cout << "Configure: " << controlNames[(Buttons)i];

        controller.ev = controller.pollEvent();


        if ((Buttons)i < Buttons::A)
        {
            // Assume it's an ABS
            if(controller.ev.type != EV_ABS)
            {
                continue;
            }
            else
            {
                input_absinfo absTemp;
                ioctl(controller.fd, EVIOCGABS(controller.ev.code), absTemp);
                std::cout << " Code: " << libevdev_event_code_get_name(controller.ev.type, controller.ev.code) << std::endl;

                controller.abs.try_emplace(controller.ev.code, &absTemp);
                controller.mappedControls.emplace((Buttons)i, controller.ev);
                i += 1;
            }
        }
        if ((Buttons)i >= Buttons::A)
        {
            // Assume it's ev key
            if (controller.ev.type != EV_KEY)
            {
                continue;
            }
            else
            {
                // 0x100 would be a button that I care about
                if (controller.ev.value >= BTN_0)
                {
                    controller.mappedControls.emplace((Buttons)i, controller.ev);
                    i+= 1;
                }
            }
        }
    }
}

bool Emit::CreateController(Message* q, bool manualControl)
{
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        printf ("Open Error! \n");
        return false;
    }
    else
    {
        queue = q;

        if (ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
        {
            printf("IoCtrl EVBit Error\n");
        }
        if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        {
            printf("IoCtrl EVBit Error\n");
        }
        //int type = controller.buttonCodes[0];
        for (int i = 0; i < controller.buttonCodes.size(); i++)
        {
            if (i < DRIGHT)
            {
                if (ioctl(fd, UI_ABS_SETUP, &controller.abs[Buttons(i)], 0) < 0)
                {
                    printf("IoCtrl ABS Error\n");
                }
            }
            else
            {
                if (ioctl(fd, UI_SET_KEYBIT, controller.buttonCodes[Buttons(i)]) < 0)
                {
                    printf("IoCtrl KEY Error\n");
                }
            }
        }
        if (ioctl(fd, UI_DEV_CREATE) < 0)
        {
            std::cout << "ERR Bad FD: " << fd << " : " << strerror(errno) << std::endl;
            return false;
        }
        sleep(1);
        return true;
    }
}

int Emit::pressBtn(uint32_t button)
{
    int emitCode = 0;

    emitCode = libevdev_uinput_write_event(uidev, EV_KEY, button, 1);
    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    sleep(1);
    releaseBtn(button);

    return emitCode;
}

int Emit::releaseBtn(uint32_t button)
{
    int emitCode = 0;
    emitCode = libevdev_uinput_write_event(uidev, EV_KEY, button, 0);
    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }
    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }
    return emitCode;
}

int Emit::moveABS(uint32_t ABS, int moveAxis, int flat)
{
    int emitCode = 0;
    emitCode = libevdev_uinput_write_event(uidev, EV_ABS, ABS, moveAxis);

    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    sleep(1);
    resetABS(ABS, flat);
    return emitCode;
}

int Emit::resetABS(uint32_t ABS, int flat)
{
    int emitCode = 0;
    emitCode = libevdev_uinput_write_event(uidev, EV_ABS, ABS, flat);

    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    emitCode = libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

    if (emitCode < 0)
    {
        std::cerr << "PRESS ERROR: " << strerror(errno) << std::endl;
        return -1;
    }

    return emitCode;
}

Buttons Emit::GetCommands(std::string key)
{
    if (commands.find(key) != commands.end())
    {
        std::cout << "Command found" << std::endl;
        return commands[key];
    }
    else
    {
        return Buttons(-1);
    }
}

bool Emit::emit(Buttons keyCode)
{
    int emitCode = 0;
    switch(keyCode)
    {
    case Buttons::UP:
        emitCode = moveABS(controller.buttonCodes[keyCode], controller.mappedControls[keyCode].value, 4095);
        break;
    case Buttons::DOWN:
        // Dpad Down
        emitCode = moveABS(controller.buttonCodes[keyCode], controller.mappedControls[keyCode].value, 4095);
        break;

    case Buttons::LEFT:
        // Dpad Left
        emitCode = moveABS(controller.buttonCodes[keyCode], controller.mappedControls[keyCode].value, 4095);
        break;

    case Buttons::RIGHT:
        // Dpad Right
        emitCode = moveABS(controller.buttonCodes[keyCode], controller.mappedControls[keyCode].value, 4095);
        break;

    case Buttons::EXIT: 
        emitCode = Close();
        return emitCode;
        break;
    }
    emitCode = pressBtn(controller.buttonCodes[keyCode]);
    return emitCode;
}

bool Emit::Close()
{
    libevdev_uinput_destroy(uidev);
    libevdev_free(controller.dev);
    return 0;
}