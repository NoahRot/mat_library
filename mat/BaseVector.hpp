#pragma once

#include <array>
#include <inttypes.h>

#include "MatObject.hpp"
#include "Vector.hpp"
#include "Complex.hpp"
#include "Quaternion.hpp"
#include "Type.hpp"

namespace mat {

template<typename T, uint32_t N>
class Vector;

template<typename T>
class Complex;

template<typename T>
class Quaternion;

//     ___ _            
//    / __| |__ _ ______
//   | (__| / _` (_-<_-<
//    \___|_\__,_/__/__/
//                      

/// @brief Class that represent a vector without any operations
/// @tparam T Type of vector
/// @tparam N Dimension of the vector
template<typename T, uint32_t N>
class BaseVector : public MatObject<T,N> {
public:
    // === Mat Object using ===

    using typename MatObject<T, N>::iterator; 
    using typename MatObject<T, N>::const_iterator;
    using MatObject<T, N>::m_component;

public:

//     ___             _               _           
//    / __|___ _ _  __| |_ _ _ _  _ __| |_ ___ _ _ 
//   | (__/ _ \ ' \(_-<  _| '_| || / _|  _/ _ \ '_|
//    \___\___/_||_/__/\__|_|  \_,_\__|\__\___/_|  
//                                                 

    /// @brief Default constructor (fill with 0)
    BaseVector();

    /// @brief Constructor with an array
    /// @param init_array Array of the components
    BaseVector(const std::array<T,N>& init_array);

    /// @brief Constructor with an initializer list
    /// @param init_list Initializer list containing the components
    BaseVector(const std::initializer_list<T>& init_list);

//    __  __     _   _            _    
//   |  \/  |___| |_| |_  ___  __| |___
//   | |\/| / -_)  _| ' \/ _ \/ _` (_-<
//   |_|  |_\___|\__|_||_\___/\__,_/__/
//                                     

    // === Utils ===

    /// @brief Get the type of the object
    /// @return The type of the object
    virtual type_mat get_type_object() const override { return type_mat::mat_bve; }

    /// @brief Get the size (dimension) of the vector
    /// @return The size (dimension) of the vector
    uint32_t size() const;

//     ___                     _              
//    / _ \ _ __  ___ _ _ __ _| |_ ___ _ _ ___
//   | (_) | '_ \/ -_) '_/ _` |  _/ _ \ '_(_-<
//    \___/| .__/\___|_| \__,_|\__\___/_| /__/
//         |_|                                

    // === Accessor operators ===

    /// @brief Constant accessor operator
    /// @param index Index of the element
    /// @return A constant reference to the element
    const T& operator[](uint32_t index) const;

    /// @brief Accessor operator
    /// @param index Index of the element
    /// @return A reference to the element
    T& operator[](uint32_t index);

    // === Cast operator ===

    /// @brief Cast to a vector
    operator Vector<T,N>&();

    /// @brief Cast to a complex
    /// @warning Only if N = 2
    operator Complex<T>&();

    /// @brief Cast to a quaternion
    /// @warning Only if N = 4
    operator Quaternion<T>&();

};


// ===================================================================================================
// ===================================================================================================


//    ___       __ _      _ _   _             
//   |   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
//   | |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
//   |___/\___|_| |_|_||_|_|\__|_\___/_||_/__/
//                                            

template<typename T, uint32_t N>
BaseVector<T,N>::BaseVector() {
    m_component.fill(T(0));
}

template<typename T, uint32_t N>
BaseVector<T,N>::BaseVector(const std::array<T,N>& init_array){
    m_component = init_array;
}

template<typename T, uint32_t N>
BaseVector<T,N>::BaseVector(const std::initializer_list<T>& init_list) {    
    m_component.fill(T(0));
    uint32_t last = std::min((size_t)N, init_list.size());
    std::copy(init_list.begin(), init_list.begin()+last, m_component.begin());
}

template<typename T, uint32_t N>
uint32_t BaseVector<T,N>::size() const {
    return N;
}

template<typename T, uint32_t N>
const T& BaseVector<T,N>::operator[](uint32_t index) const {
    return m_component[index];
}

template<typename T, uint32_t N>
T& BaseVector<T,N>::operator[](uint32_t index) {
    return m_component[index];
}

template<typename T, uint32_t N>
BaseVector<T,N>::operator Vector<T,N>&() {
    return *reinterpret_cast<Vector<T,N>*>(this);
}

template<typename T, uint32_t N>
BaseVector<T,N>::operator Complex<T>&() {
    if (N != 2) {
        throw std::runtime_error("MAT_ERROR : Can only create a Complex from a BaseVector of dimension 2");
    }
    
    return *reinterpret_cast<Complex<T>*>(this);
}

template<typename T, uint32_t N>
BaseVector<T,N>::operator Quaternion<T>&() {
    if (N != 4) {
        throw std::runtime_error("MAT_ERROR : Can only create a Quaternion from a BaseVector of dimension 4");
    }

    return *reinterpret_cast<Quaternion<T>*>(this);
}

}