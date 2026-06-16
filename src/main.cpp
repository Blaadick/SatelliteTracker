#include "data/Satellite.hpp"
#include "data/Station.hpp"
#include "serialization/JsonLoader.hpp"

int main() {
    std::vector<Satellite> satellites;
    std::vector<Station> stations;

    JsonLoader loader(satellites, stations);
    loader.loadSatellites();
    loader.loadStations();
}
