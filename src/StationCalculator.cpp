#include "StationCalculator.hpp"

#include <ranges>

StationCalculator::StationCalculator(
    const std::vector<Satellite>& satellites,
    const std::vector<Station>& stations
) : satellites(satellites), stations(stations) {}

std::vector<StationVisibility> StationCalculator::calculateStationSequence(const Station& station) const {
    const auto visibilities = generateVisibilities(station);

    if(visibilities.empty()) {
        return {};
    }

    struct Event {
        std::chrono::seconds time;
    };

    std::vector<Event> events;
    events.reserve(visibilities.size() * 2);

    for(const auto& [satelliteId, timeRange] : visibilities) {
        events.emplace_back(timeRange.start);
        events.emplace_back(timeRange.end);
    }

    std::ranges::sort(events, {}, &Event::time);

    events.erase(
        std::ranges::unique(
            events,
            [](const auto& lhs, const auto& rhs) {
                return lhs.time == rhs.time;
            }
        ).begin(),
        events.end()
    );

    auto satellitePriority = [this](const std::string_view id) {
        const auto it = std::ranges::find(satellites, id, &Satellite::getId);

        return it == satellites.end() ? std::numeric_limits<int>::min() : it->getPriority();
    };

    std::vector<StationVisibility> result;

    for(std::size_t i = 0; i + 1 < events.size(); ++i) {
        const auto interval = TimeRange(events[i].time, events[i + 1].time);

        if(interval.start == interval.end) {
            continue;
        }

        const StationVisibility* best = nullptr;

        for(const auto& visibility : visibilities) {
            if(visibility.timeRange.start > interval.start)
                continue;

            if(visibility.timeRange.end < interval.end)
                continue;

            if(best == nullptr) {
                best = &visibility;
                continue;
            }

            const auto lhsPriority = satellitePriority(visibility.satelliteId);
            const auto rhsPriority = satellitePriority(best->satelliteId);

            if(lhsPriority > rhsPriority) {
                best = &visibility;
                continue;
            }

            if(lhsPriority == rhsPriority && visibility.timeRange.end > best->timeRange.end) {
                best = &visibility;
            }
        }

        if(!best) {
            continue;
        }

        if(!result.empty() && result.back().satelliteId == best->satelliteId && result.back().timeRange.end == interval.start) {
            result.back().timeRange.end = interval.end;
        } else {
            result.emplace_back(best->satelliteId, interval);
        }
    }

    return result;
}

float StationCalculator::calculateStationCoverageRatio(const Station& station) const {
    return 0;
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
