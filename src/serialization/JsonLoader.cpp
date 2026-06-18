#include "serialization/JsonLoader.hpp"

#include <fstream>
#include <print>
#include "util/Paths.hpp"

JsonLoader::JsonLoader(
    std::vector<Satellite>& satellites,
    std::vector<Station>& stations
) : satellites(satellites), stations(stations) {}

void JsonLoader::loadSatellites() const {
    if(!util::createDirIfNotExists(util::localDataDirPath())) {
        std::println(stderr, "Failed to create directory \"{}\"", util::localDataDirPath());
        return;
    }

    std::ifstream dataFile(util::localDataDirPath().append("satellites.json"));
    nlohmann::json data = nlohmann::json::parse(dataFile);

    for(const auto& satelliteData : data) {
        satellites.emplace_back(loadSatellite(satelliteData));
    }

    std::println("Loaded {} satellites", satellites.size());
}

void JsonLoader::loadStations() const {
    if(!util::createDirIfNotExists(util::localDataDirPath())) {
        std::println(stderr, "Failed to create directory \"{}\"", util::localDataDirPath());
        return;
    }

    std::ifstream dataFile(util::localDataDirPath().append("stations.json"));
    nlohmann::json data = nlohmann::json::parse(dataFile);

    for(const auto& stationData : data) {
        stations.emplace_back(loadStation(stationData));
    }

    std::println("Loaded {} stations", stations.size());
}

Satellite JsonLoader::loadSatellite(const nlohmann::json& data) {
    return {data["id"], data["priority"]};
}

Station JsonLoader::loadStation(const nlohmann::json& data) {
    std::map<std::string, std::vector<TimeRange>> rawSatellites;
    for(const auto& rawSatellite : data["satellites"]) {
        std::vector<TimeRange> rawTimeRanges;
        for(const auto& rawTime : rawSatellite["times"]) {
            rawTimeRanges.emplace_back(TimeRange::fromJson(rawTime));
        }

        rawSatellites.emplace(rawSatellite["id"], rawTimeRanges);
    }

    return {data["id"], rawSatellites};
}
