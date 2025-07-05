#pragma once

#include <iostream>
#include <inttypes.h>

#include "MatObject.hpp"

namespace mat {

/// @brief Type of MatObject that exist
enum class type_mat {
    mat_bve,
    mat_vec,
    mat_com,
    mat_qua,
    mat_mat,
    mat_unvalid_mat_type
};

/// @brief Type of content in the MatObject that are allowed
enum class type_content {
    mat_int8,
    mat_uint8,
    mat_int16,
    mat_uint16,
    mat_int32,
    mat_uint32,
    mat_int64,
    mat_uint64,
    mat_float,
    mat_double,
    mat_long_double,
    mat_char,
    mat_uchar,
    mat_unvalid_type
};

/// @brief Function to get the type of content (see specialisation below)
/// @tparam T Type of content
template<typename T>
struct type_traits {
    static constexpr type_content value = type_content::mat_unvalid_type;
};

template<> struct type_traits<int8_t>        { static constexpr type_content value = type_content::mat_int8; };
template<> struct type_traits<uint8_t>       { static constexpr type_content value = type_content::mat_uint8; };
template<> struct type_traits<int16_t>       { static constexpr type_content value = type_content::mat_int16; };
template<> struct type_traits<uint16_t>      { static constexpr type_content value = type_content::mat_uint16; };
template<> struct type_traits<int32_t>       { static constexpr type_content value = type_content::mat_int32; };
template<> struct type_traits<uint32_t>      { static constexpr type_content value = type_content::mat_uint32; };
template<> struct type_traits<int64_t>       { static constexpr type_content value = type_content::mat_int64; };
template<> struct type_traits<uint64_t>      { static constexpr type_content value = type_content::mat_uint64; };
template<> struct type_traits<float>         { static constexpr type_content value = type_content::mat_float; };
template<> struct type_traits<double>        { static constexpr type_content value = type_content::mat_double; };
template<> struct type_traits<long double>   { static constexpr type_content value = type_content::mat_long_double; };
template<> struct type_traits<char>          { static constexpr type_content value = type_content::mat_char; };

/// @brief Get the type enum value
/// @tparam T The type
/// @return The enum value, or unvalid value if not supported type
template<typename T>
type_content get_type() {
    return type_traits<T>::value;
}

/// @brief Transform a type of MatObject in a string
/// @param t Type of MatObject
/// @return A string refering to the type of the MatObject
inline std::string to_str(type_mat t) {
    switch (t) {
        case type_mat::mat_bve: return "bve";
        case type_mat::mat_vec: return "vec";
        case type_mat::mat_com: return "com";
        case type_mat::mat_qua: return "qua";
        case type_mat::mat_mat: return "mat";
        default:                return "###";
    }
}

/// @brief Transform a string to the type of MatObject
/// @param t The string
/// @return The type of MatObject
inline type_mat to_type_mat(std::string t) {
    if (t == "bve") { return type_mat::mat_bve; }
    if (t == "vec") { return type_mat::mat_vec; }
    if (t == "com") { return type_mat::mat_com; }
    if (t == "qua") { return type_mat::mat_qua; }
    if (t == "mat") { return type_mat::mat_mat; }

    return type_mat::mat_unvalid_mat_type;
}

/// @brief Transform a type of content in a string
/// @param t Type of content
/// @return A string refering to the type of the content
inline std::string to_str(type_content t) {
    switch (t) {
        case type_content::mat_int8:            return "int8";
        case type_content::mat_uint8:           return "uint8";
        case type_content::mat_int16:           return "int16";
        case type_content::mat_uint16:          return "uint16";
        case type_content::mat_int32:           return "int32";
        case type_content::mat_uint32:          return "uint32";
        case type_content::mat_int64:           return "int64";
        case type_content::mat_uint64:          return "uint64";
        case type_content::mat_float:           return "float";
        case type_content::mat_double:          return "double";
        case type_content::mat_long_double:     return "long_double";
        case type_content::mat_char:            return "char";
        case type_content::mat_uchar:           return "unsigned_char";
        case type_content::mat_unvalid_type:    return "invalid";
        default:                                return "ERROR";
    }
}

/// @brief Transform a string to the type of content
/// @param t The string
/// @return The type of content
inline type_content to_type_content(std::string t) {
    if (t == "int8")            { return type_content::mat_int8; }
    if (t == "uint8")           { return type_content::mat_uint8; }
    if (t == "int16")           { return type_content::mat_int16; }
    if (t == "uint16")          { return type_content::mat_uint16; }
    if (t == "int32")           { return type_content::mat_int32; }
    if (t == "uint32")          { return type_content::mat_uint32; }
    if (t == "int64")           { return type_content::mat_int64; }
    if (t == "uint64")          { return type_content::mat_uint64; }
    if (t == "float")           { return type_content::mat_float; }
    if (t == "double")          { return type_content::mat_double; }
    if (t == "long_double")     { return type_content::mat_long_double; }
    if (t == "char")            { return type_content::mat_char; }
    if (t == "unsigned_char")   { return type_content::mat_uchar; }
    if (t == "invalid")         { return type_content::mat_unvalid_type; }

    return type_content::mat_unvalid_type;
}

}