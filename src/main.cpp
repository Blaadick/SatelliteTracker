#include <iostream>
#include <print>
#include "StationCalculator.hpp"
#include "data/Satellite.hpp"
#include "data/Station.hpp"
#include "serialization/JsonLoader.hpp"

int main() {
    std::vector<Satellite> satellites;
    std::vector<Station> stations;

    JsonLoader loader(satellites, stations);
    loader.loadSatellites();
    loader.loadStations();

    StationCalculator calculator(satellites, stations);

    for(const auto& station : stations) {
        const auto stationSteps = calculator.calculateStationSequence(station);
        const auto stationCoverageRatio = calculator.calculateStationCoverageRatio(station);

        std::cout << "\n" + station.getId() + ":\n";
        for(const auto& [satelliteId, timeRange] : stationSteps) {
            std::println("    {}: {}", satelliteId, timeRange.toString());
        }
        std::println("Coverage: {}%", stationCoverageRatio * 100);
    }
}
