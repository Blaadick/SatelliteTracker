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
    const std::string& getId() const;

    [[nodiscard]]
    const std::map<std::string, std::vector<TimeRange>>& getSatellites() const;

    [[nodiscard]]
    nlohmann::json toJson() const;

protected:
    std::string id;
    std::map<std::string, std::vector<TimeRange>> satellites;
};
