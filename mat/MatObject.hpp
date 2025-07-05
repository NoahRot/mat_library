#pragma once

#include <inttypes.h>
#include <array>

#include "Type.hpp"

namespace mat {

//     ___ _            
//    / __| |__ _ ______
//   | (__| / _` (_-<_-<
//    \___|_\__,_/__/__/
//  

/// @brief The base class for all object in the MAT library that need multiple components
/// @tparam T Type of the content
/// @tparam SIZE Number of components in the object
template<typename T, uint32_t SIZE>
class MatObject {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

public:

//     ___             _               _           
//    / __|___ _ _  __| |_ _ _ _  _ __| |_ ___ _ _ 
//   | (__/ _ \ ' \(_-<  _| '_| || / _|  _/ _ \ '_|
//    \___\___/_||_/__/\__|_|  \_,_\__|\__\___/_|  
//

    /// @brief Constructor
    /// @warning Throw a runtime error if the content type is unvalid
    MatObject();

//    __  __     _   _            _    
//   |  \/  |___| |_| |_  ___  __| |___
//   | |\/| / -_)  _| ' \/ _ \/ _` (_-<
//   |_|  |_\___|\__|_||_\___/\__,_/__/
//  

    /// @brief Get the type of the MatObject
    /// @return Type of the MatObject
    virtual type_mat get_type_object() const { return type_mat::mat_unvalid_mat_type; }

    /// @brief Get the type of the content of the Type of the MatObject
    /// @return Type of the content in the MatObject
    type_content get_type_content() const { return get_type<T>(); }

    /// @brief Get the size of the array containing the component of the MatObject
    /// @return Number of component in the MatObject
    uint32_t get_array_size() const { return SIZE; }

    /// @brief Get a constant reference to the array of the MatObject
    /// @return A constant reference to the array
    const std::array<T,SIZE>& data() const { return m_component; }

    /// @brief Get a reference to the array of the MatObject
    /// @return A reference to the array
    std::array<T,SIZE>& data() { return m_component; }

    /// @brief Copy data into the MatObject
    /// @param start A pointer to the first element that will be copy
    void copy(T* start);

//     ___                     _              
//    / _ \ _ __  ___ _ _ __ _| |_ ___ _ _ ___
//   | (_) | '_ \/ -_) '_/ _` |  _/ _ \ '_(_-<
//    \___/| .__/\___|_| \__,_|\__\___/_| /__/
//         |_|                                

    // === Comparison operators ===

    /// @brief Is equal operator (true if all components are equal)
    /// @param v The MatObject that will be compared
    /// @return If the MatObjects are equal
    bool operator==(const MatObject<T,SIZE>& m) const;

    /// @brief Is different operator (true if at least one component is different)
    /// @param v The MatObject that will be compared
    /// @return If the MatObjects are different
    bool operator!=(const MatObject<T,SIZE>& v) const;

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
    /// @brief Component of the object
    std::array<T, SIZE> m_component;
};

// ===================================================================================================
// ===================================================================================================


//    ___       __ _      _ _   _             
//   |   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
//   | |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
//   |___/\___|_| |_|_||_|_|\__|_\___/_||_/__/
//                                            


template<typename T, uint32_t SIZE>
MatObject<T,SIZE>::MatObject() {
    if (get_type<T>() == type_content::mat_unvalid_type) {
        throw std::runtime_error("MAT_ERROR : Try to create a MatObject with unvalid type");
    }
}

template<typename T, uint32_t SIZE>
void MatObject<T,SIZE>::copy(T* start) {
    std::copy(start, start+SIZE, m_component.begin());
}

template<typename T, uint32_t SIZE>
bool MatObject<T,SIZE>::operator==(const MatObject<T,SIZE>& m) const {
    const_iterator it = m.begin();
    for (auto i : m_component) {
        if (i != *it) {
            return false;
        }
        ++it;
    }
    return true;
}

template<typename T, uint32_t SIZE>
bool MatObject<T,SIZE>::operator!=(const MatObject<T,SIZE>& v) const {
    return !(*this == v);
}

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