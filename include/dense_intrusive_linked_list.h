#ifndef __DENSE_INTRUSIVE_LINKED_LIST__
#define __DENSE_INTRUSIVE_LINKED_LIST__

// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

//#include "assert.hpp"

namespace mlc {

    template<typename T>
    class intrusive_dense_list;

    template<typename T>
    class intrusive_dense_list_node;

    template<typename T>
    inline static std::vector<std::shared_ptr<intrusive_dense_list_node<T>>> data;

    template<typename T>
    class intrusive_dense_list_node {

        public:

            intrusive_dense_list_node() = default;
            ~intrusive_dense_list_node() noexcept = default;

            intrusive_dense_list_node* next = nullptr;
            intrusive_dense_list_node* prev = nullptr;
            T lvalue;
            
    };

    template<typename T>
    class intrusive_dense_list_iterator {

        /** ----------------------------------
         * @brief A base class that will be treated such contains overwritten operators.
         * 
         * @note It is inherited by intrusive_dense_list<T> class for the polymorphism 
         * 
        */
        public:

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference = intrusive_dense_list_node<value_type>;
            using iterator = intrusive_dense_list_iterator<value_type>;
            using const_reference = const intrusive_dense_list_node<value_type>;

            // If value_type is const, we want "const intrusive_dense_list_node<value_type>", not "intrusive_dense_list_node<const value_type>"
            using node_type = std::conditional_t<std::is_const<value_type>::value,
                                                const intrusive_dense_list_node<std::remove_const_t<value_type>>,
                                                intrusive_dense_list_node<value_type>>;

            intrusive_dense_list_iterator(const intrusive_dense_list_iterator& other) = default;
            intrusive_dense_list_iterator& operator=(const intrusive_dense_list_iterator& other) = default;

            explicit intrusive_dense_list_iterator(node_type&& data) {

                node = std::make_unique<node_type>(std::move(data));
            }

            intrusive_dense_list_iterator& operator++() {

                node = std::make_unique<node_type>(*node->next);
                return *this;
            }
            intrusive_dense_list_iterator& operator--()
            {
                node = std::make_unique<node_type>(*node->prev);
                return *this;
            }
            intrusive_dense_list_iterator operator++(int)
            {
                intrusive_dense_list_iterator it(*this);
                ++*this;
                return it;
            }
            intrusive_dense_list_iterator operator--(int)
            {
                intrusive_dense_list_iterator it(*this);
                --*this;
                return it;
            }

            bool operator==(const intrusive_dense_list_iterator& other) const
            {
                return node.get() == other.node.get();
            }
            bool operator!=(const intrusive_dense_list_iterator& other) const
            {
                return !operator==(other);
            }

            reference operator*() const
            {   
                return static_cast<reference>(*node);
            }

            // Indicing Support
            const_reference operator[](uint16_t index) {
                
                node.swap(node.get() != data<T>.at(static_cast<size_t>(index)).get() ? node = std::make_unique<node_type>(*data<T>.at(static_cast<size_t>(index)).get()) : node);
                return static_cast<reference>(*node);
            } 
            
            /**
             * Erases a node from this list.
             * @param node A pointer to the node to erase from this list.
             */
            iterator erase(node_type old_node)
            {
                return erase(iterator(old_node));
            }
        
        private:

            inline static std::unique_ptr<node_type> node;
    };

    template<typename T>
    class intrusive_dense_list {

        public:

            using difference_type = std::ptrdiff_t;
            using size_type = std::size_t;
            using value_type = T;
            using reference = intrusive_dense_list_node<value_type>;
            using const_reference = const intrusive_dense_list_node<value_type>;
            using iterator = intrusive_dense_list_iterator<value_type>;
            using const_iterator = intrusive_dense_list_iterator<const value_type>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            /** ---------------------------------------------------------------
             * @brief Function that inserts a node into the backend 
             *
             * @param location The location to insert the node.
             * @param new_node The node to add.
            */
            iterator insert(iterator location, reference new_node)
            {
                return insert_before(location, new_node);
            }

            /**
             * Inserts a node at the given location, moving the previous
             * node occupant ahead of the one inserted.
             *
             * @param location The location to insert the new node.
             * @param new_node The node to insert into the list.
             */
            iterator insert_before(iterator location, reference new_node)
            {
                auto existing_node = *data<T>.begin();

                new_node.next = existing_node.get();
                new_node.prev = existing_node.get()->prev;
                existing_node->prev->next = &new_node;
                existing_node->prev = &new_node;

                return iterator(std::move(new_node));
            }

            /**
             * Inserts a new node into the list ahead of the position indicated.
             *
             * @param position Location to insert the node in front of.
             * @param new_node The node to be inserted into the list.
             */
            iterator insert_after(iterator position, reference new_node)
            {
                if (empty())
                    return insert(begin(), new_node);

                return insert(++position, new_node);
            }

            /**
             * Add an entry to the start of the list.
             * @param node Node to add to the list.
            */
            void push_front(reference new_node)
            {
                insert(begin(), new_node);
            }

            /**
             * Erases the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            void pop_front()
            {
                //DEBUG_ASSERT(!empty());
                //erase(begin());
            }

            /**
             * Erases the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            void pop_back()
            {
                //DEBUG_ASSERT(!empty());
                //erase(--end());
            }

            /**
             * Is this list empty?
             * @returns true if there are no nodes in this list.
             */
            bool empty() const
            {
                throw("FMI");
            }

            /**
             * Gets the total number of elements within this list.
             * @return the number of elements in this list.
             */
            size_type size() const
            {
                return static_cast<size_type>(this->data.size());
            }

            /**
             * Retrieves a reference to the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            reference front()
            {
                throw("FMI");
            }

            /**
             * Retrieves a constant reference to the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            const_reference front() const
            {
               throw("FMI");
            }

            /**
             * Retrieves a reference to the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            reference back()
            {
                throw("FMI");
                //DEBUG_ASSERT(!empty());
                //return *--end();
            }

            /**
             * Retrieves a constant reference to the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            const_reference back() const
            {
                
                throw("FMI");
                //DEBUG_ASSERT(!empty());
                //return *--end();
            }
            
            // Iterator interface
            iterator begin() {

                static std::shared_ptr<reference> node = *data<T>.begin(); 
                return iterator(std::move(*node)); 
            }
            const_iterator begin() const { 

                static std::shared_ptr<reference> node = *data<T>.begin(); 
                return const_iterator(std::move(*node)); 
            }
            const_iterator cbegin() const { return begin(); }


            iterator end() { 

                static std::shared_ptr<reference> node = *data<T>.end(); 
                return iterator(std::move(*node)); 
            }

            const_iterator end() const { 

                static std::shared_ptr<reference> node = *data<T>.end(); 
                return const_iterator(std::move(*node)); 
            }

            const_iterator cend() const { return end(); }

            reverse_iterator rbegin() { return reverse_iterator(end()); }
            const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
            const_reverse_iterator crbegin() const { return rbegin(); }

            reverse_iterator rend() { return reverse_iterator(begin()); }
            const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
            const_reverse_iterator crend() const { return rend(); }

            /**
             * Erases a node from the list, indicated by an iterator.
             * @param it The iterator that points to the node to erase.
             */
            iterator erase(iterator it)
            {
                remove(it);
                return it;
            }

            /**
             * Erases a node from this list.
             * @param node A reference to the node to erase from this list.
             */
            iterator erase(reference new_node)
            {
                return erase(iterator(new_node));
            }

            /**
             * Exchanges contents of this list with another list instance.
             * @param other The other list to swap with.
             */
            void swap(intrusive_dense_list& other) noexcept
            {
                data<T>.swap(other.root);
            }

    };

}

#endif
