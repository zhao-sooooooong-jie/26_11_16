#pragma once

#include<iostream>
#include<string>
#include<unordered_map>
#include"logger.h"

std::string default_dict_path = "Dict.txt";
std::string sep = ": ";

class Dict{
public:
    Dict(std::string dict_path = default_dict_path)
    :_dict_path(dict_path)
    {
        LoadDict();
    }
    ~Dict(){}

    void LoadDict()
    {
        std::ifstream in(_dict_path);
        if(!in.is_open())
        {
            LOG_FATAL("open " << _dict_path << " error");
        }
        std::string line;
        while(std::getline(in, line))
        {
            LOG_DEBUG("load " << _dict_path << " success");
            auto pos = line.find(sep);
            if(pos == std::string::npos)
            {
                LOG_FATAL("Format: " << line << " error");
                continue;
            }

            std::string k = line.substr(0, pos);
            std::string v = line.substr(pos + sep.size());
            _dict.insert(std::make_pair(k, v));
        }

        in.close();
        LOG_INFO("load down");
    }

    std::string Translate(std::string word)
    {
        auto iter = _dict.find(word);
        if(iter != _dict.end())
        {
            return _dict[word];
        }
        return "not find";
    }

private:
    std::string _dict_path;
    std::unordered_map<std::string, std::string> _dict;

};