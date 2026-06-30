#pragma once

#include <chrono>
#include <nlohmann/json.hpp>

struct TimeRange {
    std::chrono::seconds start;
    std::chrono::seconds end;

    [[nodiscard]]
    std::chrono::seconds duration() const;

    [[nodiscard]]
    static TimeRange fromJson(const nlohmann::json& data);

    [[nodiscard]]
    nlohmann::json toJson() const;

    [[nodiscard]]
    std::string toString() const;
};
