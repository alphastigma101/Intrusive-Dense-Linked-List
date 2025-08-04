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
#include <cassert>
#include <iostream>

//#include "assert.hpp"

namespace mlc {

    template<typename T>
    class intrusive_dense_list;

    template<typename T>
    class intrusive_dense_list_node;

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

            friend class intrusive_dense_list<T>;
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

            /** ------------------------------------------
             * @brief A constructor that consists of a series of operators that can be used to iterate through the linked list.
             *        Whever you add or remove a node, this constructor will get called and will update the private member `current_node`
             * -------------------------------------------
            */
            explicit intrusive_dense_list_iterator(node_type& data) : current_node(&data) {
                std::cout << "\n===== DEBUGGING INTRUSIVE_DENSE_LIST_ITERATOR =====\n";

                std::cout << "current_node.lvaue is : " << current_node->lvalue << std::endl;
                if (current_node->next) std::cout << "current_node->next lvalue is : " << current_node->next->lvalue << std::endl;
                if (current_node->prev) std::cout << "current_node->prev is : " << current_node->prev->lvalue << std::endl;

            }

            /** -------------------------------------------
             * @brief An operator that increments private data member current_node to the next node
             * 
             * @note Is only accessable by intrusive_dense_list_iterator<T> object
             * --------------------------------------------
            */
            intrusive_dense_list_iterator& operator++() {
               
                if (current_node) current_node = current_node->next;
                else return *this;
                return *this;
            }

            /** -------------------------------------------
             * @brief An de-increment operator that gets called after operator++()
             * 
             * @note Is only accessable by intrusive_dense_list_iterator<T> object
             * --------------------------------------------
            */
            intrusive_dense_list_iterator& operator--() {

                if (current_node) current_node = current_node->prev;
                else return *this;
                return *this;
            }

            /** -------------------------------------------
             * @brief An post increment operator that gets called after operator++()
             * 
             * @note Is only accessable by intrusive_dense_list_iterator<T> object
             * --------------------------------------------
            */
            intrusive_dense_list_iterator operator++(int)
            {
                intrusive_dense_list_iterator it(*this);
                ++(*this);
                return it;
            }

            /** -------------------------------------------
             * @brief An post de-increment operator that gets called after operator--()
             * 
             * @note Is only accessable by intrusive_dense_list_iterator<T> object
             * --------------------------------------------
            */
            intrusive_dense_list_iterator operator--(int)
            {
                intrusive_dense_list_iterator it(*this);
                --*this;
                return it;
            }

            // TODO: THIS DOC STRING WILL NEED TO BE UPDATED
            /** -------------------------------------------
             * @brief An equality operator that only works if left-hand side is the same type as right-hand side
             * 
             * @note A good example would be:
             *       for (auto it = container.begin(); it != container.end(); ++it) {
             *          cout << *it << " ";
             *        }
             * --------------------------------------------
            */
            bool operator==(const intrusive_dense_list_iterator& other) const
            {
                return current_node.get() == other.current_node.get();
            }

            /** -------------------------------------------
             * @brief An non-equality operator that only works if left-hand side is the same type as right-hand side
             * 
             * @note A good example would be:
             *       for (auto it = container.begin(); it != container.end(); ++it) {
             *          cout << *it << " ";
             *        }
             * --------------------------------------------
            */
            bool operator!=(const intrusive_dense_list_iterator& other) const
            {
                return !operator==(other);
            }

            // TODO: This function will need to be tested.
            bool operator=(const intrusive_dense_list_iterator& other) const {

                return current_node == other;
            }

            reference operator*() const
            {   
                return static_cast<reference>(*current_node);
            }

            // Indicing Support
            const_reference operator[](uint16_t index) {


                // Update the backend 
                if (intrusive_dense_list_iterator<T>::data.empty()) {

                    intrusive_dense_list_iterator<T>::data.swap(intrusive_dense_list<T>::data);
                    intrusive_dense_list_iterator<T>::data.at(static_cast<size_t>(index)) = std::make_shared<node_type>(*current_node);

                }

                // TODO: Executing test_dense_list causes a segfault here, but when executing ./mem_check, there is no segfaults what so ever

                return static_cast<reference>(*current_node);
            } 
            
            /**
             * Erases a node from this list.
             * @param node A pointer to the node to erase from this list.
             */
            iterator erase(node_type old_node)
            {
                return erase(iterator(old_node));
            }

            /** ----------------------------------
             * @brief
             * 
             * 
            */
            reference AsNodePointer() const
            {
                return *current_node;
            }
        
        private:
            // The backend data that is synced with the intrusive_dense_list backend data
            inline static std::vector<std::shared_ptr<const intrusive_dense_list_node<T>>> data;

            // Node that holds a raw pointer that is managed by a shared_ptr, and it must stay raw
            node_type* current_node = nullptr;

            // A cached node that can be used to update the current node 
            inline static std::shared_ptr<const node_type> cached_node;
    };

    //template <typename T>
    /*class ts_iterator {
        node_type* current;
        mutable std::mutex mtx;

        public:
            ts_iterator(node_type* node) : current(node) {}

            // Thread-safe ++
            ts_iterator& operator++() {
                std::lock_guard<std::mutex> lock(mtx);
                if (current) current = current->next;
                return *this;
            }

            // Thread-safe dereference
            T& operator*() const {
                std::lock_guard<std::mutex> lock(mtx);
                return *current;
            }

            // ... other iterator methods ...
    };*/

    template<typename T>
    class intrusive_dense_list {

        friend class intrusive_dense_list_iterator<T>;

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
             * @brief Function that inserts a node at a the begining of the vector
             *
             * @param location The location to insert the node.
             * @param new_node The node to add.
            */
            iterator insert_node(iterator location, reference new_node) { return insert_node_before(location, new_node); }

            /** ---------------------------------------------
             * @brief A function that has direct access to the vector and will insert the node at a given position
             *        Assuming that it is not out of bounds
             * 
             * @param idx Place to put the node at in the vector
             * @param node The node to be placed at 
             * 
             * @return nothing
             * 
            */
            void insert(uint16_t idx, reference node) { 
                
                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                data.insert(data.begin() + idx, &node); 
            }

            inline static iterator at(uint16_t idx) {

                static std::shared_ptr<reference> node = std::make_shared<reference>(*data.at(static_cast<size_type>(idx)));
                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                
                return iterator(*node);
            }

            /** ------------------------------------------------------------------
             * @brief Inserts a node at the given location, moving the previous
             * node occupant ahead of the one inserted.
             *
             * @param location The location to insert the new node.
             * @param new_node The node to insert into the list.
            */
            iterator insert_node_before(iterator location, reference new_node) {
                
                auto existing_node = location.AsNodePointer();
                
                new_node.next = &existing_node;
                new_node.prev = existing_node.prev;
                existing_node.prev->next = &new_node;
                existing_node.prev = &new_node;

                root = std::make_shared<reference>(new_node);

                // TODO: Make a wrapper around this function that executes heavy functions
                broken_linkage();

                return iterator(*root);
            }

            /** ----------------------------------------------------------------------------
             * @brief Inserts a new node into the list ahead of the position indicated.
             *
             * @param position Location to insert the node in front of.
             * @param new_node The node to be inserted into the list.
            */
            iterator insert_node_after(iterator position, reference new_node)
            {
                if (node_empty())
                    return insert(node_begin(), new_node);

                return insert_node(++position, new_node);
            }

            /** --------------------------------------------------
             * @brief Add an entry to the start of the linked list.
             *        If vector is empty, then we add the node to it.
             * 
             * @param node Node to add to the list.
             * 
             * @return nothing
            */
            void push_node_front(reference new_node)
            {
                assert(!node_empty());
                insert_node(node_begin(), new_node);
            }

            /** --------------------------------------------------
             * @brief Add an entry to the end of the linked list
             * @param node Node to add to the list.
            */
            void push_back_node(reference node)
            {
                insert_node(node_end(), node);
            }

            /**
             * @brief Function that access the backend and adds a node to the end of it
             * 
             * @param node the node to be added to the backend
             * 
             * @return nothing
            */
            void push_back(reference node) {

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                root = std::make_shared<reference>(node);
                data.push_back(root);

            }

            /**
             * Erases the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            void pop_node_front()
            {
                assert(!node_empty());
                erase(node_begin());
            }

            /**
             * Erases the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            void pop_node_back()
            {
                assert(!node_empty());
                node_erase(--node_end());
            }

            /** ---------------------------------------------------------------------------
             * Removes a node from this list
             * @param it A constant iterator that points to the node to remove from list.
             */
            reference remove_node(const iterator& it)
            {
                iterator copy = it;
                return remove_node(copy);
            }

            /** ---------------------------------------------------------------------------
             * Removes a node from this list.
             * @param node A pointer to the node to remove.
            */
            reference remove_node(reference node)
            {
                return remove_node(iterator(node));
            }

            /** ----------------------------------------------------
             * @brief Is this list empty?
             * @returns true if there are no nodes in this list.
            */
            bool node_empty() const { 

                assert(root.get() != nullptr);
                return root->next == root.get(); 
            }           
            bool empty() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                return data.empty(); 
            }

            /** -------------------------------------------------------------
             * @brief Gets the total number of elements within this list.
             * @return the number of elements in this list.
            */
            size_type node_size() const { return static_cast<size_type>(std::distance(node_begin(), node_end())); }

            size_type size() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                return static_cast<size_type>(data.size()); 
            }

            /**
             * Retrieves a reference to the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            reference node_front()
            {
                assert(!node_empty());
                return node_begin();
            }

            /** ------------------------------------------------
             * @brief A function that returns the first element from the backend 
             * 
             * 
             * @return intrusive_dense_node_list<T>
            */
            reference front() {   

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(!empty());
                return *data.begin().get();
            }

            /**
             * Retrieves a constant reference to the node at the front of the list.
             * @note Must not be called on an empty list.
             */
            const_reference node_front() const
            {
                assert(!node_empty());
                return node_begin();
            }

            /**
             * Retrieves a reference to the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            reference node_back()
            {
                static_assert(!node_empty());
                return *--node_end();
            }

            /**
             * Retrieves a constant reference to the node at the back of the list.
             * @note Must not be called on an empty list.
             */
            const_reference node_back() const
            {
                assert(!node_empty());
                return *--node_end();
            }

            /** -----------------------------------------
             * @brief A function that provides direct access to the backend and will get the last element from it 
             * 
             * 
             * @return Returns intrusive_dense_list_node<T> from the backend
            */
            reference back() {
                
                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                auto node = *data.back();
                assert(node.get() != nullptr);
                return *node;
            }
            
            // ===== ITERATOR INTERFACE =====
            /** ------------------------------
             * @brief A function that will get the begining of the node from the backend at 0 or get the begining of a node at a certain place in the backend 
             * 
             * @param idx is a default parameter. If you wish 
             * 
             * @return Copy of intrusive_dense_list_iterator<T> from the backend
            */
            iterator node_begin(const uint16_t idx = -1) { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                //if (&idx) root = data.at(idx);
                if (root.get() == nullptr) {
                    std::cout << "\n===== NODE_BEGIN() ======\n";
                    std::cout << "root is nullptr. Updating it with the front element of the backend!\n";
                    //auto node = std::make_shared<reference>(*data.begin());
                    exit(0);
                    //root.swap(*node);

                }

                if (root->next) return iterator(*root->next);
                return iterator(*root);
            }
            const_iterator node_begin(uint16_t idx = -1) const { return const_iterator(*root->next);  }
            const_iterator node_cbegin(uint16_t idx = -1) const { return node_begin(); }
            iterator node_end(uint16_t idx = -1) { return iterator(*root.get()); }
            const_iterator node_end(uint16_t idx = -1) const { return const_iterator(*root.get()); }
            const_iterator node_cend(uint16_t idx = -1) const { return node_end(); }
            reverse_iterator node_rbegin(uint16_t idx = -1) { return reverse_iterator(node_end()); }
            const_reverse_iterator node_rbegin(uint16_t idx = -1) const { return const_reverse_iterator(node_end()); }
            const_reverse_iterator node_crbegin(uint16_t idx = -1) const { return node_rbegin(); }
            reverse_iterator node_rend(uint16_t idx = -1) { return reverse_iterator(node_begin()); }
            const_reverse_iterator node_rend(uint16_t idx = -1) const { return const_reverse_iterator(node_begin()); }
            const_reverse_iterator node_crend(uint16_t idx = -1) const { return node_rend(); }
            // ===============================

            /**
             * Erases a node from the list, indicated by an iterator.
             * @param it The iterator that points to the node to erase.
             */
            iterator erase_node(iterator it)
            {
                remove(it);
                return it;
            }

            /**
             * Erases a node from this list.
             * @param node A reference to the node to erase from this list.
             */
            iterator erase_node(reference old_node)
            {
                return erase_node(iterator(old_node));
            }

            /** -------------------------------
             * @brief Function that erases the element from the backend
             * 
             * @param idx the position of the element to be erased at 
            */
            void erase(uint16_t idx) {

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.begin() + idx < data.size());
                data.erase(data.begin() + idx, data.end());
            }

            /**
             * Exchanges contents of this list with another list instance.
             * @param other The other list to swap with.
             */
            void swap_node(intrusive_dense_list& other) noexcept
            {
                data.swap(other.root);
            }

            void swap(intrusive_dense_list& other) noexcept
            {
                data.swap(other.root);
            }

            // ==== BACKEND INTERFACE =====
            reference begin() { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                auto node = std::move(*data.begin());
                assert(node.get() != nullptr);
                return *node; 
            }

            const reference begin() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                auto node = *data.begin();
                assert(node.get() != nullptr);
                return *node; 
            }

            const reference cbegin() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                auto node = *data.cbegin();
                assert(node.get() != nullptr);
                return *node; 
            }

            reference end() { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.end().get() != nullptr);
                return *data.end().get(); 
            }

            const reference end() const { 
                
                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.end().get() != nullptr);
                return *data.end().get(); 
            }

            const reference cend() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.cend().get() != nullptr);
                return *data.cend().get(); 
            }

            reference rbegin() { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.rbegin().get() != nullptr);
                return *data.rbegin().get(); 
            }

            reference rbegin() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                static_assert(data.rbegin().get() != nullptr);
                return *data.rbegin().get(); 
            }

            reference crbegin() const {

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.rbegin().get() != nullptr);
                return *data.rbegin().get(); 
            }

            reference rend() { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                static_assert(data.rend().get() != nullptr);
                return *data.rend().get(); 
            }

            const reference rend() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.rend().get() != nullptr);
                return *data.rend().get(); 
            }

            const reference crend() const { 

                if (intrusive_dense_list<T>::data.size() < intrusive_dense_list_iterator<T>::data.size()) intrusive_dense_list<T>::data.swap(intrusive_dense_list_iterator<T>::data);
                assert(data.crend().get() != nullptr);
                return *data.crend().get(); 
            }
            // ===================================

        protected:

            /** ------------------------------------
             * @brief Function that fixes the linkage for the linked list by operating on the `cached_root`
             *        We cannot use `root` directly as it is used in other operations. 
             * 
             * @note Run time complexity will always be O(n^2) because it is doing a deep fix 
             * 
             * @return Function will always return `nullptr` there is no need for it to return something when it just operates on the private data members
            */
            inline static void* broken_linkage() {

                reference* c_state = root.get();
                reference* p_state = c_state;

                // Check for any breakage here
                if (p_state->prev->next) p_state = p_state->prev;
                else p_state->prev->next = p_state;

                // Make sure all previous nodes are linked
                while (p_state) {

                    if (!p_state->next) p_state->next = c_state;
                    p_state = p_state->prev;
                    c_state = c_state->prev;

                }

                // Shift it to the begining
                // TODO: Shifting won't work. This will need to be debugged
                while(c_state->prev) { c_state = c_state->prev; }

                
                return nullptr;

            };

        private:

            inline static std::shared_ptr<intrusive_dense_list_node<T>> root;            
            inline static std::vector<std::shared_ptr<const intrusive_dense_list_node<T>>> data;
    };

}

#endif
