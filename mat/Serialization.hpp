#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include "Type.hpp"
#include "BaseVector.hpp"
#include "Matrix.hpp"

namespace mat {

/// @brief Serialization of a MatObject in a text format 
/// @tparam T Type of content
/// @tparam SIZE Size of the MatObject
/// @param obj The MatObject
/// @param file_path The path to the file to store the data
/// @param separator What separate the components
/// @param precision The precision for the written value (number of written digit)
/// @return True if the process succeeded, false otherwise
template<typename T, uint32_t SIZE>
bool save_txt(const MatObject<T,SIZE>& obj, std::string file_path, char separator = ' ', uint32_t precision = 6) {
    // Open file
    std::ofstream stream(file_path);

    // Check if the file has been open
    if (!stream.is_open()) {
        std::cout << "MAT_ERROR : Can not open file : " << file_path << std::endl;
        return false;
    }

    // Set precision
    stream << std::setprecision(precision);

    // Write metadata
    type_mat t_mat = obj.get_type_object();
    type_content t_con = obj.get_type_content();

    stream << to_str(t_mat) << separator << SIZE << separator << to_str(t_con) << separator;

    // Write content
    const std::array<T,SIZE>& data = obj.data();
    for (uint32_t i(0) ; i < SIZE ; ++i) {
        stream << data[i];
        if (i != SIZE-1) {
            stream << separator;
        }
    }

    // Close file
    stream.close();
    return true;
}

/// @brief Deserialization of a MatObject from a text file
/// @tparam T Type of content
/// @tparam SIZE Size of the MatObject
/// @param obj The MatObject that will be filled with data
/// @param file_path The path to the file where the data are stored
/// @param separator What separate the components
/// @return True if the process succeeded, false otherwise
template<typename T, uint32_t SIZE>
bool load_txt(MatObject<T,SIZE>& obj, std::string file_path, char separator = ' ') {
    // Open file
    std::ifstream stream(file_path);

    // Check if the file has been open
    if (!stream.is_open()) {
        std::cout << "MAT_ERROR : Can not open file : " << file_path << std::endl;
        return false;
    }

    // Read line
    std::string line;
    std::getline(stream, line);

    // Replace separator with space if it's not already a space
    if (separator != ' ') {
        std::replace(line.begin(), line.end(), separator, ' ');
    }

    // Prepare values
    std::istringstream iss(line);
    T val;
    std::string str;

    // Read type mat
    if (iss >> str) {
        type_mat t = to_type_mat(str);
        if (t != obj.get_type_object() or t == type_mat::mat_unvalid_mat_type) {
            std::cout << "MAT_ERROR : Not able to store in different object type" << std::endl;
            return false;
        }
    }else{
        std::cout << "MAT_ERROR : Can not read object type metadata" << std::endl;
        return false;
    }

    // Read size
    if (iss >> val) {
        if (val != SIZE) {
            std::cout << "MAT_ERROR : Not able to store in different size object" << std::endl;
            return false;
        }
    }else{
        std::cout << "MAT_ERROR : Can not read size metadata" << std::endl;
        return false;
    }

    // Read content type
    if (iss >> str) {
        type_content t = to_type_content(str);
        if (t != obj.get_type_content() and t == type_content::mat_unvalid_type) {
            std::cout << "MAT_ERROR : Not able to store in different content type" << std::endl;
            return false;
        }
    }else{
        std::cout << "MAT_ERROR : Can not read content type metadata" << std::endl;
        return false;
    }
    
    // Read data
    std::array<T,SIZE>& data = obj.data();
    uint32_t count(0);
    while (iss >> val) {
        if (count >= SIZE) {
            std::cout << "MAT_ERROR : Serialized data bigger than actual structure" << std::endl;
            return false;
        }
        data[count] = val;
        ++count;
    }

    if (count < SIZE) {
        std::cout << "MAT_ERROR : Serialized data smaller than actual structure" << std::endl;
        return false;
    }

    return true;
}

/// @brief Serialization of a MatObject in a binary format
/// @tparam T Type of content
/// @tparam SIZE Size of the MatObject
/// @param obj The MatObject
/// @param file_path The path to the file to store the data
/// @return True if the process succeeded, false otherwise 
template<typename T, uint32_t SIZE>
bool save_bin(const MatObject<T,SIZE>& obj, std::string file_path) {
    // Open the file
    std::ofstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        std::cerr << "MAT_ERROR: Cannot open file for writing: " << file_path << std::endl;
        return false;
    }

    // Write metadata
    uint32_t mat_t = (uint32_t)obj.get_type_object();
    uint32_t size = SIZE;
    uint32_t t = (uint32_t)get_type<T>();
    
    stream.write(reinterpret_cast<const char*>(&mat_t), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&t), sizeof(uint32_t));

    // Write content
    stream.write(reinterpret_cast<const char*>(obj.begin()), sizeof(T) * SIZE);

    return true;
}

/// @brief Deserialization of a MatObject from a binary file
/// @tparam T Type of content
/// @tparam SIZE Size of the MatObject
/// @param obj The MatObject that will be filled with data
/// @param file_path The path to the file where the data are stored
/// @return True if the process succeeded, false otherwise 
template<typename T, uint32_t SIZE>
bool load_bin(MatObject<T,SIZE>& obj, std::string file_path) {
    // Open the file
    std::ifstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        std::cout << "MAT_ERROR: Cannot open file for reading" << std::endl;
        return false;
    }

    // Load metadata
    uint32_t mat_t, t, size;

    stream.read(reinterpret_cast<char*>(&mat_t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&t), sizeof(uint32_t));

    // Check the metadata
    if (mat_t != (uint32_t)obj.get_type_object()) {
        std::cout << "MAT_ERROR: mat type do not match" << std::endl;
        return false;
    }
    if (t != (uint32_t)get_type<T>()) {
        std::cout << "MAT_ERROR: type of content do not match" << std::endl;
        return false;
    }
    if (SIZE != size) {
        std::cout << "MAT_ERROR: size do not match" << std::endl;
        return false;
    }

    // Read data
    stream.read(reinterpret_cast<char*>(obj.begin()), sizeof(T) * size);
    if (!stream) {
        std::cout << "MAT_ERROR: Incomplete vector data" << std::endl;
        return false;
    }

    return true;
}

}