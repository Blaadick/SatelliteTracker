#pragma once

#include <filesystem>

inline std::filesystem::path localDataDirPath() {
    #ifdef __linux__
    std::filesystem::path homeDir = getenv("HOME");
    return homeDir.append(".local/share").append(PROJECT_NAME);
    #endif
}

inline bool createDirIfNotExists(const std::filesystem::path& path) {
    if(!std::filesystem::exists(path)) {
        if(!std::filesystem::create_directory(path)) {
            return false;
        }
    }

    return true;
}