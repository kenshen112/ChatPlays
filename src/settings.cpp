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

// Could be printing settings. Or, writing the file
std::ostream& operator<<(std::ostream &out, const Settings &c)
{

}

std::ifstream& operator>>(std::istream &in, Settings &c)
{

}