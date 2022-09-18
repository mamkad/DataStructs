#pragma once

#include <utility>
#include <algorithm>

#include <iostream>
using std::cout;

using std::copy_n;
using std::swap;
using std::for_each;
using std::move;
using std::logic_error;
using std::invalid_argument;
using std::initializer_list;


template<typename type_t>
class Arr final
{
private:
    enum {STDSIZE = 64, KOEFF = 3};

private:
    size_t cap_, len_;
    type_t* ptr_;

private:
    void alloc(size_t cap, size_t len = 0)
    {
        cap_ = cap;
        len_ = len;
        ptr_ = new type_t[cap_];
    }

    void reset()
    {
        cap_ = 0;
        len_ = 0;
        ptr_ = nullptr;
    }

    void free()
    {
        if(!emptyCap())
        {
            cout << "free\n";
            delete[] ptr_;
            reset();
        }
    }

    inline bool emptyCap() const noexcept
    {
        return (ptr_ == nullptr);
    }

    void swp(Arr& otherArr)
    {
        swap(cap_, otherArr.cap_);
        swap(len_, otherArr.len_);
        swap(ptr_, otherArr.ptr_);
    }

public:
    Arr() 
    {
        cout << "Arr()\n";
        alloc(STDSIZE);
    }
  
    explicit Arr(size_t size)
    {
        cout << "Arr(size_t size)\n";
        alloc(size);
    }

    Arr(type_t const* ptr, size_t cap)
    {
        cout << "Arr(type_t const* ptr, size_t cap)\n";
        alloc(cap, cap);
        copy_n(ptr, len(), ptr_);
    }

    Arr(initializer_list<type_t> l)
    {
        cout << "Arr(initializer_list<type_t> l)\n";
        alloc(l.size(), l.size());
        copy_n(l.begin(), len(), ptr_);
    }

    Arr(Arr const& otherArr)
    {
        cout << "Arr(Arr const& otherArr)\n";
        alloc(otherArr.cap_, otherArr.len_);
        copy_n(otherArr.ptr_, otherArr.len_, ptr_);
    }
  
    Arr(Arr&& otherArr) noexcept
    {
       cout << "Arr(Arr&& otherArr)\n";
       reset();
      //*this = move(otherArr);
      swp(otherArr);
    }

    Arr& operator = (Arr const& otherArr)
    {
        if (this != &otherArr)
		{
            cout << "Arr& operator = (Arr const& otherArr)\n";
            free();
            alloc(otherArr.cap_, otherArr.len_);
            copy_n(otherArr.ptr_, len(), ptr_);
        }

        return *this;
    }

    Arr& operator = (Arr&& otherArr) noexcept
    {
        if (this != &otherArr)
		{
            cout << "Arr& operator = (Arr&& otherArr)\n";
            free();
            swp(otherArr);
        }

        return *this;
    }
 
    ~Arr()
    {
        cout << "~Arr()\n";
        free();
    }

public:
    void push(type_t const& value)
    {
        if (len() >= cap())
        {
             resize(KOEFF * cap());
        }

        ptr_[len_++] = value;
    }

    void pop()
    {
        if (empty())
        {
            throw logic_error("array is empty");
        }

        --len_;
    }

    void pushf()
    {

    }

    void popf()
    {

    }

    void front()
    {
        
    }

    void last()
    {

    }

    void clear()
    {
        if (empty())
        {
            throw logic_error("array is already empty");
        }

        len_ = 0;
    }

    void reverse()
    {
        
    }

    void resize(size_t newSize)
    {
        Arr newArr(newSize);
        for_each(ptr_, ptr_ + len(), [&newArr](type_t const& v){ newArr.ptr_[newArr.len_++] = v; });
        swp(newArr);
    }

    void reserve(size_t newSize)
    {
        
    }

    void shrink()
    {
        if (empty())
        {
            throw logic_error("array is empty");
        }

        if (len() == cap())
        {
            return;
        }

        resize(len());
    }

public:
    inline type_t& operator [] (size_t idx) noexcept
    {
        return  ptr_[idx];
    }

    inline type_t const& operator [] (size_t idx) const noexcept
    {
        return  ptr_[idx];
    }

    inline type_t& at (size_t idx)
    {
        if (idx < len())
        {
            return  ptr_[idx];
        }

        throw invalid_argument("wrong index");
    }

    inline type_t const& at (size_t idx) const
    {
        if (idx < len())
        {
            return  ptr_[idx];
        }

        throw invalid_argument("wrong index");
    }

public:
    inline size_t len() const noexcept
    {
        return len_;
    }

    inline size_t cap() const noexcept
    {
        return cap_;
    }

    inline type_t const* const ptr() const noexcept
    {
        return ptr_;
    }

    inline bool empty() const noexcept
    {
        return (len_ == 0);
    }
};