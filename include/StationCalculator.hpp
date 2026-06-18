#pragma once

#include <vector>
#include "data/Satellite.hpp"
#include "data/Station.hpp"

class StationCalculator final {
public:
    StationCalculator(
        const std::vector<Satellite>& satellites,
        const std::vector<Station>& stations
    );

    [[nodiscard]]
    std::vector<std::pair<std::string, TimeRange>> calculateStationSequence(const Station& station) const;

private:
    const std::vector<Satellite>& satellites;
    const std::vector<Station>& stations;
};
