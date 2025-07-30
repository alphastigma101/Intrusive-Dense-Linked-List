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
    static intrusive_dense_list_node<T>* node = nullptr;

    template<typename T>
    class intrusive_dense_list_node {

        public:

            intrusive_dense_list_node() = default;
            ~intrusive_dense_list_node() noexcept = default;

            intrusive_dense_list_node* next = nullptr;
            intrusive_dense_list_node* prev = nullptr;
            T lvalue;

            bool operator==(std::nullptr_t) const {

                if (&node<T> != nullptr) return true;
                return false;
            }
            
            bool operator!=(std::nullptr_t) const {

                if (!(&node<T> == nullptr)) return true;
                return false;
            }
            
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

            intrusive_dense_list_iterator() = default;
            ~intrusive_dense_list_iterator() = default;
            intrusive_dense_list_iterator(const intrusive_dense_list_iterator& other) = default;
            intrusive_dense_list_iterator& operator=(const intrusive_dense_list_iterator& other) = default;

            // Indicing Support
            intrusive_dense_list_node<T>& operator[](uint32_t index) {
                
                node<T> = this->data.at(static_cast<size_t>(index)).get();
                return *this->data.at(static_cast<size_t>(index));
            }

            const intrusive_dense_list_node<T>& operator[](uint32_t index) const {

                node<T> = this->data.at(static_cast<size_t>(index)).get();
                return *this->data.at(static_cast<size_t>(index));
            }
        
        protected:

            inline static std::vector<std::shared_ptr<intrusive_dense_list_node<T>>> data;
    };

    template<typename T>
    class intrusive_dense_list final : public intrusive_dense_list_node<T>, public intrusive_dense_list_iterator<T> {

        friend class intrusive_dense_list_iterator<T>;
        
        public:

            using value_type = T;
            using size_type = std::size_t;
            using reference = intrusive_dense_list_node<value_type>;
            using const_reference = const value_type&;
            

            intrusive_dense_list() = default;
            ~intrusive_dense_list() noexcept = default;


            /**
             * Inserts a node at the given location indicated by an iterator.
             *
             * @param location The location to insert the node.
             * @param new_node The node to add.
            */
            void insert(uint32_t location, reference new_node) {
                
                auto temp = std::make_shared<reference>(new_node);
                this->data.insert(this->data.begin() + location, std::move(temp));
            }

            /**
             * Add an entry to the start of the list.
             * @param node Node to add to the list.
            */
            void push_front(reference node) {

                auto temp = std::make_shared<reference>(node);
                if (this->data.empty()) this->data.push_back(std::move(temp));
                else this->data.insert(this->data.begin(), std::move(temp));
            }

            void push_back(reference node) {

                auto temp = std::make_shared<reference>(node);
                this->data.push_back(std::move(temp));
            }

            /**
             * Erases the node at the front of the list.
             * @note Must not be called on an empty list.
            */
            void pop_front() {

                if (!this->data.empty()) this->data.erase(this->data.begin() + 0);
                return;
            } 
            
            /**
             * Erases the node at the back of the list.
             * @note Must not be called on an empty list.
            */
            void pop_back() {

                if (!(this->data.empty())) this->data.erase(this->data.begin(), this->data.end() - 1);
                return;
            }

            /**
             * Is this list empty?
             * @returns true if there are no nodes in this list.
            */
            bool empty() const 
            {

                return this->data.empty();
            }

            /**
             * Gets the total number of elements within this list.
             * @return the number of elements in this list.
            */
            size_type size() const 
            {

                return this->data.size();
            }

            /**
             * Retrieves a reference to the node at the front of the list.
             * @note Must not be called on an empty list.
            */
            reference front() {

                if (!this->data.empty()) return *(this->data.front());
                throw("Nothing has been added to the linked list!\n");
            }

            /**
             * Retrieves a reference to the node at the back of the list.
             * @note Must not be called on an empty list.
            */
            reference back() {

                if (!this->data.empty()) return *(this->data.back());
                throw("Nothing has been added to the linked list!\n");
            }

            /**
             * Erases a node from the list, indicated by an iterator.
             * @param it The iterator that points to the node to erase.
            */
            void erase(uint32_t idx) {

                if (!this->data.empty()) this->data.erase(this->data.begin() + idx, this->data.end());
                return;
            }

            /**
             * Exchanges contents of this list with another list instance.
             * @param other The other list to swap with.
            */
            void swap(intrusive_dense_list& other) noexcept 
            {
                this->data.swap(other.data);
            }
    };

}

#endif
