
#ifndef __DENSE_INTRUSIVE_LINKED_LIST__
#define __DENSE_INTRUSIVE_LINKED_LIST__

// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>
#include <iostream>

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
            intrusive_dense_list_node(T val) : lvalue(val) {}

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
            std::weak_ptr<intrusive_dense_list_node<T>> tail;

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
    
                        // Update links
                        original_prev->next = new_node;
                        new_node->prev = original_prev;
                        new_node->next = old_next;
                        current_node->prev = original_prev;

                        // Update head/tail if needed
                        if (!original_prev) head = new_node;
                        if (!new_node->next) tail = new_node;

                        data.at(index).swap(current_node);

                    }
                    else if (!current_node->next) {
                        
                        current_node->prev = new_node;
                        new_node->next = current_node;

                        if (current_node == head) head = new_node;
                        tail = current_node;

                        data.at(index).swap(new_node);
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

    };
 
};

#endif
