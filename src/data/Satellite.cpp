#include "data/Satellite.hpp"

Satellite::Satellite(const std::string& id, const int& priority) : id(id), priority(priority) {}

const std::string& Satellite::getId() const {
    return id;
}

const int& Satellite::getPriority() const {
    return priority;
}

nlohmann::json Satellite::toJson() const {
    return {{"id", id}, {"priority", priority}};
}
