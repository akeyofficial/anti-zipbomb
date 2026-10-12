#include <iostream>
#include <string>
#include <cstdint>
#include <minizip/unzip.h>

const uint64_t MAX_TOTAL_UNCOMPRESSED = 500ULL * 1024 * 1024;
const uint64_t MAX_SINGLE_FILE = 500ULL * 1024 * 1024;
const uint32_t MAX_RATIO = 100;
const uint32_t MAX_FILES = 10000;

std::string formatSize(uint64_t bytes) {
    const char* units[] = {"b", "kb", "mb", "gb", "tb"};
    int i = 0;
    double size = (double)bytes;
    while (size >= 1024 && i < 4) {
        size /= 1024;
        i++;
    }
    
    char buf[64];
    if (size == (uint64_t)size) {
        snprintf(buf, sizeof(buf), "%llu%s", (unsigned long long)size, units[i]);
    } else {
        snprintf(buf, sizeof(buf), "%.1f%s", size, units[i]);
    }
    return std::string(buf);
}

std::pair<std::string, uint64_t> checkZip(const std::string& path) {
    unzFile uf = unzOpen(path.c_str());
    if (!uf) return {"corrupted", 0};
    
    unz_global_info64 gi;
    if (unzGetGlobalInfo64(uf, &gi) != UNZ_OK) {
        unzClose(uf);
        return {"corrupted", 0};
    }
    
    if (gi.number_entry > MAX_FILES) {
        unzClose(uf);
        return {"count", 0};
    }
    
    uint64_t total = 0;
    int ret = unzGoToFirstFile(uf);
    while (ret == UNZ_OK) {
        unz_file_info64 fi;
        char name[256];
        if (unzGetCurrentFileInfo64(uf, &fi, name, sizeof(name), nullptr, 0, nullptr, 0) != UNZ_OK) {
            unzClose(uf);
            return {"corrupted", 0};
        }
        
        total += fi.uncompressed_size;
        
        if (fi.uncompressed_size > MAX_SINGLE_FILE || total > MAX_TOTAL_UNCOMPRESSED) {
            unzClose(uf);
            return {"size", total};
        }
        
        double ratio = 0.0;
        if (fi.compressed_size > 0) {
            ratio = (double)fi.uncompressed_size / (double)fi.compressed_size;
        } else if (fi.uncompressed_size > 0) {
            ratio = 999999.0;
        }
        if (ratio > MAX_RATIO) {
            unzClose(uf);
            return {"ratio", total};
        }
        
        ret = unzGoToNextFile(uf);
    }
    
    unzClose(uf);
    return {"safe", total};
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <zipfile>" << std::endl;
        return 1;
    }
    
    auto result = checkZip(argv[1]);
    std::string status = result.first;
    uint64_t size = result.second;
    
    if (status == "safe") {
        std::cout << "Archive is safe." << std::endl;
        return 0;
    } else if (status == "corrupted") {
        std::cout << "Detected (corrupted), Dont extract." << std::endl;
    } else {
        std::cout << "Detected " << formatSize(size) << ", dont extract 😭✌" << std::endl;
    }
    return 1;
}
