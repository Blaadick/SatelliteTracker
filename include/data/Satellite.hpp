#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Satellite {
public:
    Satellite(const std::string& id, const int& priority);

    [[nodiscard]]
    const std::string& getId() const;

    [[nodiscard]]
    const int& getPriority() const;

    [[nodiscard]]
    nlohmann::json toJson() const;

protected:
    std::string id;
    int priority;
};
