#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "TimeRange.hpp"

class Station {
public:
     Station(
        const std::string& id,
        const std::map<std::string, std::vector<TimeRange>>& satellites
    );

    [[nodiscard]]
    nlohmann::json toJson() const;

protected:
    std::string id;
    std::map<std::string, std::vector<TimeRange>> satellites;
};
