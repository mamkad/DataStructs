#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <set>

using std::ostream;
using std::istream;
using std::set;
using std::function;

namespace DataStructs
{
	class String final
	{
	private:
		size_t len_; // длина буфера памяти
		char* buff_; // указатель на буфер

	private:
		void reset();		// задать нулевые значения для len_ и buf_
		void alloc(size_t); // выделить память и присвоить buff

	public:
		String();
		String(char const*);
		String(size_t, char);
		explicit String(size_t);
		String(String const&);
		String(String&&) noexcept;
		~String();

	public:
		String& operator= (String const&);
		String& operator= (String&&) noexcept;

	public:
		char& operator[] (size_t);
		char const& operator[] (size_t) const;

		// тот же функционал но с проверкой выхода за границы
		char& at(size_t);
		char const& at(size_t) const;

	public:
		size_t len()   const noexcept;			   // длина строки
		bool   empty() const noexcept;			   // пуста ли строка
		char const* const getPtr() const noexcept;         // получиь указатель на буфер

	public:
		void clear();	      // очистить буфер
		void reserve(size_t); // выделить память под строку заранее (при этом строка должна быть пуста). задаваемый размер не должен быть равен 0

	public:
		friend ostream& operator << (ostream&, String const&);
		friend istream& operator >> (istream&, String&);

	public:
		size_t count(function<bool(char)> const&); // кол-во символов в строке, удов определённому условию
		int pos(char);				   // позиция символа
		String del(set<char> const&);		   // удалить все символы, указанные в аргументе

		String trim();				   // удалить все пробелы
	};
}
