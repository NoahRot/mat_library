#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

#include "Type.hpp"
#include "BaseVector.hpp"
#include "Matrix.hpp"

namespace mat {

template<typename T, uint32_t SIZE>
bool save_txt(const MatObject<T,SIZE>& obj, std::string file_path, char separator = ' ') {
    // Open file
    std::ofstream stream(file_path);

    // Check if the file has been open
    if (!stream.is_open()) {
        std::cout << "MAT_ERROR : Can not open file : " << file_path << std::endl;
        return false;
    }

    // Write metadata
    type_mat t_mat = obj.get_type_object();
    type_content t_con = obj.get_type_content();

    stream << to_str(t_mat) << " " << SIZE << " " << to_str(t_con) << " ";

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
            throw std::runtime_error("MAT_ERROR : Serialized data bigger than actual structure");
        }
        data[count] = val;
        ++count;
    }

    if (count < SIZE) {
        throw std::runtime_error("MAT_ERROR : Serialized data smaller than actual structure");
    }

    return true;
}

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

template<typename T, uint32_t SIZE>
bool load_bin(MatObject<T,SIZE>& obj, std::string file_path) {
    // Open the file
    std::ifstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        throw std::runtime_error("MAT_ERROR: Cannot open file for reading");
    }

    // Load metadata
    uint32_t mat_t, t, size;

    stream.read(reinterpret_cast<char*>(&mat_t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&t), sizeof(uint32_t));

    // Check the metadata
    if (mat_t != (uint32_t)obj.get_type_object()) {
        throw std::runtime_error("MAT_ERROR: mat type do not match");
    }
    if (t != (uint32_t)get_type<T>()) {
        throw std::runtime_error("MAT_ERROR: type of content do not match");
    }
    if (SIZE != size) {
        throw std::runtime_error("MAT_ERROR: size do not match");
    }

    // Read data
    stream.read(reinterpret_cast<char*>(obj.begin()), sizeof(T) * size);
    if (!stream) {
        throw std::runtime_error("MAT_ERROR: Incomplete vector data");
    }

    return true;
}

/*

template<typename T, uint32_t N>
bool save_bin(const BaseVector<T,N>& vec, std::string file_path) {
    // Open the file
    std::ofstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        std::cerr << "MAT_ERROR: Cannot open file for writing: " << file_path << std::endl;
        return false;
    }

    // Write metadata
    uint32_t mat_t = (uint32_t)get_mat_type(&vec);
    uint32_t t = (uint32_t)get_type<T>();
    uint32_t size = N;
    
    stream.write(reinterpret_cast<const char*>(&mat_t), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&t), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));

    // Write content
    stream.write(reinterpret_cast<const char*>(vec.begin()), sizeof(T) * N);

    return true;
}


template<typename T, uint32_t N, uint32_t M>
bool save_bin(const Matrix<T,N,M>& mat, std::string file_path) {
    // Open the file
    std::ofstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        std::cerr << "MAT_ERROR: Cannot open file for writing: " << file_path << std::endl;
        return false;
    }

    // Write metadata
    uint32_t mat_t = (uint32_t)mat_type::mat_mat;
    uint32_t t = (uint32_t)get_type<T>();
    uint32_t row = N;
    uint32_t column = M;
    
    stream.write(reinterpret_cast<const char*>(&mat_t), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&t), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&row), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(&column), sizeof(uint32_t));

    // Write content
    const std::array<T, N*M>& data = mat.data();
    stream.write(reinterpret_cast<const char*>(data.data()), sizeof(T) * N * M);

    return true;
}

template<typename T, uint32_t N>
bool load_bin(BaseVector<T,N>& vec, std::string file_path) {
    // Open the file
    std::ifstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        throw std::runtime_error("MAT_ERROR: Cannot open file for reading");
    }

    // Load metadata
    uint32_t mat_t, t, size;

    stream.read(reinterpret_cast<char*>(&mat_t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

    // Check the metadata
    if (mat_t != (uint32_t)get_mat_type(&vec)) {
        throw std::runtime_error("MAT_ERROR: mat type do not match");
    }
    if (t != (uint32_t)get_type<T>()) {
        throw std::runtime_error("MAT_ERROR: type of content do not match");
    }
    if (N != size) {
        throw std::runtime_error("MAT_ERROR: size do not match");
    }

    // Read data
    stream.read(reinterpret_cast<char*>(vec.begin()), sizeof(T) * size);
    if (!stream) {
        throw std::runtime_error("MAT_ERROR: Incomplete vector data");
    }

    return true;
}

template<typename T, uint32_t N, uint32_t M>
bool load_bin(Matrix<T,N,M>& mat, std::string file_path) {
    // Open the file
    std::ifstream stream(file_path, std::ios::binary);

    // Check if the file is open
    if (!stream.is_open()) {
        throw std::runtime_error("MAT_ERROR: Cannot open file for reading");
    }

    // Load metadata
    uint32_t mat_t, t, row, column;

    stream.read(reinterpret_cast<char*>(&mat_t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&t), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&row), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&column), sizeof(uint32_t));

    // Check the metadata
    if (mat_t != (uint32_t)mat_type::mat_mat) {
        throw std::runtime_error("MAT_ERROR: mat type do not match");
    }
    if (t != (uint32_t)get_type<T>()) {
        throw std::runtime_error("MAT_ERROR: type of content do not match");
    }
    if (N != row) {
        throw std::runtime_error("MAT_ERROR: row do not match");
    }
    if (M != column) {
        throw std::runtime_error("MAT_ERROR: column do not match");
    }

    // Read data
    stream.read(reinterpret_cast<char*>(mat.begin()), sizeof(T) * N * M);
    if (!stream) {
        throw std::runtime_error("MAT_ERROR: Incomplete vector data");
    }

    return true;
}
*/
}