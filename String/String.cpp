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
using std::for_each;
using std::swap;
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
		reset();
	}

	String::String(char const* CstyleString)
	try
	{
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
		reset();
		*this = move(otherString);
	}

	String::~String()
	{
		clear();
	}
	// --------------------------------------------------------------------------------------------------------------------------------
	String& String::operator= (String const& otherString)
	{
		if (this != &otherString)
		{
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
			return nullptr;
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

		delete[] buff_; // удаляем буфер
		reset();		// обнуляем указатель на буфер и длину
	}

	void String::reserve(size_t size)
	{
		if (!empty())
		{
			throw logic_error("string is not empty. clear buffer");
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
			copy_n(otherString.buff_, otherString.len_, ostream_iterator<char>(outStream));
		}

		return outStream;
	}

	istream& operator >> (istream& inStream, String& otherString)
	{
		if (otherString.empty())
		{
			throw invalid_argument("buffer of string is empty");
		}

		otherString.fill('\0');

		char ch = '\0';
		size_t i = 0;
		for (; (i < otherString.len_) && (inStream.get(ch)) && (ch != '\n') && (ch != ' '); ++i)
		{
			otherString.buff_[i] = ch;
		}

		return inStream;
	}

	bool operator < (String const& left, String const& right)
	{
		return (strncmp(left.buff_, right.buff_, (left.len_ < right.len_) ? left.len_ : right.len_) < 0);
	}

	bool operator > (String const& left, String const& right)
	{
		return !(left < right);
	}

	bool operator == (String const& left, String const& right)
	{
		return (strncmp(left.buff_, right.buff_, (left.len_ < right.len_) ? left.len_ : right.len_) == 0);
	}

	// --------------------------------------------------------------------------------------------------------------------------------
	size_t String::count(function<bool(char)> const& cond)
	{
		size_t count = 0;
		for_each( buff_, buff_ + len(), [&cond, &count](char c) { if (cond(c)) { ++count; } } );

		return count;
	}

	int String::pos(size_t pos, char ch)
	{
		for (int i = pos; i < len(); ++i)
		{
			if (buff_[i] == ch)
			{
				return i;
			}
		}

		return -1;
	}

	int String::rpos(size_t pos, char ch)
	{
		for (int i = pos; i >= 0; --i)
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
		if (chars.empty())
		{
			return "";
		}

		auto cond = [&chars](char c){for(auto it = chars.begin(); it != chars.end(); ++it){if (*it == c){return true;}}return false;};
		size_t newSize = len() - count(cond);

		if (!newSize)
		{
			return "";
		}

		String newString(newSize);

		for(size_t i = 0, j = 0; i < len(); ++i)
		{
			if (!cond(buff_[i]))
			{
				newString[j++] = buff_[i];
			}
		}

		return newString;
	}

	String String::trim()
	{
		return del({' ', '\t', '\n', '\r', '\a'});
	}

	void String::fill(char ch)
	{
		for_each( buff_, buff_ + len_, [ch](char& c) { c = ch; } );
	}	

	String String::slice(size_t pos, size_t count)
	{
		String newString(count);

		for(size_t n = 0, i = pos; (i < len()) && (n < count); ++n, ++i)
		{
			newString[n] = buff_[i];
		}

		return newString;
	}

	void String::split(vector<String>& stringVector, size_t pos, char delim)
	{
		int endPos = this->pos(pos, delim);
		int startPos = 0;

		for( ; endPos != -1; startPos = endPos + 1, endPos = this->pos(startPos, delim))
		{
			if (endPos - startPos > 0)
			{
				stringVector.push_back(slice(startPos, endPos - startPos));
			}
		}
	}				   
}
