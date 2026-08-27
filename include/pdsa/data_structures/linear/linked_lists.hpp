#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
namespace psda
{
    namespace detail
    {
        template<typename T>
        class linked_list_node
        {
        private:
            T value;
            linked_list_node<T>* next;
        public:
            linked_list_node(T key): value(key), next(nullptr) {}
            ~linked_list_node()
            {
                delete next;
            }
        };
        
        template<typename T>
        class linked_list_node_double
        {
        private:
            T value;
            linked_list_node<T>* next, previous;
            public:
            linked_list_node_double(T key): value(key), next(nullptr), previous(nullptr) {}
        };
    }

    template<typename T>
    class linked_list
    {
    private:
        detail::linked_list_node<T>* head;
        detail::linked_list_node<T>* tail;
        std::size_t _size;
    public:
        linked_list(): head(new detail::linked_list_node<T>()), tail(head), _size(0) {}
        ~linked_list()
        {
            delete head;
        }
        std::size_t size() 
        {
            return _size;
        }
        template<typename T>
        void push_back(T key)
        {
            //
        }
        template<typename T>
        void push_front(T key)
        {
            detail::linked_list_node<T>* new_node = new detail::linked_list_node(key);
            new_node->next = head->next;
            head->next = new_node;
        }
    };

    
}

#endif