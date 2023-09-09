#include "../../include/header.hpp"

ServerConfig::ServerConfig()
{
    initErrPages();
}

ServerConfig::~ServerConfig()
{
}

void ServerConfig::initErrPages()
{
    // Access the first server's errorPage map
    if (!serverConfigs.empty() && !serverConfigs[0].errorPage.empty())
    {
        // Use a reference to the errorPage map for convenience
        std::map<short, std::string> &errorPage = serverConfigs[0].errorPage;

        errorPage[301] = "";
        errorPage[302] = "";
        errorPage[400] = "";
        errorPage[401] = "";
        errorPage[402] = "";
        errorPage[403] = "";
        errorPage[404] = "";
        errorPage[405] = "";
        errorPage[406] = "";
        errorPage[500] = "";
        errorPage[501] = "";
        errorPage[502] = "";
        errorPage[503] = "";
        errorPage[505] = "";
    }
}



// void ServerConfig::parseServerConfigLine(std::string& line)
// {
//     std::vector<std::string> tokens;
//     std::stringstream ss(line);
//     std::string token;

//     while (std::getline(ss, token, ' '))
//     {
//         if (!token.empty())
//             tokens.push_back(token);
//     }

//     if (tokens[0] == "host:")
//         serverConfigs.back().host = tokens[1];
//     else if (tokens[0] == "port:")
//         serverConfigs.back().port = tokens[1];
//     else if (tokens[0] == "max_body_size:")
//         serverConfigs.back().maxBodySize = tokens[1];
//     else if (tokens[0] == "server_name:"){
//         for (size_t i = 1; i < tokens.size(); i++)
//             serverConfigs.back().serverName.push_back(tokens[i]);
//     }
//     else if (tokens[0] == "error_page"){
//         for (size_t i = 1; i < tokens.size(); i++)
//         {
//             if (tokens[i] == "400:")
//                 serverConfigs.back().errorPage400[tokens[i]] = tokens[i + 1];
//             else if (tokens[i] == "404:")
//                 serverConfigs.back().errorPage404[tokens[i]] = tokens[i + 1];
//         }
//     }
//     else if (tokens.size() == 3 && tokens[0] == "server_name:"){
//         for (size_t i = 1; i < tokens.size(); i++)
//             serverConfigs.back().serverName.push_back(tokens[i]);
//     }
//     else
//         return ;
// }

void ServerConfig::parseServerConfigLine(std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    if (tokens[0] == "host:")
    {
        serverConfigs.push_back(Server());
        serverConfigs.back().host = tokens[1];
    }
    else if (tokens[0] == "port:")
        serverConfigs.back().port = tokens[1];
    else if (tokens[0] == "max_body_size:")
        serverConfigs.back().maxBodySize = tokens[1];
    else if (tokens[0] == "server_name:")
    {
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else if (tokens[0] == "error_page")
    {
        for (size_t i = 1; i < tokens.size() - 1; i += 2)
        {
            short errorCode;
            std::stringstream(tokens[i]) >> errorCode;
            serverConfigs.back().errorPage[errorCode] = tokens[i + 1];
        }
    }
    else if (tokens.size() == 3 && tokens[0] == "server_name:")
    {
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else
        return;
}


void ServerConfig::parseLocationConfigLine(std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' '))
        if (!token.empty())
            tokens.push_back(token);

    if (tokens.empty() || tokens[0] == "}")
        return;
    else if (tokens[0] == "root:")
        serverConfigs.back().locations.back().root = tokens[1];
    else if (tokens[0] == "index:")
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().locations.back().index.push_back(tokens[i]);
    else if (tokens[0] == "autoindex:")
        serverConfigs.back().locations.back().autoindex = tokens[1];
    else if (tokens.size() == 3 && tokens[0] == "cgi_path:")
        serverConfigs.back().locations.back().cgiPath[tokens[1]] = tokens[2];
    else if (tokens[0] == "methods:")
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().locations.back().methods.push_back(tokens[i]);
    else if (tokens[0] == "return:")
        serverConfigs.back().locations.back().return_ = tokens[1];
    else if (tokens[0] == "upload_path:")
        serverConfigs.back().locations.back().uploadPath = tokens[1];
    else
        return ;
}

ServerConfig::ServerConfig(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (file.is_open())
    {
        std::string line;
        int         i;

        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue;
            if (line.find("server {") != std::string::npos){
                i = 1;
                continue;
            }
            if (line.find("location") != std::string::npos){
                serverConfigs.back().locations.push_back(LocationConfig());
                std::size_t start = line.find("location") + 9;
                std::size_t end = line.find("{") - 1;
                std::string locationName = line.substr(start, end - start);
                serverConfigs.back().locations.back().location_Config.push_back(locationName);
                i = 2;
                continue;
            }
            if (i == 1)
                parseServerConfigLine(line);
            else if (i == 2)
                parseLocationConfigLine(line);
            else
                std::cout << "Error: " << line << std::endl;
        }

        if (serverConfigs.empty())
        {
            std::cout << "No server found" << std::endl;
            exit(1);
        }
        file.close();
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
        exit(1);
    }

}

