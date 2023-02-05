#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    using mypair = std::pair<KeyType, ValueType>;
    using const_mypair = std::pair<const KeyType, ValueType>;

    class Node {
    public:
        Node() = default;

        explicit Node(const mypair& pair) : pair_(pair), exist_(true) {}

        void delValue() {
            exist_ = false;
            dist_ = 0;
        }

        mypair& getValue(){
            return pair_;
        }

        const mypair& getValue() const{
            return pair_;
        }

        size_t &getDist() {
            return dist_;
        }

        bool isExist() const {
            return exist_;
        }

    protected:
        mypair pair_;
        bool exist_ = false;
        size_t dist_ = 0;
    };

    using iter = typename std::vector<Node>::iterator;
    using const_iter = typename std::vector<Node>::const_iterator;

    class iterator {
    public:
        iterator() = default;

        iterator(iter it, iter begin, iter end) {
            iterator_ = it;
            begin_ = begin;
            end_ = end;
        }

        const_mypair& operator*() const {
            return *(operator->());
        }

        const_mypair* operator->() const {
            return reinterpret_cast<std::pair<const KeyType, ValueType>*>(&iterator_->getValue());
        }

        iterator &operator=(const iterator& other) {
            iterator_ = other.iterator_;
            begin_ = other.begin_;
            end_ = other.end_;
            return *this;
        }

        iterator &operator++() {
            do {
                ++iterator_;
            } while (iterator_ != end_ && !iterator_->isExist());
            return *this;
        }

        iterator operator++(int) {
            iterator new_iterator = *this;
            do {
                ++iterator_;
            } while (iterator_ != end_ && !iterator_->isExist());
            return new_iterator;
        }

        iterator &operator--() {
            do {
                --iterator_;
            } while (iterator_ != begin_ && !iterator_->isExist());
            return *this;
        }

        iterator operator--(int) {
            iterator new_iterator = *this;
            do {
                --iterator_;
            } while (iterator_ != begin_ && !iterator_->isExist());
            return new_iterator;
        }

        bool operator==(const iterator& other) const {
            return other.iterator_ == iterator_;
        }

        bool operator!=(const iterator& other) const {
            return other.iterator_ != iterator_;
        }

        iter& getIter(){
            return iterator_;
        }

        size_t getInd(){
            return iterator_ - begin_;
        }

    protected:
        iter iterator_, begin_, end_;
    };

    class const_iterator {
    public:
        const_iterator() = default;

        const_iterator(const_iter it, const_iter begin, const_iter end) {
            iterator_ = it;
            begin_ = begin;
            end_ = end;
        }

        const const_mypair& operator*() const {
            return *(operator->());
        }

        const const_mypair* operator->() const {
            return reinterpret_cast<const std::pair<const KeyType, ValueType>*>(&iterator_->getValue());
        }

        const_iterator &operator=(const const_iterator& other) {
            iterator_ = other.iterator_;
            begin_ = other.begin_;
            end_ = other.end_;
            return *this;
        }

        const_iterator &operator++() {
            do {
                ++iterator_;
            } while (iterator_ != end_ && !iterator_->isExist());
            return *this;
        }

        const const_iterator operator++(int) {
            const_iterator new_iterator = *this;
            do {
                ++iterator_;
            } while (iterator_ != end_ && !iterator_->isExist());
            return new_iterator;
        }

        const_iterator &operator--() {
            do {
                --iterator_;
            } while (iterator_ != begin_ && !iterator_->isExist());
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator new_iterator = *this;
            do {
                --iterator_;
            } while (iterator_ != begin_ && !iterator_->isExist());
            return new_iterator;
        }

        bool operator==(const const_iterator& other) const {
            return other.iterator_ == iterator_;
        }

        bool operator!=(const const_iterator& other) const {
            return other.iterator_ != iterator_;
        }

    protected:
        const_iter iterator_, begin_, end_;
    };

    explicit HashMap(Hash hasher = Hash()) : hasher_(hasher) {}

    HashMap(const std::initializer_list<mypair> list, Hash hasher = Hash()) : hasher_(hasher) {
        resize(list.size() * 10);
        for (auto &pair: list) insert(pair);
    }

    HashMap(const iterator begin, const iterator end,
            Hash hasher = Hash()) : hasher_(hasher) {
        for (auto it = begin; it != end; ++it) {
            insert(*it);
        }
    }

    size_t mod(size_t x) const{
        if (x >= capacity_) return x - capacity_;
        return x;
    }

    size_t size() const {
        return n_;
    }

    bool empty() const {
        return !n_;
    }

    Hash hash_function() const {
        return hasher_;
    }

    bool needRebuild() {
        return n_ * 10 > capacity_;
    }

    void insert(const mypair pair) {
        if (find(pair.first) != end()) {
            return;
        }
        size_t ind = hasher_(pair.first) % capacity_;
        Node new_node = Node(pair);
        while (new_node.isExist()) {
            if (new_node.getDist() >= table_[ind].getDist()) {
                std::swap(new_node, table_[ind]);
            }
            ++new_node.getDist();
            ind = mod(ind + 1);
        }
        ++n_;
        if (needRebuild()) {
            rebuild();
        }
    }

    void erase(const KeyType key) {
        iterator it = find(key);
        if (it == end()){
            return;
        }
        size_t ind = it.getInd();
        table_[ind].delValue();
        while (table_[mod(ind + 1)].isExist() and table_[mod(ind + 1)].getDist()){
            std::swap(table_[ind], table_[mod(ind + 1)]);
            --table_[ind].getDist();
            ind = mod(ind + 1);
        }
        --n_;
    }

    void rebuild() {
        capacity_ *= 10;
        std::vector<Node> new_table = std::vector<Node>(capacity_);
        new_table.swap(table_);
        n_ = 0;
        for (auto &node: new_table) {
            if (node.isExist()) {
                insert(node.getValue());
            }
        }
    }

    iterator find(const KeyType key){
        size_t ind = hasher_(key) % capacity_;
        while (table_[ind].isExist()) {
            if (table_[ind].getValue().first == key) {
                return iterator(table_.begin() + ind, table_.begin(), table_.end());
            }
            ind = mod(ind + 1);
        }
        return end();
    }

    const_iterator find(const KeyType key) const {
        size_t ind = hasher_(key) % capacity_;
        while (table_[ind].isExist()) {
            if (table_[ind].getValue().first == key) {
                return const_iterator(table_.begin() + ind, table_.begin(), table_.end());
            }
            ind = mod(ind + 1);
        }
        return end();
    }

    ValueType &operator[](const KeyType key) {
        insert({key, ValueType()});
        return find(key)->second;
    }

    const ValueType& at(const KeyType key) const{
        const_iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("out_of_range");
        }
        return it->second;
    }

    void clear() {
        table_ = std::vector<Node>(capacity_);
        n_ = 0;
    }

    void resize(size_t size){
        table_ = std::vector<Node>(size);
        capacity_ = size;
    }

    iterator begin() {
        if (!n_) {
            return iterator(table_.end(), table_.begin(), table_.end());
        }

        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].isExist()) {
                return iterator(table_.begin() + i, table_.begin(), table_.end());
            }
        }
        return iterator(table_.end(), table_.begin(), table_.end());
    }

    iterator end() {
        return iterator(table_.end(), table_.begin(), table_.end());
    }

    const_iterator begin() const{
        if (!n_) {
            return const_iterator(table_.end(), table_.begin(), table_.end());
        }

        for (size_t i = 0; i < capacity_; ++i) {
            if (table_[i].isExist()) {
                return const_iterator(table_.begin() + i, table_.begin(), table_.end());
            }
        }
        return const_iterator(table_.end(), table_.begin(), table_.end());
    }

    const_iterator end() const{
        return const_iterator(table_.end(), table_.begin(), table_.end());
    }

protected:
    size_t capacity_ = 10;
    size_t n_ = 0;
    Hash hasher_;

    std::vector<Node> table_ = std::vector<Node>(capacity_);
};