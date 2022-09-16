#include "Json.hpp"

#include <algorithm>
#include <iostream>

using std::cout;
using std::for_each;
using std::move;
using std::exchange;
using std::logic_error;

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

		value_.push_back(value);
	}

	String& JsonNode::getValue()
	{
		
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
	
	function<bool(char)> Json::isNotDigAlph = [](char ch) { return !(isdigit(ch) || isalpha(ch)); };
	function<bool(char)> Json::isNotDig = [](char ch) { return !(isdigit(ch)); };
	
	Json::Json()
	{
		cout << "Json()\n";
	}

	Json::Json(String const& fileName)
	{
		cout << "Json(String const& fileName)\n";
		parseFromFile(fileName);
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
		return *this;
	}

	Json& Json::operator=(Json&& otherJson)
	{
		cout << "Json::operator=(Json&& otherJson)\n";
		return *this;
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
		//parseFromString("");
	}

	void Json::parseFromString(String& jsonString)
	{
		try
		{
			String tmpJsonString = jsonString.trim();
			parse(rootNode_, tmpJsonString, 0);
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
	size_t Json::extract(/*String const& jsonString, String& saveWord*/)
	{
		/*
		int start = i + 1;
		int end = jsonString.pos(start, '"');

		if (end == -1)
		{
			throw logic_error("Не найден символ \"");
		}

		key = jsonString.slice(start, end - start);

		if (key.count(isNotDigAlph))
		{
			throw logic_error("Ключи должны содержать только символы латинского алфавита");
		}*/
		return 6;

	}
	
	size_t Json::parse(shared_ptr<DataStructs::JsonNode>& node, String& jsonString, size_t pos)
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
						throw logic_error("Неправильно использован символ {");
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
							throw logic_error("Неправильно использован символ }");
						}
						else
						{
							return i;
						}
					}
					else
					{
						throw logic_error("Неправильно использован символ }");
					}
				}
				else
				{
					throw logic_error("Неправильно использован символ }");
				}
				break;
			case '"':
				if (inObj)
				{
					if (key.empty())
					{
						if (isDef)
						{
							throw logic_error("45Неправильно использован символ \"");
						}
						else
						{
							int start = i + 1;
							int end = jsonString.pos(start, '"');

							if (end == -1)
							{
								throw logic_error("Не найден символ \"");
							}

							key = jsonString.slice(start, end - start);

							if (key.count(isNotDigAlph))
							{
								throw logic_error("Ключи должны содержать только символы латинского алфавита");
							}

							i = end;
						}
					}
					else
					{
						if (isDef)
						{
							int start = i + 1;
							int end = jsonString.pos(start, '"');

							if (end == -1)
							{
								throw logic_error("Не найден символ \"");
							}

							value = jsonString.slice(start, end - start);

							if (value.count(isNotDigAlph))
							{
								throw logic_error("Ключи должны содержать только символы латинского алфавита");
							}

							isDef = false;
							i = end;
						}
						else
						{
							throw logic_error("3Неправильно использован символ \"");
						}
					}
				}
				else
				{
					throw logic_error("2Неправильно использован символ \"");
				}
				break;
			case ':':
				if (inObj)
				{
					if (key.empty())
					{
						throw logic_error("Не указан ключ");
					}
					else
					{
						isDef = true;
					}
				}
				else
				{
					throw logic_error("Неправильно использован символ :");
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
						throw logic_error("Неправильно использован символ , ");
					}
				}
				else
				{
					throw logic_error("Неправильно использован символ , ");
				}
				break;
			case '[':
				if (inObj)
				{
					if (key.empty())
					{
						throw logic_error("Не указан ключ");
					}
					else
					{
						if (isDef)
						{
							int start = i + 1;
							int end = jsonString.pos(start, ']');

							if (end == -1)
							{
								throw logic_error("Не найден символ ["); 
							}

							vector<String> args;
							jsonString.slice(start, end - start).split(args, 0, ',');

							isDef = false;
							i = end;
						}
						else
						{
							throw logic_error("Неправильно использован символ [");
						}
					}
				}
				else
				{
					throw logic_error("Неправильно использован символ [");
				}
				break;
			case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if (inObj)
				{
					if (key.empty())
					{
						throw logic_error("Не указан ключ");
					}
					else
					{
						if (isDef)
						{
							int start = i;
							int end = jsonString.pos(start, ',');

							if (end == -1)
							{
								logic_error("Не найден символ ,"); 
							}

							value = jsonString.slice(start, end - start);

							if (value.count(isNotDig))
							{
								logic_error("Неправильно задано значение");
							}

							isDef = false;
							i = end - 1;
						}
						else
						{
							throw logic_error("Ошибка дока");
						}
					}
				}
				else
				{
					throw logic_error("Значение вне документа");
				}
				break;
			default:
				throw logic_error("Неизвестный символ");
				break;
			}
		}
		return 0;
	}
}

int main()
{
	DataStructs::String h = "{\"name\":\"Anton\",\"lastname\":\"Rudenok\",\"age\":34,\"address\":{\"city\":\"SpB\",\"streets\":{\"lenin\":34,\"pushkin\":456,},\"area\":3423423,},\"postalCode\":101101,}";
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
	
	
	DataStructs::Json json;
	try
	{
		try 
		{
			json.parseFromString(h);
		}
		catch(...)
		{
			throw;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << '\n' << e.what() << '\n';
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
