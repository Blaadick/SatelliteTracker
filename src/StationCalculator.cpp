#include "StationCalculator.hpp"

StationCalculator::StationCalculator(
    const std::vector<Satellite>& satellites,
    const std::vector<Station>& stations
) : satellites(satellites), stations(stations) {}

std::vector<std::pair<std::string, TimeRange>> StationCalculator::calculateStationSequence(const Station& station) const {
    std::vector<std::pair<std::string, TimeRange>> sequence;

    for(const auto& [satelliteId, satelliteTimeRanges] : station.getSatellites()) {
        sequence.emplace_back(satelliteId, satelliteTimeRanges[0]);
    }

    return sequence;
}
