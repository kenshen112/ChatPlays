#include "settings.h"

// Load Json
bool Settings::Load()
{
    file.open(filePath);
    file.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    if (file.is_open())
    {
        try
        {
            j = json::parse(file);
        }
        catch (std::exception const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
        file.close();
    }
    return true;
}

// Save Json
bool Settings::Save()
{
    std::string settingsParsed;
    file.open(filePath);
    file.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file << j.dump(4);
        file.close();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
     return true;
}

void Settings::SetUser(std::string& str)
{
    if (!str.empty() && twitch->userName != str)
        twitch->userName = str;
}

void Settings::SetOauth(std::string& oauth)
{
    if (!oauth.empty() && twitch->oauthToken != oauth)
        twitch->oauthToken = oauth;
}

void Settings::SetChannel(std::string& channel)
{
    if (!channel.empty() && twitch->channelName != channel)
        twitch->channelName = channel;
}

void to_json(json& j, const TwitchInfoSettings& p)
{
    j = nlohmann::json
    {
        {"userName", p.userName},
        {"oauthToken", p.oauthToken},
        {"channelName", p.channelName}
    };
}

void from_json(const nlohmann::json& j, TwitchInfoSettings& p)
{
    j[0]["userName"].get_to(p.userName);
    j[0]["oauthToken"].get_to(p.oauthToken);
    j[0]["channelName"].get_to(p.channelName);
}

// Could be printing settings. Or, writing the file
/*std::ostream& operator << (std::ostream& out, const Settings& c)
{

    return *this;
}

std::istream& operator >> (std::istream& in, Settings& c)
{

    return *this;
}*/