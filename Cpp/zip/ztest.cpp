#include <iostream>
#include <vector>
#include <cstring>
#include <zlib.h>
#include <bzlib.h>

// compress data using zlib
std::vector<char> compressZlib(const std::vector<char>& data) {
    uLongf compressedSize = compressBound(data.size());
    std::vector<char> compressedData(compressedSize);

    if (compress2(reinterpret_cast<Bytef*>(compressedData.data()), &compressedSize,
                  reinterpret_cast<const Bytef*>(data.data()), data.size(), Z_BEST_COMPRESSION) != Z_OK) {
        throw std::runtime_error("zlib compression failed");
    }

    compressedData.resize(compressedSize);
    return compressedData;
}

// decompress data using zlib
std::vector<char> decompressZlib(const std::vector<char>& compressedData, size_t originalSize) {
    std::vector<char> decompressedData(originalSize);
    uLongf decompressedSize = originalSize;

    if (uncompress(reinterpret_cast<Bytef*>(decompressedData.data()), &decompressedSize,
                   reinterpret_cast<const Bytef*>(compressedData.data()), compressedData.size()) != Z_OK) {
        throw std::runtime_error("zlib decompression failed");
    }

    return decompressedData;
}

// compress data using bzip2
std::vector<char> compressBzip2(const std::vector<char>& data) {
    unsigned int compressedSize = data.size() * 1.1 + 600; // Estimate size
    std::vector<char> compressedData(compressedSize);

    int result = BZ2_bzBuffToBuffCompress(compressedData.data(), &compressedSize,
                                          const_cast<char*>(data.data()), data.size(), 9, 0, 30);
    if (result != BZ_OK) {
        throw std::runtime_error("bzip2 compression failed");
    }

    compressedData.resize(compressedSize);
    return compressedData;
}

// decompress data using bzip2
std::vector<char> decompressBzip2(const std::vector<char>& compressedData, size_t originalSize) {
    std::vector<char> decompressedData(originalSize);
    unsigned int decompressedSize = originalSize;

    int result = BZ2_bzBuffToBuffDecompress(decompressedData.data(), &decompressedSize,
                                            const_cast<char*>(compressedData.data()), compressedData.size(), 0, 0);
    if (result != BZ_OK) {
        throw std::runtime_error("bzip2 decompression failed");
    }

    return decompressedData;
}

int main() {
    // data to compress
    std::string originalData = "Hello, this is a test string to compress using zlib and bzip2!";
    std::vector<char> data(originalData.begin(), originalData.end());

    // compress and decompress using zlib
    try {
        auto compressedZlib = compressZlib(data);
        auto decompressedZlib = decompressZlib(compressedZlib, data.size());

        std::cout << "* zlib compression and decompression successful!" << std::endl;
        std::cout << "  original size: " << data.size() << ", compressed size: " << compressedZlib.size() << std::endl;
        std::cout << "  decompressed data: " << std::string(decompressedZlib.begin(), decompressedZlib.end()) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  zlib error: " << e.what() << std::endl;
    }

    // compress and decompress using bzip2
    try {
        auto compressedBzip2 = compressBzip2(data);
        auto decompressedBzip2 = decompressBzip2(compressedBzip2, data.size());

        std::cout << "* bzip2 compression and decompression successful!" << std::endl;
        std::cout << "  original size: " << data.size() << ", compressed size: " << compressedBzip2.size() << std::endl;
        std::cout << "  decompressed data: " << std::string(decompressedBzip2.begin(), decompressedBzip2.end()) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "  bzip2 error: " << e.what() << std::endl;
    }

    return 0;
}
