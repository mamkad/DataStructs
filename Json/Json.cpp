#include "Json.hpp"

#include <stack>
#include <algorithm>
#include <functional>

using std::cout;
using std::for_each;
using std::function;

namespace DataStructs
{
	Json::Json()
	{

	}

	Json::Json(string const& fileName)
	{

	}

	Json::~Json()
	{

	}
	//------------------------------------------------------------------------------------------------------------------------------------
	JsonNode& Json::get(string const& key)
	{

	}

	JsonNode& Json::set(string const& key)
	{

	}
	//------------------------------------------------------------------------------------------------------------------------------------
	void Json::parseFromFile(string const& fileName)
	{

	}

	void Json::parseFromString(string const& jsonString)
	{
	}
}

size_t extractValue(string const& str, string& s, char delim, size_t pos, function<bool(char)> const& ifFunc, function<void()> const& elseAction)
{
	size_t j = pos;

	for(; j < str.size() && str[j] != delim; ++j)
	{
		if (ifFunc(str[j]))
		{
			s.push_back(str[j]);
		}
		else 
		{
			elseAction();
		}
	}
	return j;
}


size_t parse(shared_ptr<DataStructs::JsonNode>& node, string const& jsonString, size_t pos)
{	
	bool inObj = false, isDef = false;
	string key, value;

	for(size_t i = pos; i < jsonString.size(); ++i)
	{
		switch(jsonString[i])
		{
		case '{':
			if (inObj)
			{
				if (isDef)
				{
					auto t = node->getNode(key);
					i = parse(t, jsonString, i) + 1;

					key.clear();
					isDef = false;
				}
				else 
				{
					throw "ошибка1";
				}
			}
			else 
			{
				inObj = true;
			}
			break;
		case '}':
			if (inObj)
			{
				if (value.empty() && key.empty())
				{
					if (isDef)
					{
						throw "ошибка2";
					}
					else 
					{
						return i;
					}	
				}
				else 
				{
					throw "ошибка3";
				}
			}
			else 
			{
				throw "ошибка4";
			}
			break;
		case '"':
			if (inObj)
			{
				if (key.empty())
				{
					if (isDef)
					{
						throw "ошибка5";
					}
					else 
					{
						i = extractValue(jsonString, key, '"', i + 1, [](char ch) { return (isdigit(ch) || isalpha(ch)); }, [](){ throw "ошибка"; });
					}
				}
				else 
				{
					if (isDef)
					{
						i = extractValue(jsonString, value, '"', i + 1, [](char ch) { return (isdigit(ch) || isalpha(ch)); }, [](){ throw "ошибка"; });
						isDef = false;
					}
					else 
					{
						throw "ошибка7";
					}
				}
			}
			else 
			{
				throw "ошибка8";
			}
			break;
		case ':':
			if (inObj)
			{
				if (key.empty())
				{
					throw "ошибка9";
				}
				else 
				{
					isDef = true;
				}
			}
			else 
			{
				throw "ошибка10";
			}
			break;
		case ',':
			if (inObj)
			{
				if ( (!value.empty()) && (!key.empty()) )
				{
					auto t = node->getNode(key);
					t->setValue(value);

					key.clear();
					value.clear();
				}
				else 
				{
					throw "ошибка11";
				}
			}	
			else 
			{
				throw "ошибка12";
			}
			break;
		case '[':
			if (inObj)
			{
				if (key.empty())
				{
					throw "ошибка14";
				}
				else 
				{
					if (isDef)
					{
						i = extractValue(jsonString, value, ']', i + 1, [](char ch) { return ch != ',' && ch != '"'; }, [&value]() { value.push_back(' '); });
						isDef = false;
					}
					else 
					{
						throw "ошибка16";
					}
				}
			}
			else 
			{
				throw "ошибка13";
			}
			break;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			if (inObj)
			{
				if (key.empty())
				{
					throw "ошибка14";
				}
				else 
				{
					if (isDef)
					{
						i = extractValue(jsonString, value, ',', i, [](char ch) { return (isdigit(ch)); }, [](){ throw "ошибка"; }) - 1;
						isDef = false;
					}
					else 
					{
						throw "ошибка16";
					}
				}
			}
			else 
			{
				throw "ошибка17";
			}
			break;	
		default:
			throw "ошибка18";
			break;
		}
	}	
}

void print(shared_ptr<DataStructs::JsonNode> const& node)
{
	for (auto it = node->JsonNodes.begin(); it != node->JsonNodes.end(); ++it)
	{
		if (it->second->value.empty())
		{
			cout << it->first << "\n\t";
			print(it->second);
		}
		else 
		{
			cout << it->first << ' ' << it->second->value <<'\n';
		}
	}
}

void parseFromString(string const& jsonString)
{
	string newjsonString;
	for_each(jsonString.begin(), jsonString.end(), [&newjsonString](char c) { if( (c != ' ') && (c != '\t') && (c != '\n') && (c != '\r') ) { newjsonString.push_back(c); } });
	newjsonString.shrink_to_fit();

	shared_ptr<DataStructs::JsonNode> node = make_shared<DataStructs::JsonNode>();

	parse(node, newjsonString, 0);

	print(node);
}

int main()
{
	//shared_ptr<DataStructs::JsonNode> node = make_shared<DataStructs::JsonNode>();
	string h = "{\"name\":\"Anton\",\"lastname\":\"Rudenok\",\"age\":34,\"address\":{\"city\":\"SpB\",\"streets\":{\"lenin\":34,\"pushkin\":456,},\"area\":3423423,},\"postalCode\":101101,}";
	/*
	string h = "{\
   					\"firstName\": \"Iavn\",\
   					\"lastName\": \"Iavnov\",\
   					\"address\": {\
				       \"streetAddress\": \"Moscow\",\
				       \"city\": \"Leningrad\",\
				       \"postalCode\": 101101,\
					   },\
					   \"phoneNumbers\": [\
					       \"812 123-1234\",\
					       \"916 123-4567\"\
					   ],\
						}";*/
	try 
	{
		parseFromString(h);
	}
	catch(char const* s)
	{
		cout << s << '\n';
	}
}