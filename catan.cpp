#include "catan.hpp"
#include <stdexcept>

Resource stringToResource(const std::string& s) {
    if (s == "Wood") return Resource::Wood;
    if (s == "Brick") return Resource::Brick;
    if (s == "Ore") return Resource::Ore;
    if (s == "Grain") return Resource::Grain;
    if (s == "Wool") return Resource::Wool;
    throw std::runtime_error("Invalid resource name");
}

std::string resourceToString(Resource r) {
    switch (r) {
        case Resource::Wood: return "Wood";
        case Resource::Brick: return "Brick";
        case Resource::Ore: return "Ore";
        case Resource::Grain: return "Grain";
        case Resource::Wool: return "Wool";
        default: return "Unknown";
    }
}
