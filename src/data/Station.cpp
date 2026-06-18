#include "data/Station.hpp"

#include "data/TimeRange.hpp"

Station::Station(
    const std::string& id,
    const std::map<std::string, std::vector<TimeRange>>& satellites
) : id(id), satellites(satellites) {}

const std::string& Station::getId() const {
    return id;
}

const std::map<std::string, std::vector<TimeRange>>& Station::getSatellites() const {
    return satellites;
}

nlohmann::json Station::toJson() const {
    nlohmann::json output;
    output["id"] = id;
    output["satellites"] = nlohmann::json::array();

    for(const auto& [satelliteId, timeRanges] : satellites) {
        nlohmann::json satelliteJson;
        satelliteJson["id"] = satelliteId;
        satelliteJson["times"] = nlohmann::json::array();

        for(const auto& time : timeRanges) {
            satelliteJson["times"].push_back(time.toJson());
        }

        output["satellites"].push_back(satelliteJson);
    }

    return output;
}
