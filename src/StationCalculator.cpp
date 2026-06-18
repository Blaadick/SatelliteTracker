#include "StationCalculator.hpp"

StationCalculator::StationCalculator(
    const std::vector<Satellite>& satellites,
    const std::vector<Station>& stations
) : satellites(satellites), stations(stations) {}

std::vector<std::pair<std::string, TimeRange>> StationCalculator::calculateStationSequence(const Station& station) const {
    std::vector<StationVisibility> all;

    for(const auto& [satId, ranges] : station.getSatellites()) {
        for(const auto& r : ranges) {
            all.push_back({satId, r});
        }
    }

    if(all.empty()) {
        return {};
    }

    std::ranges::sort(
        all,
        [](const StationVisibility& a, const StationVisibility& b) {
            return a.timeRange.start < b.timeRange.start;
        }
    );

    std::vector<std::pair<std::string, TimeRange>> result;

    size_t i = 0;

    while(i < all.size()) {
        auto currentEnd = all[i].timeRange.end;
        std::string currentSat = all[i].satelliteId;

        result.push_back({currentSat, all[i].timeRange});

        bool extended = true;
        while(extended) {
            extended = false;

            std::chrono::seconds bestEnd = currentEnd;
            StationVisibility best;
            bool found = false;

            for(const auto& it : all) {
                if(it.timeRange.start <= currentEnd && it.timeRange.end > bestEnd) {
                    best = it;
                    bestEnd = it.timeRange.end;
                    found = true;
                }
            }

            if(found) {
                result.push_back({best.satelliteId, best.timeRange});
                currentEnd = bestEnd;
                extended = true;
            }
        }

        while(i < all.size() && all[i].timeRange.start <= currentEnd) {
            i++;
        }
    }

    return result;
}

std::vector<StationVisibility> StationCalculator::generateVisibilities(const Station& station) const {
    std::vector<StationVisibility> visibilities;

    for(const auto& [satelliteId, timeRanges] : station.getSatellites()) {
        for(const auto& timeRange : timeRanges) {
            visibilities.emplace_back(satelliteId, timeRange);
        }
    }

    std::ranges::sort(
        visibilities,
        [](const auto& a, const auto& b) {
            return a.timeRange.start < b.timeRange.start;
        }
    );

    return visibilities;
}
