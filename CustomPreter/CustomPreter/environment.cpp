#include "environment.h"
#include <stdexcept>

void Environment::set(const std::string &name, int value) {
    vars[name] = value;
}

int Environment::get(const std::string &name) const {
    auto it = vars.find(name);
    if (it == vars.end()) {
        throw std::runtime_error("Runtime error: undefined variable '" + name + "'");
    }
    return it->second;
}

bool Environment::exists(const std::string &name) const {
    return vars.find(name) != vars.end();
}
