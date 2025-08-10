
#ifndef __DENSE_INTRUSIVE_LINKED_LIST__
#define __DENSE_INTRUSIVE_LINKED_LIST__

// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <thread>
//#include "assert.hpp"

#include <iostream> // This will need to go
#include <string> // This will need to go 
#include <cassert>



namespace mlc {

    template<typename T>
    class intrusive_dense_list;

    template<typename T>
    class intrusive_dense_list_iterator;

    template<typename T>
    class intrusive_dense_list_node;

    template<typename T>
    class threading_iterator;

    template<typename T>
    class threading_iterator : public std::thread {

        public:

            // Gonna need a constructor that captures functions
            // threading_iterator(template_function_here) {
            //  Create the thread and append it to the vector 
            //}

            // Gonna need a constructor that captures the operators and uses the operators here instead for threading. Add it to the vector
            ~threading_iterator() noexcept {

            }
            // Create a emthod called start that automatically creates another pid, and keeps track of the pid's and kills them when they are done 
            // Once they are all done, kill the process itself 

    };


    template <typename T>
    class intrusive_dense_list_node  {

        friend class intrusive_dense_list<T>;
        friend class intrusive_dense_list_iterator<T>;

        public:

            T lvalue;

            // Constructs a node type
            explicit intrusive_dense_list_node(T val) : lvalue(val) {}

        private:

            std::shared_ptr<intrusive_dense_list_node<T>> next;
            std::shared_ptr<intrusive_dense_list_node<T>> prev;
    
    };

    template<typename T>
    class intrusive_dense_list_iterator {

        public:

            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;

            // If value_type is const, we want "const intrusive_list_node<value_type>", not "intrusive_list_node<const value_type>"
            using node_type = std::conditional_t<std::is_const<value_type>::value,
                                                const intrusive_dense_list_node<std::remove_const_t<value_type>>,
                                                intrusive_dense_list_node<value_type>>;

            intrusive_dense_list_iterator(const intrusive_dense_list_iterator& other) = default;
            intrusive_dense_list_iterator& operator=(const intrusive_dense_list_iterator& other) = default;
            explicit intrusive_dense_list_iterator(node_type* node) : current(node) {}

            intrusive_dense_list_iterator& operator++()
            {
                if (current) current = current->next.get();
                else return *this;
                return *this;
            }

            intrusive_dense_list_iterator& operator--() 
            {
                if (!current) return *this; 
                else current = current->prev.get();
                return *this;
            }

            intrusive_dense_list_iterator operator--(int) 
            {
                intrusive_dense_list_iterator it(*this);
                --(*this);
                return it;
            }

            intrusive_dense_list_iterator operator++(int)
            {
                intrusive_dense_list_iterator it(*this);
                ++*this;
                return it;
            }

            node_type operator[](uint16_t index) {  return static_cast<node_type>(*current); } 

            bool operator==(const intrusive_dense_list_iterator& other) const
            {
                return current == other.node;
            }

            bool operator!=(const intrusive_dense_list_iterator& other) const
            {
                return !operator==(other);
            }

            
        private:

            node_type* current = nullptr;

    };

    template <typename T>
    class intrusive_dense_list {

        friend class intrusive_dense_list_iterator<T>;

        public:

            using size_type = std::size_t;
            using value_type = T;
            using iterator = intrusive_dense_list_iterator<value_type>;
            using const_iterator = intrusive_dense_list_iterator<const value_type>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            using node_type = intrusive_dense_list_iterator<T>::node_type;
            
            /** ---------------------------------------------------------------------------------------
             * @brief Method that gets the the linked list from the array from whatever was added last 
             * 
             * 
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator begin() {
        
                assert(!data.empty());
                auto node = *data.begin();
                return iterator(node.get());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief 
             * 
             * @note
             * 
             * @return
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_iterator begin() const { 
                
                assert(!data.empty()); 
                return const_iterator(*data.begin());
            }

            const_iterator cbegin() const { 

                assert(!data.empty());
                auto node = *data.cbegin();

                return const_iterator(node.get()); 
            }

            /** ---------------------------------------------------------------------------------------
             * @brief 
             * 
             * @note
             * 
             * @return
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator end() { 
                
                assert(!data.empty() && data.size() > 1); 
                return iterator(*data.end());
            }
            
            /** ---------------------------------------------------------------------------------------
             * @brief 
             * 
             * @note
             * 
             * @return
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_iterator end() const { 

                assert(!data.empty() && data.size() > 1); 
                return const_iterator(*data.end());
            }

            const_iterator cend() const { 

                assert(!data.empty() && data.size() > 1); 
                auto node = *data.cend();
                return const_iterator(node.get()); 
            }

            reverse_iterator rbegin() { return reverse_iterator(end()); }
            const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
            const_reverse_iterator crbegin() const { return rbegin(); }

            reverse_iterator rend() { return reverse_iterator(begin()); }
            const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
            const_reverse_iterator crend() const { return rend(); }        

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator at(uint16_t index) {
                
                assert(index < data.size());
                auto& node = data.at(index);
                return iterator(node.get());
            }

            void insert(uint16_t index, T value) {

                std::shared_ptr<node_type> new_node = std::make_shared<node_type>(value);
                if (index < data.size()) data.at(index).swap(new_node);
                else data.insert(data.begin() + index, data.end());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Inserts nodes at behind the root, but in the middle
             * 
             * @note Operates on the desired element in the vector. To operate on the nodes itself and add the series of nodes to the backend.
             *       You would use the node methods and then use the backend swap method 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void insert_before(uint16_t index, T value) {
                
                assert(index < data.size());

                auto new_node = std::make_shared<node_type>(value);     
                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));
                
                if (!current_node->next) {
                    
                    current_node->prev = new_node;
                    new_node->next = current_node;

                    data.at(static_cast<size_type>(index)).swap(new_node);

                    return;
                }
                
                auto old_next = current_node->next; 
                auto original_prev = old_next->prev;
                    
                new_node->prev = original_prev;
                new_node->next = old_next;

                original_prev->next = new_node;
                old_next->prev = new_node;

                data.at(static_cast<size_type>(index)).swap(current_node);

                return;
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Inserts nodes at after the root, but in the middle
             * 
             * @note Operates on the desired element in the vector. To operate on the nodes itself and add the series of nodes to the backend.
             *       You would use the node methods and then use the backend swap method 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
           void insert_after(uint16_t index, T value) {

                assert(index < data.size());
                auto new_node = std::make_shared<node_type>(value);
                
                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));
                
                new_node->prev = current_node;
                new_node->next = current_node->next;
                
                if (current_node->next) current_node->next->prev = new_node;
                
                current_node->next = new_node;
                
                data.at(static_cast<size_type>(index)).swap(current_node);
                return;
            }

            void insert_begining(uint16_t index, T value) {

                assert(index < data.size());
                auto new_node = std::make_shared<node_type>(value);  

                std::shared_ptr<node_type> current_node; 
                current_node.swap(data.at(static_cast<size_type>(index)));

                while (current_node->prev) current_node = current_node->prev;

                new_node->next = current_node->next;
                new_node->prev = current_node->prev;

                debug_node(new_node, true);

                data.at(static_cast<size_type>(index)).swap(new_node);

                return;
            }

            void insert_end(uint16_t index, T value) {

                assert(index < data.size());
                auto new_node = std::make_shared<node_type>(value); 

                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));

                while (current_node->next) current_node = current_node->next;

                auto old_tail = current_node; 

                old_tail->next = new_node;
                new_node->prev = old_tail;

                debug_node(old_tail, true);

                data.at(index).swap(old_tail);

                return;
            }

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            void push_back(T value) {

                auto new_node = std::make_shared<node_type>(value);
                data.push_back(new_node);

            }

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            void pop_back() {

                assert(!data.empty());
                data.pop_back();
                data.shrink_to_fit();

            }

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            void erase(uint16_t index) {

                assert(data.size() > 1 && !data.empty());
                data.erase(data.begin() + index, data.end());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator front() {

                assert(!data.empty());
                return iterator(data.front());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator back() {

                assert(!data.empty()); 
                return iterator(data.back());
            }
            
            /** ---------------------------------------------------------------------------------------
             * @brief
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            value_type get_value(uint16_t index) { return data.at(static_cast<size_t>(index))->lvalue; }
  

        protected:
                  
            // This needs to be moved somewhere else 
            static void debug_node(std::shared_ptr<intrusive_dense_list_node<T>> node, bool is_forward = false) {
                std::cout << "\n\n=============================================\n";
                std::cout << "===          NODE DEBUGGER              ===\n";
                std::cout << "=============================================\n\n";

                auto traverse_forward = [](std::shared_ptr<intrusive_dense_list_node<T>> node) 
                {
                    std::cout << "FORWARD TRAVERSAL:\n";
                    std::cout << "-------------------\n";
                    
                    int count = 0;
                    std::string progress;
                    
                    while (node) 
                    {
                        std::cout << "Step " << ++count << ":\n";
                        std::cout << "  Node address: " << node.get() << "\n";
                        std::cout << "  Node value:   " << node->lvalue << "\n";
                        std::cout << "  Next node:    " << (node->next ? node->next.get() : nullptr) << "\n";
                        std::cout << "  Prev node:    " << (node->prev ? node->prev.get() : nullptr) << "\n";
                        
                        progress.append("[ ").append(std::to_string(node->lvalue)).append(" ] -> ");
                        std::cout << "  Current path: " << progress << "\n\n";
                        
                        node = node->next;
                        
                        if (count > 20) {
                            std::cout << "WARNING: Potential infinite loop detected!\n";
                            break;
                        }
                    }
                    
                    std::cout << "END OF CHAIN REACHED\n";
                    std::cout << "Total nodes traversed: " << count << "\n";
                };

                auto traverse_backward = [](std::shared_ptr<intrusive_dense_list_node<T>> node) 
                {
                    std::cout << "BACKWARD TRAVERSAL:\n";
                    std::cout << "-------------------\n";
                    
                    int count = 0;
                    std::string progress;
                    
                    while (node) 
                    {
                        std::cout << "Step " << ++count << ":\n";
                        std::cout << "  Node address: " << node.get() << "\n";
                        std::cout << "  Node value:   " << node->lvalue << "\n";
                        std::cout << "  Next node:    " << (node->next ? node->next.get() : nullptr) << "\n";
                        std::cout << "  Prev node:    " << (node->prev ? node->prev.get() : nullptr) << "\n";
                        
                        progress.append("[ ").append(std::to_string(node->lvalue)).append(" ] -> ");
                        std::cout << "  Current path: " << progress << "\n\n";
                        
                        node = node->prev;
                        
                        if (count > 20) {
                            std::cout << "WARNING: Potential infinite loop detected!\n";
                            break;
                        }
                    }
                    
                    std::cout << "END OF CHAIN REACHED\n";
                    std::cout << "Total nodes traversed: " << count << "\n";
                };

                if (is_forward) traverse_forward(node);
                else traverse_backward(node);

                std::cout << "\n=============================================\n";
                std::cout << "===        END OF DEBUG OUTPUT          ===\n";
                std::cout << "=============================================\n\n";
            };

            
        private:

            inline static std::vector<std::shared_ptr<node_type>> data;
    };
 
};

#endif
