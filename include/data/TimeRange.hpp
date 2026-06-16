#pragma once

#include <chrono>
#include <nlohmann/json.hpp>
#include "util/Time.hpp"

struct TimeRange {
    std::chrono::seconds start;
    std::chrono::seconds end;

    [[nodiscard]]
    static TimeRange fromJson(const nlohmann::json& data) {
        return {parseTime(data["start"]), parseTime(data["end"])};
    }

    [[nodiscard]]
    nlohmann::json toJson() const;
};
