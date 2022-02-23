#pragma once
#include <map>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <filesystem>

#include <linux/uinput.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "json.hpp"
#include "message.h"

#include "Controller.h"

using json = nlohmann::json;

namespace fs = std::filesystem;

// This is supposed to represent the actual controller in memory as it's created. 
// It can hold it's own assigned controller data and mappings
struct EvDevDevice : Controller
{
    // Consider all of the below a default mapping
    std::map<Buttons, uint32_t> buttonCodes;
    struct std::map<uint32_t, input_absinfo> abs;

    // All of this data is intended to create as convincing a device as possible
    std::string uniqueID;
    std::string controllerName;

    fs::path eventPath = "/dev/input";

    int fd;
    int driverVersion;

    // Controller struct pointers
    libevdev_uinput* uidev;
    input_event ev;
    libevdev *dev;
    timeval timer;

    bool isActive;

    bool AttachController() override;
    int PressButton(Buttons b) override;
    int ReleaseButton(Buttons b) override;
    void MoveABS(ABS abs, float axis) override;
    void ResetABS(ABS abs) override;
    void DisconnectController() override;

    input_event pollEvent(pollfd* fds);
    std::map<Buttons, input_event> mappedControls;

    friend void to_json(nlohmann::json& j, const Controller& p);
    friend void from_json(const nlohmann::json& j, Controller& p);

};

class Emit
{
    private:
    int fd = 0;
    int maxInput = 0;

    bool isActive;
    json control;

    pollfd* fds;

    EvDevDevice* controller;

    std::queue<Buttons>controlQueue;

    std::vector<fs::path> controlSelect;

    public:

    Emit() = default;
    Emit(json j);

    Emit *InitalConfig();
    void listControllers(pollfd* fds);

    EvDevDevice selectController();

    void PrintControllers();
    void CreateController(bool manual);

    void save(json &j, bool isDefault = false);

    friend void to_json(nlohmann::json& j, const Emit& p);
    friend void from_json(const nlohmann::json& j, Emit& p);

    friend void to_json(nlohmann::json& j, const input_absinfo& p);
    friend void from_json(const nlohmann::json& j, input_absinfo& p);


    bool Close();
    bool emit(Buttons cmd);
};