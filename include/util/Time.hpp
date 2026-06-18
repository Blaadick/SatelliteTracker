#pragma once

#include <chrono>

namespace util {
    /**
     * @param str expected "HH:mm:ss" format
     */
    inline std::chrono::seconds parseTime(const std::string& str) {
        return std::chrono::hours(stoi(str.substr(0, 2))) +
            std::chrono::minutes(stoi(str.substr(3, 2))) +
            std::chrono::seconds(stoi(str.substr(6, 2)));
    }
}
