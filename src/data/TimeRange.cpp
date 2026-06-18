#include "data/TimeRange.hpp"

#include <format>
#include "util/Time.hpp"

TimeRange TimeRange::fromJson(const nlohmann::json& data) {
    return TimeRange(util::parseTime(data["start"]), util::parseTime(data["end"]));
}

nlohmann::json TimeRange::toJson() const {
    return {
        {"start", std::format("{:%H:%M:%S}", start)},
        {"end", std::format("{:%H:%M:%S}", end)}
    };
}

std::string TimeRange::toString() const {
    return std::format("{:%H:%M:%S} - {:%H:%M:%S}", start, end);
}
