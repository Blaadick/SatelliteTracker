#include "StationCalculator.hpp"

#include <algorithm>
#include <limits>
#include <unordered_map>

namespace {
    std::unordered_map<std::string, int> buildPriorityBySatelliteId(const std::vector<Satellite>& satellites) {
        std::unordered_map<std::string, int> priorityBySatelliteId;

        for(const auto& satellite : satellites) {
            priorityBySatelliteId.emplace(satellite.getId(), satellite.getPriority());
        }

        return priorityBySatelliteId;
    }

    int getSatellitePriority(const std::unordered_map<std::string, int>& priorityBySatelliteId, const std::string& satelliteId) {
        const auto priority = priorityBySatelliteId.find(satelliteId);
        return priority == priorityBySatelliteId.end() ? 0 : priority->second;
    }

    long long calculateVisibilityScore(const StationVisibility& visibility, const std::unordered_map<std::string, int>& priorityBySatelliteId) {
        return visibility.timeRange.duration().count() * getSatellitePriority(priorityBySatelliteId, visibility.satelliteId);
    }
}

StationCalculator::StationCalculator(
    const std::vector<Satellite>& satellites,
    const std::vector<Station>& stations
) : satellites(satellites), stations(stations) {}

std::vector<StationVisibility> StationCalculator::calculateStationSequence(const Station& station) const {
    const auto visibilities = generateVisibilities(station);

    if(visibilities.empty()) {
        return {};
    }

    const auto priorityBySatelliteId = buildPriorityBySatelliteId(satellites);

    std::vector<std::chrono::seconds> timePoints;
    timePoints.reserve(visibilities.size() * 2);

    for(const auto& [satelliteId, timeRange] : visibilities) {
        if(timeRange.end <= timeRange.start) {
            continue;
        }

        timePoints.emplace_back(timeRange.start);
        timePoints.emplace_back(timeRange.end);
    }

    std::ranges::sort(timePoints);
    const auto [uniqueBegin, uniqueEnd] = std::ranges::unique(timePoints);
    timePoints.erase(uniqueBegin, uniqueEnd);

    std::vector<StationVisibility> sequence;

    for(std::size_t index = 0; index + 1 < timePoints.size(); ++index) {
        const auto segmentStart = timePoints[index];
        const auto segmentEnd = timePoints[index + 1];

        if(segmentEnd <= segmentStart) {
            continue;
        }

        const StationVisibility* bestVisibility = nullptr;
        auto bestScore = std::numeric_limits<long long>::min();

        for(const auto& visibility : visibilities) {
            if(visibility.timeRange.start > segmentStart || visibility.timeRange.end < segmentEnd) {
                continue;
            }

            const auto segmentScore = (segmentEnd - segmentStart).count() * getSatellitePriority(priorityBySatelliteId, visibility.satelliteId);

            if(bestVisibility == nullptr || segmentScore > bestScore || (segmentScore == bestScore && visibility.satelliteId < bestVisibility->satelliteId)) {
                bestVisibility = &visibility;
                bestScore = segmentScore;
            }
        }

        if(bestVisibility == nullptr) {
            continue;
        }

        if(!sequence.empty() && sequence.back().satelliteId == bestVisibility->satelliteId && sequence.back().timeRange.end == segmentStart) {
            sequence.back().timeRange.end = segmentEnd;
            continue;
        }

        sequence.emplace_back(bestVisibility->satelliteId, TimeRange{segmentStart, segmentEnd});
    }

    return sequence;
}

float StationCalculator::calculateStationCoverageRatio(const Station& station) const {
    const auto visibilities = generateVisibilities(station);
    const auto sequence = calculateStationSequence(station);
    const auto priorityBySatelliteId = buildPriorityBySatelliteId(satellites);

    long long totalScore = 0;
    for(const auto& visibility : visibilities) {
        totalScore += calculateVisibilityScore(visibility, priorityBySatelliteId);
    }

    if(totalScore == 0) {
        return 0;
    }

    long long selectedScore = 0;
    for(const auto& visibility : sequence) {
        selectedScore += calculateVisibilityScore(visibility, priorityBySatelliteId);
    }

    return static_cast<float>(selectedScore) / static_cast<float>(totalScore);
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
