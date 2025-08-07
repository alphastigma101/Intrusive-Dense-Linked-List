
#ifndef __DENSE_INTRUSIVE_LINKED_LIST__
#define __DENSE_INTRUSIVE_LINKED_LIST__

// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>

#include <iostream>
#include <string>

namespace mlc {

    template<typename T>
    class intrusive_dense_list;

    template<typename T>
    class intrusive_dense_list_iterator;

    template<typename T>
    class intrusive_dense_list_node;


    template <typename T>
    class intrusive_dense_list_node : public std::enable_shared_from_this<intrusive_dense_list_node<T>> {

        friend class intrusive_dense_list<T>;
        friend class intrusive_dense_list_iterator<T>;

        public:

            T lvalue;
            bool at_center = false;
            explicit intrusive_dense_list_node(T val) : lvalue(val) {}
            explicit intrusive_dense_list_node(bool center) : at_center(center) { lvalue = {}; }

        private:
            std::shared_ptr<intrusive_dense_list_node<T>> next;
            std::shared_ptr<intrusive_dense_list_node<T>> prev;
    
    };

    template<typename T>
    class intrusive_dense_list_iterator {

        std::shared_ptr<intrusive_dense_list_node<T>> current;

        public:

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;

            // If value_type is const, we want "const intrusive_list_node<value_type>", not "intrusive_list_node<const value_type>"
            using node_type = std::conditional_t<std::is_const<value_type>::value,
                                                const intrusive_dense_list_node<std::remove_const_t<value_type>>,
                                                intrusive_dense_list_node<value_type>>;

            intrusive_dense_list_iterator(const intrusive_dense_list_iterator& other) = default;
            intrusive_dense_list_iterator& operator=(const intrusive_dense_list_iterator& other) = default;
            explicit intrusive_dense_list_iterator(std::shared_ptr<node_type> node) : current(node) {}

            intrusive_dense_list_iterator& operator++()
            {
                if (current) current = current->next;
                else return *this;
                return *this;
            }

            intrusive_dense_list_iterator& operator--() 
            {
                if (!current) return *this; 
                else current = current->prev;
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

            node_type operator[](uint16_t index) { 

                intrusive_dense_list<T>::data.at(static_cast<size_t>(index)) = current;
                return static_cast<node_type>(*current);
            } 

            //bool operator!=(const iterator& other) const { return current != other.current; }
            
    };

    template <typename T>
    class intrusive_dense_list {

        friend class intrusive_dense_list_iterator<T>;

        private:

            inline static std::vector<std::shared_ptr<intrusive_dense_list_node<T>>> data;
            std::shared_ptr<intrusive_dense_list_node<T>> head;
            std::shared_ptr<intrusive_dense_list_node<T>> tail;

        public:

            using difference_type = std::ptrdiff_t;
            using size_type = std::size_t;
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;
            using iterator = intrusive_dense_list_iterator<value_type>;
            using const_iterator = intrusive_dense_list_iterator<const value_type>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            iterator begin() {
                // Need an assert that makes sure that the array is not empty
                return iterator(data.at(data.size() - 1));

            };

            iterator at(uint16_t index) {
                // Need an assert that makes sure that the arrary is not less than index
                return iterator(data.at(index));
            };

            // Inserts nodes beging the root, but in the middle
            void insert_before(uint16_t index, T value) {
                
                if (index < data.size()) {

                    auto new_node = std::make_shared<intrusive_dense_list_node<T>>(value);     
                    auto current_node = data.at(index);
                  
                    if (current_node->next) {

                        auto old_next = current_node->next; 
                        auto original_prev = old_next->prev;
                         
                        new_node->prev = original_prev;
                        new_node->next = old_next;

                        original_prev->next = new_node;
                        old_next->prev = new_node;
                        
                        static auto update_tail_and_head = [&]() -> void {

                            // Update the head 
                            auto find_head = current_node;
                            while (find_head->prev) { find_head = find_head->prev; }
                            head = find_head;

                            // Update the tail
                            auto find_tail = current_node;
                            while (find_tail->next) { find_tail = find_tail->next; }
                            tail = find_tail;

                            return;
                            
                        };
        
                        update_tail_and_head();

                        create_bi_node();

                        data.at(index).swap(current_node);

                        return;

                    }
                    else if (!current_node->next) {
                        
                        current_node->prev = new_node;
                        new_node->next = current_node;

                        data.at(index).swap(new_node);

                        return;
                    }
                }
            };


            void insert_after(uint16_t index, T value) {
                
                if (index < data.size()) {

                    auto new_node = std::make_shared<intrusive_dense_list_node<T>>(value);
                    auto current_node = data.at(index);
                
                    new_node->next = current_node;
                    if (current_node->prev) {

                        // Link with previous node if exists
                        auto prev_node = current_node->prev;
                        prev_node->next = new_node;
                        new_node->prev = prev_node;

                    } else head = new_node;
                
                    current_node->prev = new_node;
                
                    data.at(index) = current_node;
                    
                }
                
            };

            // Push values back into the array
            void push_back(T value) {

                auto new_node = std::make_shared<intrusive_dense_list_node<T>>(value);
                data.push_back(new_node);
            };

        protected:

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

            void create_bi_node() {
               
                // Make a new node that will serve as the bi-directional node
                std::shared_ptr<intrusive_dense_list_node<T>> current_node = 
                    std::make_shared<intrusive_dense_list_node<T>>(true);

                current_node->next = head;
                current_node->prev = tail;

                data.at(data.size() - 1).swap(current_node);
                
            }
    };
 
};

#endif
