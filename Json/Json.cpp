#include "Json.h"

#include <stack>
#include <algorithm>
#include <functional>
#include <iostream>

using std::cout;
using std::for_each;
using std::function;
using std::move;
using std::exchange;
using std::logic_error;
using std::make_shared;

namespace DataStructs
{
	JsonNode::JsonNode() 
	{
		cout << "JsonNode()\n";
	}

	JsonNode::JsonNode(JsonNode const& otherNode) : value_(otherNode.value_), childNodes_(otherNode.childNodes_)
	{
		cout << "JsonNode(JsonNode const& otherNode)\n";
	}

	JsonNode::JsonNode(JsonNode&& otherNode) noexcept : value_(move(otherNode.value_)), childNodes_(move(otherNode.childNodes_))
	{
		cout << "JsonNode(JsonNode&& otherNode)\n";
	}

	JsonNode::~JsonNode()
	{
		cout << "~JsonNode()\n";
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------
	JsonNode& JsonNode::operator=(JsonNode const& otherNode)
	{
		cout << "JsonNode::operator=(JsonNode const& otherNode)\n";

		if (this != &otherNode)
		{
			clear();
			value_ = otherNode.value_;
			childNodes_ = otherNode.childNodes_;
		}

		return *this;
	}

	JsonNode& JsonNode::operator=(JsonNode&& otherNode) noexcept
	{
		cout << "JsonNode::operator=(JsonNode&& otherNode)\n";
		if (this != &otherNode)
		{
			clear();
			value_ = move(otherNode.value_);
			childNodes_ = move(otherNode.childNodes_);
		}

		return *this;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------
	shared_ptr<JsonNode> JsonNode::createChildNode(String const& key)
	{
		if (!emptyValue())
		{
			throw logic_error("You can not set child nodes for a node that already has a set value");
		}

		auto resIt = childNodes_.insert({key, make_shared<JsonNode>()});
		return resIt.first->second;
	}

	void JsonNode::setValue(String const& value)
	{
		if (!emptyChilds())
		{
			throw logic_error("To set a value for a node it must not contain child nodes");
		}

		value_ = value;
	}

	String& JsonNode::getValue()
	{
		return value_;
	}

	void JsonNode::clear()
	{
		value_.clear();
		childNodes_.clear();
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool JsonNode::emptyValue() const noexcept
	{
		return value_.empty();
	}
	
	bool JsonNode::emptyChilds() const noexcept
	{
		return childNodes_.empty();
	}
	//================================================================================================================================================================
	Json::Json()
	{
		cout << "Json()\n";
	}

	Json::Json(String const& fileName)
	{
		cout << "Json(String const& fileName)\n";
	}

	Json::Json(Json const& otherJson)
	{
		cout << "Json(Json const& otherJson)\n";
	}

	Json::Json(Json&& otherJson)
	{
		cout << "Json(Json&& otherJson)\n";
	}

	Json::~Json()
	{
		cout << "~Json()\n";
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	Json& Json::operator=(Json const& otherJson)
	{
		cout << "Json::operator=(Json const& otherJson)\n";
	}

	Json& Json::operator=(Json&& otherJson)
	{
		cout << "Json::operator=(Json&& otherJson)\n";
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	JsonNode& Json::getByKey(String const& key)
	{

	}
	JsonNode& Json::operator[](String const& key)
	{
		return getByKey(key);
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	void Json::parseFromFile(String const& fileName)
	{

	}

	void Json::parseFromString(String const& jsonString)
	{
		try
		{
			parse(rootNode_, jsonString, 0);
		}
		catch (...)
		{
			clear();
			throw;
		}
	}

	void Json::clear()
	{
		rootNode_->clear();
	}

	bool Json::empty() const noexcept
	{
		return rootNode_->emptyChilds();
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	size_t Json::extractValue(String const& str, String& s, char delim, size_t pos, function<bool(char)> const& ifFunc, function<void()> const& elseAction)
	{
		size_t j = pos;

		for (; j < str.len() && str[j] != delim; ++j)
		{
			if (ifFunc(str[j]))
			{
				//s.push_back(str[j]);
			}
			else
			{
				elseAction();
			}
		}
		return j;
	}

	size_t Json::parse(shared_ptr<DataStructs::JsonNode>& node, String const& jsonString, size_t pos)
	{
		bool inObj = false, isDef = false;
		String key, value;

		for (size_t i = pos; i < jsonString.len(); ++i)
		{
			switch (jsonString[i])
			{
			case '{':
				if (inObj)
				{
					if (isDef)
					{
						auto t = node->createChildNode(key);
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
							i = extractValue(jsonString, key, '"', i + 1, [](char ch) { return (isdigit(ch) || isalpha(ch)); }, []() { throw "ошибка"; });
						}
					}
					else
					{
						if (isDef)
						{
							i = extractValue(jsonString, value, '"', i + 1, [](char ch) { return (isdigit(ch) || isalpha(ch)); }, []() { throw "ошибка"; });
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
					if ((!value.empty()) && (!key.empty()))
					{
						auto t = node->createChildNode(key);
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
							i = extractValue(jsonString, value, ']', i + 1, [](char ch) { return ch != ',' && ch != '"'; }, [&value]() {/*value.push_back(' ');*/ });
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
							i = extractValue(jsonString, value, ',', i, [](char ch) { return (isdigit(ch)); }, []() { throw "ошибка"; }) - 1;
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
}



/*
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
			cout << it->first << ' ' << it->second->value << '\n';
		}
	}
}*/
/*
void parseFromString(string const& jsonString)
{
	string newjsonString;
	for_each(jsonString.begin(), jsonString.end(), [&newjsonString](char c) { if ((c != ' ') && (c != '\t') && (c != '\n') && (c != '\r')) { newjsonString.push_back(c); } });
	newjsonString.shrink_to_fit();

	shared_ptr<DataStructs::JsonNode> node = make_shared<DataStructs::JsonNode>();

	parse(node, newjsonString, 0);

	print(node);
}*/
