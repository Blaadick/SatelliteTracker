#pragma once

#include <string>
#include "data/TimeRange.hpp"

struct StationVisibility {
    std::string satelliteId;
    TimeRange timeRange;
};
