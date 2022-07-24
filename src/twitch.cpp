#include "twitch.h"
#include <errno.h>

void Twitch::StartTwitchThread(Message* q, TwitchInfo* s)
{
    Twitch t;
    if (t.login(q, s))
    {
        t.update();
    }
}

bool Twitch::login(Message* q, TwitchInfo* s)
{
    settings = *s;
    if (connection.open(address.c_str(), "6667"))
    {
        queue = q;
        std::string buf1 = ("PASS " + settings.oauthToken + "\r\n");
        if (connection.sendBytes(buf1.data(), buf1.size()) <= 0)
        {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            isJoined = false;
        }

        std::string buf2 = ("NICK " + settings.userName + "\r\n");
        if (connection.sendBytes(buf2.c_str(), buf2.size()) <= 0)
        {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            isJoined = false;
        }

        if (!isJoined)
        {
            std::string buf3 = ("JOIN #" + settings.channelName + "\r\n");
            if (connection.sendBytes(buf3.c_str(), buf3.size()) < 0)
            {
                std::cerr << "Send failed: " << strerror(errno) << std::endl;
                isJoined = false;
            }

            connection.recieve();

            if (std::string(connection.recieve()).find(".tmi.twitch.tv JOIN #" + settings.channelName) != std::string::npos)
            {
                std::string buf4 = "CAP REQ :twitch.tv/membership\r\n";
                if (connection.sendBytes(buf4.c_str(), buf4.size()) < 0)
                {
                    std::cerr << "Send failed: " << strerror(errno) << std::endl;
                    isJoined = false;
                }
                connection.recieve();

                std::string buf5 = "CAP REQ :twitch.tv/commands\r\n";
                if (connection.sendBytes(buf5.c_str(), buf5.size()) < 0)
                {
                    std::cerr << "Send failed: " << strerror(errno) << std::endl;
                    isJoined = false;
                }
                connection.recieve();
                std::cout << "Channel Joined" << std::endl;
                isJoined = true;
            }
            else
            {
                isJoined = false;
            }
        }
    }
    return isJoined;
}

// Step two. Parse command and send through thread to controller or IPC alike.
bool Twitch::update()
{
    while (connection.isConnected())
    {
        buffer += connection.recieve();

        std::flush(std::cout);
    }
    return true;
}

// If it's a command process it! Don't push it to global thread queue needlessly! Enqueue in the recognized twitch queue of commands and push to controller.

bool Twitch::ParseCommand(std::string command)
{
    std::string com = connection.ParseCommand(buffer);

    if (buffer.find("PING :tmi.twitch.tv") != std::string::npos)
    {
        std::cout << "Pong: " << pong.c_str() << std::endl;
        if (!connection.sendBytes(pong.c_str(), pong.size()))
        {
            std::cout << "Send Error" << std::endl;
            return false;
        }
    }

    com.clear();
    buffer.clear();
}
