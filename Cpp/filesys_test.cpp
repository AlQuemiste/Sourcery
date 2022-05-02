#include <iostream>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <string>

namespace fs = std::filesystem;
namespace ch = std::chrono;
int main()
{
    std::time_t epoch_time = ch::system_clock::to_time_t(ch::system_clock::now());
    const std::string tmp_name {"$_nsx_tmp_" + std::to_string(epoch_time)};
    std::cout << "temp name = " << tmp_name << std::endl;

    fs::path tmp = fs::temp_directory_path();
    std::cout << "tmp path =  " << tmp << std::endl;
    fs::create_directories(tmp / "abcdef/example");
    std::uintmax_t n = fs::remove_all(tmp / "abcdef");
    std::cout << "Deleted " << n << " files or directories\n";
    const bool removed = fs::remove(tmp / "abcdef");
    std::cout << std::boolalpha << "removed = " << removed << std::endl;
    
    fs::path p = fs::current_path() / "sandbox";
    fs::create_directories(p/"from");
    std::ofstream(p/"from/file1.txt").put('a');
    fs::create_directory(p/"to");

//    fs::rename(p/"from/file1.txt", p/"to/"); // error: to is a directory
    fs::rename(p/"from/file1.txt", p/"to/file2.txt");
    
    
    const fs::path tmp_filepath = tmp / (tmp_name + ".tmp");
    const std::string filepath_str = tmp_filepath.string();
    std::cout << "TMP filepath = '" + filepath_str + "'" << std::endl;
    return 0;
}

// g++ -std=c++17 filesys_test.cpp -lstdc++fs
