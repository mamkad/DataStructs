#include "String.hpp"

#include <cstring>
#include <iterator>
#include <utility>
#include <algorithm>

using std::move;
using std::logic_error;
using std::invalid_argument;
using std::ostream_iterator;
using std::copy_n;
using std::cout;
using std::for_each;
using std::swap;
using std::find;
using std::fill_n;

namespace DataStructs
{
	// задать нулевые значения для len_ и buf_
	void String::reset()
	{
		len_ = 0;
		buff_ = nullptr;
	}

	// выделить память и присвоить buff 
	void String::alloc(size_t size)
	{
		len_ = size + 1;
		buff_ = new char[len_];
		buff_[len_ - 1] = '\0';
	}

	String::String()
	{
		cout << "String()\n";
		reset();
	}

	String::String(char const* CstyleString)
	try
	{
		cout << "String(char const* CstyleString)\n";

		size_t cStrLen = strlen(CstyleString);

		if (cStrLen)
		{
			alloc(cStrLen);
			copy_n(CstyleString, len(), buff_);
		}
		else
		{
			reset();
		}
	}
	catch (...)
	{
		throw;
	}

	String::String(size_t count, char chr)
	try
	{
		cout << "String(size_t count, char chr)\n";

		if (count)
		{
			alloc(count);
			fill_n(buff_, len(), chr);
		}
		else
		{
			reset();
		}
	}
	catch (...)
	{
		throw;
	}

	String::String(size_t size)
	try 
	{
		cout << "String(size_t size)\n";

		if (size)
		{
			alloc(size);
			fill_n(buff_, len(), '\0');
		}
		else
		{
			reset();
		}
	}
	catch (...)
	{
		throw;
	}

	String::String(String const& otherString)
	try
	{
		cout << "String(String const& otherString)\n";

		if (!otherString.empty())
		{
			alloc(otherString.len_ - 1);
			copy_n(otherString.buff_, len(), buff_);
		}
		else
		{
			reset();
		}
	}
	catch (...)
	{
		throw;
	}

	String::String(String&& otherString) noexcept
	{
		cout << "String(String&& otherString)\n";

		reset();
		*this = move(otherString);
	}

	String::~String()
	{
		cout << "~String()\n";

		clear();
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	String& String::operator= (String const& otherString)
	{
		if (this != &otherString)
		{
			cout << "operator= (String const& otherString)\n";

			clear();

			if (!otherString.empty())
			{
				alloc(otherString.len_ - 1);

				copy_n(otherString.buff_, len(), buff_);
			}
		}
		return *this;
	}

	String& String::operator= (String&& otherString) noexcept
	{
		if (this != &otherString)
		{
			cout << "operator= (String&& otherString)\n";

			clear();
			swap(len_, otherString.len_);
			swap(buff_, otherString.buff_);
		}
		return *this;
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	char& String::operator[] (size_t index)
	{
		return buff_[index];
	}

	char const& String::operator[] (size_t index) const
	{
		return buff_[index];
	}

	char& String::at(size_t index)
	{
		if (empty())
		{
			throw logic_error("string is empty");
		}

		if (index > len())
		{
			throw invalid_argument("wrong index");
		}

		return buff_[index];
	}

	char const& String::at(size_t index) const
	{
		if (empty())
		{
			throw logic_error("string is empty");
		}

		if (index > len())
		{
			throw invalid_argument("wrong index");
		}

		return buff_[index];
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	size_t String::len() const noexcept
	{
		if (empty())
		{
			return len_;
		}

		return len_ - 1;
	}

	bool String::empty() const noexcept
	{
		return (len_ == 0) && (buff_ == nullptr);
	}

	char const* const String::getPtr() const noexcept
	{
		if (empty())
		{
			throw logic_error("string is empty");
		}

		return buff_;
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	void String::clear()
	{
		if (empty())
		{
			return;
		}

		cout << "\tdeleting String\n";

		delete[] buff_; // удаляем буфер
		reset();		// обнуляем указатель на буфер и длину
	}

	void String::reserve(size_t size)
	{
		if (!empty())
		{
			throw invalid_argument("string is not empty. clear buffer");
		}

		if (!size)
		{
			throw invalid_argument("size is zero");
		}

		alloc(size);
		fill_n(buff_, len_, '\0');
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	ostream& operator << (ostream& outStream, String const& otherString)
	{
		if (!otherString.empty())
		{
			copy_n(otherString.getPtr(), otherString.len(), ostream_iterator<char>(outStream));
		}

		return outStream;
	}

	istream& operator >> (istream& inStream, String& otherString)
	{
		if (otherString.empty())
		{
			throw invalid_argument("buffer of string is empty");
		}

		char ch = '\0';
		size_t i = 0;
		for (; (i < otherString.len()) && (inStream.get(ch)) && (ch != ' '); ++i)
		{
			otherString[i] = ch;
		}

		i = (i < otherString.len()) ? i : otherString.len();
		otherString[i] = '\0';

		return inStream;
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	// кол-во символов, удов определённому условию в строке
	size_t String::count(function<bool(char)> const& cond)
	{
		if (empty())
		{
			return 0;
		}

		size_t count = 0;
		for_each( buff_, buff_ + len_, [&cond, &count](char c) { if (cond(c)) { ++count; } } );

		return count;
	}

	int String::pos(char ch)
	{
		if (empty())
		{
			return -1;
		}

		for (int i = 0; i < len_; ++i)
		{
			if (buff_[i] == ch)
			{
				return i;
			}
		}

		return -1;
	}

	// удалить все символы, указанные в аргументе
	String String::del(set<char> const& chars)		       
	{
		if (empty())
		{
			return "";
		}

		if (chars.empty())
		{
			return "";
		}

		auto cond = [&chars](char c){for(auto it = chars.begin(); it != chars.end(); ++it){if (*it == c){return true;}}return false;};
		size_t newSize = count(cond);

		if (!newSize)
		{
			return "";
		}

		String newString(newSize);

		for (auto it = chars.begin(); it != chars.end(); ++it)
		{
			int i;
			while ((i = pos(*it))&& i != -1)
			{
				
			}
		}
	}

	String String::trim()
	{
		auto cond = [](char c) {return (c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != '\a'); };
		size_t newSize = count(cond);

		if (!newSize)
		{
			return "";
		}

		String newString(newSize);

		size_t i = 0;
		for_each(buff_, buff_ + len_, [&i, &cond, &newString](char ch) { if (cond(ch)) { newString[i++] = ch; } });
		
		return newString;
	}
}
