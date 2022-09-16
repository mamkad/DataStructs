#pragma once

#include "../String/String.hpp"
//#include "../File/File.hpp"

#include <map>
#include <memory>
#include <vector>
#include <functional>

using std::map;
using std::shared_ptr;
using std::make_shared;
using std::function;
using std::vector;

namespace DataStructs
{
    // класс json-узла (ключ и указатель на значения для данного ключа
    struct JsonNode final
    {
    private:
        vector<String> value_;                           // значения
        map<String, shared_ptr<JsonNode>> childNodes_;   // дочерние узлы

    public:
        JsonNode();
        JsonNode(JsonNode const&);
        JsonNode(JsonNode&&) noexcept;
        ~JsonNode();

    public:
        JsonNode& operator=(JsonNode const&);
        JsonNode& operator=(JsonNode&&) noexcept;

    public:
        shared_ptr<JsonNode> createChildNode(String const&); // создать дочерний узел для данного узла с ключом 
        void setValue(String const&);                        // задать занчение по данному ключу
        String& getValue();                                  // получить значение
        void clear();                                        // очистить содержимое

    public:
        bool emptyValue()  const noexcept;  // содежит ли узел значение
        bool emptyChilds() const noexcept;  // имеет ли узел дочерние узлы
    };

    class Json final
    {
    public:
        static function<bool(char)> isNotDigAlph;
        static function<bool(char)> isNotDig;

    private:
        shared_ptr<JsonNode> rootNode_ = make_shared<JsonNode>(); // главнй корневой узел 

    public:
        Json();
        Json(String const&);
        Json(Json const&);
        Json(Json&&);
        ~Json();

    public:
        Json& operator=(Json const&);
        Json& operator=(Json&&);

    public:
        JsonNode& getByKey(String const&);   // получить значение по ключу
        JsonNode& operator[](String const&); // получить значение по ключу

    public:
        void parseFromFile(String const&);
        void parseFromString(String&);
        void clear();

    public:
        bool empty() const noexcept;

    private:
        static size_t extract();
        static size_t parse(shared_ptr<JsonNode>&, String&, size_t);
    };
}
