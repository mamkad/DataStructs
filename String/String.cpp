#include "String.hpp"

#include <cstring>
#include <iterator>
#include <utility>
#include <algorithm>
#include <iostream>

using std::move;
using std::logic_error;
using std::invalid_argument;
using std::ostream_iterator;
using std::copy_n;
using std::cout;
using std::to_string;
using std::for_each_n;
using std::swap;

namespace DataStructs
{
	void String::initBuff(size_t size)
	{
		if(!size)
		{
			len_ = 0;
			buff_ = nullptr;
		}
		else 
		{
			len_ = size;
			buff_ = new char[len_];
		}
	}

	String::String()
	{
		cout << "String()\n";
		initBuff();
	}

	String::String(char const* CstyleString)
	try 
	{
		cout << "String(char const* CstyleString)\n";

		initBuff(strlen(CstyleString) + 1);
		copy_n (CstyleString, len_, buff_);
		buff_[len_ - 1] = '\0';
	}
	catch(...)
	{
		throw;
	}

	String::String(size_t count, char chr)
	{
		cout << "String(size_t count, char chr)\n";

		initBuff(count + 1);
		fill_n(buff_, len_, chr);
		buff_[len_ - 1] = '\0';
	}
	catch(...)
	{
		throw;
	}

	String::String(size_t size)
	{
		cout << "String(size_t size)\n";

		initBuff(size + 1);
		fill_n(buff_, len_, '\0');
	}
	catch(...)
	{
		throw;
	}

	String::String(String const& otherString)
	try
	{	
		cout << "String(String const& otherString)\n";

		initBuff(otherString.len_);
		copy_n (otherString.buff_, len_ - 1, buff_);
		buff_[len_ - 1] = '\0';
	}
	catch(...)
	{
		throw;
	}

	String::String(String&& otherString) noexcept
	{
		cout << "String(String&& otherString)\n";

		initBuff();
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

			initBuff(otherString.len_);
			copy_n (otherString.buff_, len_ - 1, buff_);
			buff_[len_ - 1] = '\0';
		}
		return *this;
	}

	String& String::operator= (String&& otherString) noexcept
	{
		if (this != &otherString)
		{
			cout << "operator= (String&& otherString)\n";

			clear();

			initBuff();
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

		if (index > len_ - 1)
		{
			throw invalid_argument("wrong index. len of string: " + to_string(len_ - 1));
		}

		return buff_[index];
	}

	char const& String::at (size_t index) const
	{
		if (empty())
		{
			throw logic_error("string is empty");
		}

		if (index > len_ - 1)
		{
			throw invalid_argument("wrong index. len of string: " + to_string(len_ - 1));
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

		delete[] buff_;	// удаляем буфер
		initBuff();     // обнуляем указатель на буфер и длину
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

		initBuff(size + 1)
		fill_n(buff_, len_, '\0');
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	ostream& operator << (ostream & outStream, String const & stringClass)
	{
		if(!stringClass.empty())
		{
			copy_n(stringClass.buff_, stringClass.len_, ostream_iterator<char> (outStream));
		}

	    return outStream;
	}

	istream& operator >> (istream & inStream, String & otherString)
	{
		if(otherString.empty())
		{
			throw invalid_argument("buffer of string is empty");
		}

		char ch = '\0';
		size_t i = 0;
		for(; (i < len_) && (inStream.get(ch)) && (ch != ' '); ++i)
		{
			buff_[i] = ch;
		}

		i = (i < len_) ? i : len_ - 1;
		buff_[i] = '\0';

	    return inStream;
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	String String::trim()
	{
		String newString;
		size_t newSize = 0;

		auto cond = [](char ch) {return (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\a'); };

		for_each_n(buff_, len_, [&newSize, &cond](char ch) { if(cond(ch)) { ++newSize; } } );

		newString.reserve(newSize);

		size_t idx = 0;
		for_each_n(buff_, len_, [&idx, &cond, &newString](char ch) { if(cond(ch)) { newString[idx++] = ch; } } );

		return newString;
	}
}