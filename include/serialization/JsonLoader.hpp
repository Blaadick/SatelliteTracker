#pragma once

#include "data/Satellite.hpp"
#include "data/Station.hpp"

class JsonLoader final {
public:
    JsonLoader(
        std::vector<Satellite>& satellites,
        std::vector<Station>& stations
    );

    void loadSatellites() const;

    void loadStations() const;

private:
    std::vector<Satellite>& satellites;
    std::vector<Station>& stations;

    static Satellite loadSatellite(const nlohmann::json& data);

    static Station loadStation(const nlohmann::json& data);
};
