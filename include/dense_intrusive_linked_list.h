
#ifndef __DENSE_INTRUSIVE_LINKED_LIST__
#define __DENSE_INTRUSIVE_LINKED_LIST__

// This file is part of the mcl project.
// Copyright (c) 2022 merryhime
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <memory>
#include <atomic>
#include <future>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <type_traits>
#include <thread>
#include <chrono>
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

    /*template<typename T>
    class threading_iterator : public std::thread {
        private:
            static std::vector<std::unique_ptr<threading_iterator<T>>> thread_pool_;
            static std::mutex pool_mutex_;
            static std::atomic<size_t> next_id_;
            
            size_t thread_id_;
            std::atomic<bool> finished_;
            std::promise<void> completion_promise_;
            std::future<void> completion_future_;

        public:
            // Constructor that captures a function to execute in thread
            template<typename Func, typename... Args>
            threading_iterator(Func&& func, Args&&... args) 
                : std::thread([this, func = std::forward<Func>(func), args...]() mutable {
                    try {
                        func(args...);
                    } catch (...) {
                        // Handle exceptions in thread
                    }
                    finished_.store(true);
                    completion_promise_.set_value();
                })
                , thread_id_(next_id_.fetch_add(1))
                , finished_(false)
                , completion_future_(completion_promise_.get_future()) {
                
                // Add this thread to the pool
                std::lock_guard<std::mutex> lock(pool_mutex_);
                thread_pool_.push_back(std::make_unique<threading_iterator<T>>(std::move(*this)));
            }

            // Constructor that captures operators for threading operations
            template<typename Iterator, typename BinaryOp>
            threading_iterator(Iterator begin, Iterator end, BinaryOp op) 
                : std::thread([this, begin, end, op]() mutable {
                    try {
                        // Perform threaded operation on range
                        std::for_each(begin, end, op);
                    } catch (...) {
                        // Handle exceptions
                    }
                    finished_.store(true);
                    completion_promise_.set_value();
                })
                , thread_id_(next_id_.fetch_add(1))
                , finished_(false)
                , completion_future_(completion_promise_.get_future()) {
                
                std::lock_guard<std::mutex> lock(pool_mutex_);
                thread_pool_.push_back(std::make_unique<threading_iterator<T>>(std::move(*this)));
            }

            // Destructor - ensures thread is properly joined
            ~threading_iterator() noexcept {
                if (joinable()) {
                    join();
                }
            }

            // Start method that creates another thread and manages PIDs
            static void start() {
                std::thread manager_thread([]() {
                    while (true) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        
                        std::lock_guard<std::mutex> lock(pool_mutex_);
                        
                        // Check for finished threads and clean them up
                        thread_pool_.erase(
                            std::remove_if(thread_pool_.begin(), thread_pool_.end(),
                                [](const std::unique_ptr<threading_iterator<T>>& thread_ptr) {
                                    if (thread_ptr && thread_ptr->finished_.load()) {
                                        if (thread_ptr->joinable()) {
                                            thread_ptr->join();
                                        }
                                        return true;
                                    }
                                    return false;
                                }),
                            thread_pool_.end()
                        );
                        
                        // If no threads are running, break
                        if (thread_pool_.empty()) {
                            break;
                        }
                    }
                });
                
                manager_thread.detach();
            }

            // Get thread ID
            size_t get_thread_id() const { return thread_id_; }
            
            // Check if thread is finished
            bool is_finished() const { return finished_.load(); }
            
            // Wait for completion
            void wait_for_completion() {
                completion_future_.wait();
            }
            
            // Wait for completion with timeout
            template<typename Rep, typename Period>
            std::future_status wait_for_completion_for(const std::chrono::duration<Rep, Period>& timeout_duration) {
                return completion_future_.wait_for(timeout_duration);
            }

            // Kill all threads and cleanup
            static void kill_all_processes() {
                std::lock_guard<std::mutex> lock(pool_mutex_);
                
                for (auto& thread_ptr : thread_pool_) {
                    if (thread_ptr && thread_ptr->joinable()) {
                        // For forceful termination, you might need platform-specific code
                        // This is a graceful approach
                        thread_ptr->join();
                    }
                }
                
                thread_pool_.clear();
            }

            // Get active thread count
            static size_t active_thread_count() {
                std::lock_guard<std::mutex> lock(pool_mutex_);
                return thread_pool_.size();
            }
    };

    template<typename T>
    class decorator_wrapper {
        private:
            std::function<void()> before_operation_;
            std::function<void()> after_operation_;
            std::function<void(const std::exception&)> exception_handler_;
            bool logging_enabled_;
            std::string operation_name_;

            template<typename U>
            friend class intrusive_dense_list<U>;

        public:
            // Default constructor
            decorator_wrapper() 
                : logging_enabled_(false) {}

            // Constructor with operation name
            explicit decorator_wrapper(const std::string& name) 
                : logging_enabled_(false), operation_name_(name) {}

            // Destructor
            ~decorator_wrapper() noexcept = default;

            // Set before operation callback
            void set_before_operation(std::function<void()> before) {
                before_operation_ = std::move(before);
            }

            // Set after operation callback  
            void set_after_operation(std::function<void()> after) {
                after_operation_ = std::move(after);
            }

            // Set exception handler
            void set_exception_handler(std::function<void(const std::exception&)> handler) {
                exception_handler_ = std::move(handler);
            }

            // Enable/disable logging
            void enable_logging(bool enable = true) {
                logging_enabled_ = enable;
            }

            // Set operation name for logging
            void set_operation_name(const std::string& name) {
                operation_name_ = name;
            }

            // Main decoration method - wraps any callable
            template<typename Func, typename... Args>
            auto decorate(Func&& func, Args&&... args) -> decltype(func(args...)) {
                // Before operation
                if (before_operation_) {
                    before_operation_();
                }

                if (logging_enabled_) {
                    std::cout << "[DECORATOR] Starting operation: " << operation_name_ << std::endl;
                }

                try {
                    // Execute the actual operation
                    if constexpr (std::is_void_v<decltype(func(args...))>) {
                        func(std::forward<Args>(args)...);
                        
                        // After operation (void case)
                        if (after_operation_) {
                            after_operation_();
                        }
                        
                        if (logging_enabled_) {
                            std::cout << "[DECORATOR] Completed operation: " << operation_name_ << std::endl;
                        }
                    } else {
                        auto result = func(std::forward<Args>(args)...);
                        
                        // After operation (non-void case)
                        if (after_operation_) {
                            after_operation_();
                        }
                        
                        if (logging_enabled_) {
                            std::cout << "[DECORATOR] Completed operation: " << operation_name_ << std::endl;
                        }
                        
                        return result;
                    }
                } catch (const std::exception& e) {
                    if (exception_handler_) {
                        exception_handler_(e);
                    } else if (logging_enabled_) {
                        std::cout << "[DECORATOR] Exception in operation " << operation_name_ << ": " << e.what() << std::endl;
                    }
                    throw; // Re-throw the exception
                }
            }

            // Convenience method for decorating member functions
            template<typename Object, typename MemberFunc, typename... Args>
            auto decorate_member(Object& obj, MemberFunc&& func, Args&&... args) -> decltype((obj.*func)(args...)) {
                return decorate([&obj, &func](Args&&... args) -> decltype((obj.*func)(args...)) {
                    return (obj.*func)(std::forward<Args>(args)...);
                }, std::forward<Args>(args)...);
            }

            // Static factory methods for common decorators
            static decorator_wrapper<T> create_logging_decorator(const std::string& operation_name) {
                decorator_wrapper<T> decorator(operation_name);
                decorator.enable_logging(true);
                return decorator;
            }

            static decorator_wrapper<T> create_sync_decorator(std::function<void()> sync_func) {
                decorator_wrapper<T> decorator("sync_operation");
                decorator.set_after_operation(std::move(sync_func));
                return decorator;
            }

            static decorator_wrapper<T> create_timing_decorator(const std::string& operation_name) {
                decorator_wrapper<T> decorator(operation_name);
                decorator.enable_logging(true);
                
                auto start_time = std::make_shared<std::chrono::high_resolution_clock::time_point>();
                
                decorator.set_before_operation([start_time, operation_name]() {
                    *start_time = std::chrono::high_resolution_clock::now();
                    std::cout << "[TIMER] Starting " << operation_name << std::endl;
                });
                
                decorator.set_after_operation([start_time, operation_name]() {
                    auto end_time = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - *start_time);
                    std::cout << "[TIMER] " << operation_name << " took " << duration.count() << " microseconds" << std::endl;
                });
                
                return decorator;
            }

            // Method to chain multiple decorators
            template<typename Func, typename... Args>
            auto chain_with(decorator_wrapper<T>& other, Func&& func, Args&&... args) -> decltype(func(args...)) {
                return decorate([&other, &func](Args&&... args) -> decltype(func(args...)) {
                    return other.decorate(std::forward<Func>(func), std::forward<Args>(args)...);
                }, std::forward<Args>(args)...);
            }
    };*/

    template<typename T>
    class intrusive_dense_list_node  {

        friend class intrusive_dense_list<T>;
        friend class intrusive_dense_list_iterator<T>;

        public:

            T lvalue; // This needs to be moved into private

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
            using reference = value_type&;
            using const_reference = const value_type&;

            // If value_type is const, we want "const intrusive_list_node<value_type>", not "intrusive_list_node<const value_type>"
            using node_type = std::conditional_t<std::is_const<value_type>::value,
                                                const intrusive_dense_list_node<std::remove_const_t<value_type>>,
                                                intrusive_dense_list_node<value_type>>;

            intrusive_dense_list_iterator(const intrusive_dense_list_iterator& other) = default;
            intrusive_dense_list_iterator& operator=(const intrusive_dense_list_iterator& other) = default;
            
            explicit intrusive_dense_list_iterator(node_type* node) : current(node) {}

            intrusive_dense_list_iterator& operator++() {
                if (!current && !is_synced) {
                    
                    // nullptr will occur when linked list gets modified i.e new node gets added in
                    current = intrusive_dense_list<T>::root_node.get(); // same as data.size() - 1  
                    is_synced = true;
                }
                if (current) current = current->next.get();
                else return *this;
                return *this;
            }

            intrusive_dense_list_iterator& operator--() {
                if (!current && !is_synced) {

                    current = intrusive_dense_list<T>::root_node.get();
                    is_synced = true;
                }
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

            node_type operator[](uint16_t index) {
                if (!current && !is_synced) {

                    current = intrusive_dense_list<T>::root_node.get();
                    is_synced = true;
                }
                intrusive_dense_list<T>::root_node = intrusive_dense_list<T>::data.at(index);  
                return static_cast<node_type>(*current); 
            } 

            bool operator==(const intrusive_dense_list_iterator& other) const
            {
                return current == other.node;
            }

            bool operator!=(const intrusive_dense_list_iterator& other) const
            {
                return !operator==(other);
            }
            
            reference operator*() const {

                assert(current != nullptr);
                return static_cast<reference>(*current->lvalue);
            }
            
        private:

            // Variable used for syncing the nodes with intrusive_dense_list class 
            bool is_synced = false;

            node_type* current = nullptr;

    };

    template <typename T>
    class intrusive_dense_list {

        friend class intrusive_dense_list_iterator<T>;

        public:

            using size_type = std::size_t;
            using value_type = T;
            using reference = value_type&;
            using const_reference = const value_type&;
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
                sync_to_root();
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
                sync_to_root();
                return const_iterator(*data.begin());
            }

            const_iterator cbegin() const { 

                assert(!data.empty());
                sync_to_root();
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
                sync_to_root();
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
                sync_to_root();
                return const_iterator(*data.end());
            }

            const_iterator cend() const { 

                assert(!data.empty() && data.size() > 1); 
                sync_to_root();
                auto node = *data.cend();
                return const_iterator(node.get()); 
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Gets the element in the backend and reverses the nodes
             * 
             * 
             * @return iterator class but reveresed
             * 
             * -----------------------------------------------------------------------------------------
            */
            reverse_iterator rbegin() { return reverse_iterator(end()); }
        
            /** ---------------------------------------------------------------------------------------
             * @brief Gets the element in the backend the last element but with a cv qualifer and reverses the nodes
             * 
             * @note Reverses the linked list in the element
             * 
             * @return cv qualifer iterator class reversed
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
            
            /** ---------------------------------------------------------------------------------------
             * @brief Gets the backend element but reverses it 
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_reverse_iterator crbegin() const { return rbegin(); }
    
            /** ---------------------------------------------------------------------------------------
             * @brief Gets the backend front element and reverses it 
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            reverse_iterator rend() { return reverse_iterator(begin()); }
            
            /** ---------------------------------------------------------------------------------------
             * @brief Gets the backend element front and reverses it with a cv qualifier 
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
            
            /** ---------------------------------------------------------------------------------------
             * @brief Gets the backend front element and reverses the nodes with a cv qaulifier
             * 
             * @note 
             * 
             * @return 
             * 
             * -----------------------------------------------------------------------------------------
            */
            const_reverse_iterator crend() const { return rend(); }

            /** ---------------------------------------------------------------------------------------
             * @brief Index into a spot to reverse the list making the head the tail and the tail the head.
             * 
             * @param index A spot that will index into the array if valid 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void reverse_ends(uint16_t index) {

                assert(index < data.size());
                
                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));
                
                assert(current_node.get() != nullptr);
                
                auto root = current_node;
                root->prev = current_node->next;
                root->next = current_node->prev;

                data.at(static_cast<size_type>(index)).swap(root);
                
                return;
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Index into a spot to reverse the linked list entirely
             * 
             * @param index A spot that will index into the array if valid 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void reverse_list(uint16_t index) {

                assert(index < data.size());
                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));
                
                assert(current_node.get() != nullptr);
                
                auto head = current_node;
                while (head->prev) { head = head->prev; }
                
                auto tail = current_node;
                while (tail->next) { tail = tail->next; }
                
                // Reverse all the links in the chain
                auto node = head;
                while (node) {

                    auto next_node = node->next;  // Store next before we change it
                    
                    // Swap next and prev pointers
                    std::swap(node->next, node->prev);
                    
                    node = next_node;  // Move to what was originally the next node
                }
                
                auto new_head = tail; // Divide it by two to find the center. This is the new head. 
                // TODO: We need to find the new head by using pointer arthimitic and update the iterator class's current with it divide it by 2.
                // You can use the offsets of the memory address to find the head node
                root_node = new_head;
                data.at(static_cast<size_type>(index)).swap(tail);
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that indexes into the backend at a specific location
             * 
             * @note Method should only be used if existing indexed element is in the backend.
             * 
             * @return Returns the linked list that has a series of operations
             * 
             * -----------------------------------------------------------------------------------------
            */
            iterator at(uint16_t index) {
                
                assert(index < data.size());

                sync_to_root(index);

                auto node = data.at(index);

                return iterator(node.get());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that inserts a node at a specific spot in the backend 
             * 
             * @note Method should only be used if you want to add a node to the backend. 
             *       It can be inserted in the middle, infront or behind the a previous node element.
             * 
             * @return Nothing.
             * 
             * -----------------------------------------------------------------------------------------
            */
            void insert(uint16_t index, T value) {

                std::shared_ptr<node_type> new_node = std::make_shared<node_type>(value);
                if (index < data.size()) data.at(index).swap(new_node);
                else data.insert(data.begin() + index, data.end());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Inserts nodes at behind the root, but in the middle
             * 
             * @note Operates on the desired element in the vector.
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
            
                auto prev_item = current_node->prev;
            
                new_node->prev = prev_item;
            
                new_node->next = current_node;
            
                current_node->prev = new_node;
            
                if (prev_item) prev_item->next = new_node;
            
                data.at(static_cast<size_type>(index)).swap(current_node);
            
                return;
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Inserts nodes at after the root.
             * 
             * @note Operates on the desired element in the vector. 
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

            /** ---------------------------------------------------------------------------------------
             * @brief Method that adds a new node at the end of tail 
             * 
             * @note Method should only be used for adding nodes at the end of the linked list 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void push_back(uint16_t index, T value) {

                assert(index < data.size());
                auto new_node = std::make_shared<node_type>(value); 

                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(static_cast<size_type>(index)));

                auto old_tail = current_node.get(); 

                while (old_tail->next) old_tail = old_tail->next.get();

                old_tail->next = new_node;
                new_node->prev = current_node;
                while (old_tail->lvalue != current_node->lvalue) old_tail = old_tail->prev.get();

                auto new_tail = std::make_shared<node_type>(*old_tail);
                current_node.swap(new_tail);

                data.at(index).swap(current_node);

                return;
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that creates the linked list embedding the next/prev into it 
             * 
             * @note To start adding linked lists into the backend, this function should be used first 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void push_back(T value) {

                auto new_node = std::make_shared<node_type>(value);
                root_node = new_node;
                data.push_back(new_node);

            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that pops the last element in the backend 
             * 
             * @note Method should be used when you want to remove an element and not just the a node.
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void pop_back() {

                assert(!data.empty());
                data.pop_back();
                data.shrink_to_fit();

            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that pops the current tail and creates the new tail based on what was linked to the old tail.
             * 
             * @note Method that actually operates on the nodes in the element from the backend. It can continue popping the nodes from the back if 
             *       default argument is not sentinial.
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void pop_back(uint16_t index = -1) {

                assert(!data.empty());
                std::shared_ptr<node_type> current_node;
                current_node.swap(data.at(index != 65535 && index < data.size() ? index : data.size() - 1).get());
                auto tail = current_node;
                
                while (tail->next) tail = tail->next;
                auto prev = tail->prev;
                tail = nullptr;
                tail->prev = prev;

                while (tail->lvalue != current_node->lvalue) tail = tail->prev;

                data.at(static_cast<size_type>(index)).swap(tail);

            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that removes the front node and preserves the linkage 
             * 
             * @note Use it only when you want to remove nodes from the list 
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void pop_front(uint16_t index = -1) {

                assert(!data.empty());
    
                std::shared_ptr<node_type> current_node; 
                current_node.swap(data.at(index != 65535 && index < data.size() ? index : data.size() - 1).get());
                
                // Store references to adjacent nodes
                auto prev_node = current_node->prev;
                auto next_node = current_node->next;
                
                // Update the previous node's next pointer
                if (prev_node != nullptr) {
                    prev_node->next = next_node;
                }
                
                // Update the next node's prev pointer
                if (next_node != nullptr) {
                    next_node->prev = prev_node;
                }
                
                current_node->next = next_node;

                data.at(static_cast<size_type>(index)).swap(current_node);
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that erases the element in the front 
             * 
             * @note Use it to remove the front element and nothing else.
             * 
             * @return nothing
             * 
             * -----------------------------------------------------------------------------------------
            */
            void pop_front() {

                data.erase(data.begin(), data.end());
                assert(!data.empty());
            }

            /** ---------------------------------------------------------------------------------------
             * @brief Method that erases an element from the backend.
             * 
             * @return Nothing
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

            void* sync_to_root(uint16_t index = -1) {

                bool found_root = false;
                auto current_node = data.at(index != 65535 && index < data.size() ? index : data.size() - 1).get();
                
                while (current_node && !found_root) {
                    if (current_node->lvalue == root_node->lvalue) {
                        found_root = true;
                        break;
                    }

                    current_node = current_node->next.get();
                }
                
                if (!found_root) {
                    auto iter = iterator(data.at(index != 65535 && index < data.size() ? index : data.size() - 1).get());
                    current_node = std::make_shared<node_type>(iter[index != 65535 && index < data.size() ? index : data.size() - 1]).get();
                    do  {
                        if (current_node && current_node->lvalue == root_node->lvalue) {
                            found_root = true;
                            break;
                        }
                        current_node = current_node->prev.get();
                    } while (current_node && current_node->lvalue != root_node->lvalue);
                }

                if (found_root && current_node) {
                    std::shared_ptr<node_type> root_ptr = std::make_shared<node_type>(*current_node);
                    data[index != 65535 && index < data.size() ? index : data.size() - 1].swap(root_ptr);
                    root_node = data[index != 65535 && index < data.size() ? index : data.size() - 1];
                }

                return nullptr;
            }
                  
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

            inline static std::shared_ptr<node_type> root_node;
            inline static std::vector<std::shared_ptr<node_type>> data;
    };
 
};

#endif
