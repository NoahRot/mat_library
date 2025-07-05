#pragma once

#include <inttypes.h>
#include <array>

#include "Type.hpp"

namespace mat {

template<typename T, uint32_t SIZE>
class MatObject {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

public:
    MatObject() {
        if (get_type<T>() == type_content::mat_unvalid_type) {
            throw std::runtime_error("MAT_ERROR : Try to create a MatObject with unvalid type");
        }
    }

    virtual type_mat get_type_object() const { return type_mat::mat_unvalid_mat_type; }
    type_content get_type_content() const { return get_type<T>(); }
    uint32_t get_array_size() const { return SIZE; }

    const std::array<T,SIZE>& data() const { return m_component; }
    std::array<T,SIZE>& data() { return m_component; }

    // === Iterators methods ===

    /// @brief Get the begin iterator
    /// @return The begin iterator
    iterator begin();

    /// @brief Get the constant begin iterator
    /// @return The constant begin iterator
    const_iterator begin() const;

    /// @brief Get the end iterator
    /// @return The end iterator
    iterator end();

    /// @brief Get the constant end iterator
    /// @return The constant end iterator
    const_iterator end() const;

protected:
    std::array<T, SIZE> m_component;
};

template<typename T, uint32_t SIZE>
T* MatObject<T,SIZE>::begin() {
    return &m_component[0];
}

template<typename T, uint32_t SIZE>
const T* MatObject<T,SIZE>::begin() const {
    return &m_component[0];
}

template<typename T, uint32_t SIZE>
T* MatObject<T,SIZE>::end() {
    return &m_component[0] + SIZE;
}

template<typename T, uint32_t SIZE>
const T* MatObject<T,SIZE>::end() const {
    return &m_component[0] + SIZE;
}

}