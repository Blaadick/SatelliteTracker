#include "data/TimeRange.hpp"

#include <format>

nlohmann::json TimeRange::toJson() const {
    return {{"start", std::format("{:%H:%M:%S}", start)}, {"end", std::format("{:%H:%M:%S}", end)}};
}
