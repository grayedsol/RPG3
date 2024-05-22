/**
 * @file SparseSet.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief SparseSet
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <vector>
#include <assert.h>
#include "GRY_Log.hpp"

/**
 * @brief Class template data structure used for storing component data.
 * 
 * @details
 * The structure is limited to a size of `SIZE`.
 * 
 * Conceptually, there is a "NONE" entity, which is represented
 * by an entity with the value `SIZE`.
 * 
 * @tparam T Type of the component data.
 * @tparam entity Type of the identifier to use. Must be unsigned integral type.
 * @tparam SIZE Maximum number of elements to contain.
 */
template <typename T, typename entity, entity SIZE>
struct SparseSet {
    /**
     * @brief Sparse array in the sparse set.
     * 
     * @details
     * The size is `SIZE + 1` to accomodate the "NONE" entity.
     */
    entity sparse[SIZE+1]{};

    /**
     * @brief Dense array in the sparse set.
     * 
     */
    std::vector<entity> dense;

    /**
     * @brief Dense array in the sparse set that contains component data.
     * 
     */
    std::vector<T> value;

    /**
     * @brief Constructor.
     * 
     * @details
     * All elements in the sparse array are initialized to `SIZE`.
     */
    SparseSet() {
        for (int i = 0; i <= SIZE; i++) {
            this->sparse[i] = SIZE;
        }
    };

    SparseSet(const SparseSet&) = delete;
    SparseSet& operator=(const SparseSet&) = delete;

    friend void swap(SparseSet& lhs, SparseSet& rhs) {
        using std::swap;
        swap<entity, SIZE+1>(lhs.sparse, rhs.sparse);
        swap(lhs.dense, rhs.dense);
        swap(lhs.value, rhs.value);
    }

    SparseSet(SparseSet&& other) noexcept { swap(*this, other); }

    /**
     * @brief Check if `e` has data in this SparseSet.
     * 
     * @param e Entity to check.
     * @return `true` if there is component data for the entity.
     * @return `false` otherwise.
     */
    const bool contains(entity e) const { return sparse[e] != SIZE; }

    /**
     * @brief Number of entities with data in this SparseSet.
     * 
     * @return The size.
     */
    const size_t size() const { return dense.size(); }

    /**
     * @brief Add `data` and associate it with `e`.
     * 
     * @details
     * Logs an error if `e` already has data in the SparseSet, or if
     * `e` is `ECS::NONE`.
     * 
     * @param e Entity to add.
     * @param data Data to add.
     */
    void add(entity e, T data) {
        if (e == SIZE) {
            GRY_Log("[SparseSet] Tried to add the NONE entity.\n");
        }
        else if (!contains(e)) {
            sparse[e] = (uint8_t)dense.size();
            dense.push_back(e);
            value.push_back(data);
        }
        else {
            GRY_Log("[SparseSet] Tried to add an entity that already existed.\n");
        }
    }

    /**
     * @brief Remove data that is associated with `e`.
     * 
     * @details
     * This is slower than adding an entity, due to the way the data
     * has to be rearranged to preserve the underlying sparse set structure.
     * 
     * Logs an error if `e` does not have any associated data.
     * 
     * @param e Entity for which data is to be removed.
     */
    void remove(entity e) {
        if (contains(e)) {
            const auto last = dense.back();
            std::swap(dense.back(), dense[sparse[e]]);
            std::swap(value.back(), value[sparse[e]]);
            std::swap(sparse[last], sparse[e]);
            dense.pop_back();
            value.pop_back();
            sparse[e] = SIZE;
        }
        else {
            GRY_Log("[SparseSet] Tried to remove an entity that didn't exist.\n");
        }
    }

    /**
     * @brief Remove all component data.
     * 
     * @details
     * Effectively resets the SparseSet by calling `remove()`
     * with any entities with data.
     * 
     * @sa remove
     */
    void removeAll() {
        std::vector<entity> entities;
        for (int i = 0; i < dense.size(); i++) { entities.push_back(getEntity(i)); }
        for (entity e : entities) { remove(e); }
    }

    /**
     * @brief Get the data associated with `e`.
     * 
     * @details
     * If there is no data associated with `e`, logs an error and asserts
     * that there is at least one instance of stored component data. 
     * Then, returns the first instance of stored component data.
     * 
     * @param e Entity to get data for.
     * @return Reference to the data.
     * 
     * @sa get(entity) const
     */
    T& get(entity e) {
        if (contains(e)) { return value[sparse[e]]; }
        else {
            GRY_Log("[SparseSet] Tried to access data for an entity that didn't exist. (%d)\n", e);
            assert(!value.empty());
            return value[0];
        }
    }

    /**
     * @brief @copybrief get
     * 
     * @details
     * @copydetails get
     * 
     * @param e Entity to get data for.
     * @return `const` reference to the data.
     * 
     * @sa get
     */
    const T& get(entity e) const {
        if (contains(e)) { return value[sparse[e]]; }
        else {
            GRY_Log("[SparseSet] Tried to access data for an entity that didn't exist. (%d)\n", e);
            assert(!value.empty());
            return value[0];
        }
    }

    /**
     * @brief Get an entity. Useful for iterating through entity data.
     * 
     * @details
     * Does not retrieve any specific entity, as the order
     * in which the data is stored is not guaranteed.
     * 
     * Throws an out_of_range exception from std::vector if `i` is
     * greater than or equal to `size()`.
     * 
     * @param i Index of an entity to retrieve. Must be less than `size()`.
     * @return An entity with data in the SparseSet.
     */
    const entity getEntity(uint8_t i) const { return dense.at(i); }

    /**
     * @brief Pointer to the first entity. Useful in range based for loops.
     * 
     * @return Pointer to the first entity in the SparseSet. 
     */
    entity* begin() { return dense.data(); }

    /**
     * @brief Pointer to one past the last entity. Useful in range based for loops.
     * 
     * @return Pointer to one past the last entity in the SparseSet. 
     */
    entity* end() { return dense.data() + dense.size(); }

};