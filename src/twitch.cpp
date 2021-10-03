#include "twitch.h"
#include <errno.h>

TwitchInfo::TwitchInfo()
{
    twitch = nlohmann::json
    {
        {"userName", "USERNAME HERE"},
        {"oauthToken", "OAUTH HERE"},
        {"channelName", "CHANNEL_NAME HERE"}
    };
}


TwitchInfo::TwitchInfo(json& j)
{
    from_json(j, *this);
}

void TwitchInfo::Save(json& j, bool isDefault)
{
    if (isDefault)
    {
        TwitchInfo t = TwitchInfo();
        j += t.twitch;
    }
    else
    {
        twitch = *this;
        j += twitch;

    }
}

void TwitchInfo::Load(nlohmann::json& j)
{
    from_json(j, *this);
}

void to_json(json& j, const TwitchInfo& p)
{
    j = nlohmann::json
    {
        {"userName", p.userName},
        {"oauthToken", p.oauthToken},
        {"channelName", p.channelName}
    };
}

void from_json(const nlohmann::json& j, TwitchInfo& p)
{
    j[0]["userName"].get_to(p.userName);
    j[0]["oauthToken"].get_to(p.oauthToken);
    j[0]["channelName"].get_to(p.channelName);
}

void Twitch::create(Message* q, TwitchInfo* s)
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
            return false;
        }

        std::string buf2 = ("NICK " + settings.userName + "\r\n");
        if (connection.sendBytes(buf2.c_str(), buf2.size()) <= 0)
        {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return false;
        }

        std::cout << "Buffer: " << connection.recieve() << std::endl;

        if (!isJoined)
        {
            std::string buf4 = ("JOIN #" + settings.channelName + "\r\n");
            if (connection.sendBytes(buf4.c_str(), buf4.size()) < 0)
            {
                std::cerr << "Send failed: " << strerror(errno) << std::endl;
                isJoined = false;
            }

            std::string buf5 = "CAP REQ :twitch.tv/membership";
            if (connection.sendBytes(buf5.c_str(), buf5.size()) < 0)
            {
                std::cerr << "Send failed: " << strerror(errno) << std::endl;
                isJoined = false;
            }
            isJoined = true;
        }
        return isJoined;
    }
}

// Step two. Parse command and send through thread to controller or IPC alike.
bool Twitch::update()
{
    while (connection.isConnected())
    {
        if ((buffer += connection.recieve()).empty())
        {
            return false;
        }

        std::cout << "Update" << std::endl;
        std::cout << buffer << std::endl;

        std::string com = connection.parseCommand(buffer);

        if (buffer.find("PING :tmi.twitch.tv") != std::string::npos)
        {
            std::cout << "Pong: " << pong.c_str() << std::endl;
            if (!connection.sendBytes(pong.c_str(), pong.size()))
            {
                std::cout << "Send Error" << std::endl;
                return false;
            }
        }
        else if (buffer.find("PRIVMSG ") != std::string::npos)
        {
            queue->enque(com);
        }

        com.clear();
        buffer.clear();
        std::flush(std::cout);
    }
    return true;
}
