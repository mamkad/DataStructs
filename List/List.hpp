#pragma once

#include <memory>

using std::unique_ptr;
using std::shared_ptr;
using std::move;

#include <iostream>
using namespace std;

namespace DataStructs
{
    template<typename Type_t>
    class List final
    {
    private:
        struct Node 
        {
            Node(Type_t const& value) : value_(value), nextNode_(nullptr) 
            {   
                //cout << "Node" << endl;
            }
            ~Node()
            {
                //cout << "~Node" << endl;
            }
            /*shared_ptr<*/Type_t/*>*/ value_;
            unique_ptr<Node> nextNode_;
        };
    private:
        unique_ptr<Node> head_ = nullptr;
        size_t len_ = 0;
    public:
        List();
        ~List();
        List(List const&);
        List(List&&);
    public:
        List& operator=(List const&);
        List& operator=(List&&);
    public:
        void push(Type_t const& newValue);
        void pop();
    public:
        bool empty() const noexcept;
        size_t len() const noexcept;
    public:
        void clear();
    };

    template<typename Type_t>
    List<Type_t>::List()
    {
    }

    template<typename Type_t>
    List<Type_t>::~List()
    {
        clear();
        //cout << '~' << endl;
    }
    //------------------------------------------------------------------------------------------------------
    template<typename Type_t>
    void List<Type_t>::push(Type_t const& newValue)
    {
        unique_ptr<Node> newNode = make_unique<Node>(newValue);
        newNode->nextNode_ = move(head_);
        head_ = move(newNode);
        ++len_;
    }

    template<typename Type_t>
    void List<Type_t>::pop()
    {
        if(!empty())
        {
            unique_ptr<Node> newHead = move(head_->nextNode_);
            head_.reset();
            head_ = move(newHead);
            --len_;
        }
    }
    //------------------------------------------------------------------------------------------------------
    template<typename Type_t>
    bool List<Type_t>::empty() const noexcept
    {
        return head_ == nullptr;
    }
    template<typename Type_t>
    size_t List<Type_t>::len() const noexcept
    {
        return len_;
    }
    //------------------------------------------------------------------------------------------------------
    template<typename Type_t>
    void List<Type_t>::clear() 
    {
        while (!empty())
        {
            head_ = move(head_->nextNode_);
            --len_;
        }
    }
}