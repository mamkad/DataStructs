#pragma once

//#include "../String/String.hpp"
#include "../File/File.hpp"

#include <map>
#include <memory>
#include <string>
#include <iostream>

using std::string;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::cout;

namespace DataStructs
{
    struct JsonNode final
    {
        JsonNode()
        {
            //cout << "JsonNode()\n";
        }

        ~JsonNode()
        {
            //cout << "~JsonNode()\n";
        }

        void setValue(string val)
        {
            value = val;
        }

        shared_ptr<JsonNode> getNode(string key)
        {
            auto resIter = JsonNodes.insert({key, make_shared<JsonNode>()});
            return resIter.first->second;
        }

        string value;
        map<string, shared_ptr<JsonNode>> JsonNodes;
    };

    class Json final
    {
    private:
        JsonNode headNode;
        File jsonFile;

    public:
        Json();
        Json(string const& fileName);
        ~Json();

    public:
        JsonNode& get(string const& key);
        JsonNode& set(string const& key);

    public:
        void parseFromFile(string const& fileName);
        void parseFromString(string const& jsonString); 

    private:
        
    };
}

