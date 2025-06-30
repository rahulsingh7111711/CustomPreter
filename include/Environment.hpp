#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>

class Environment {
public:
    void define(const std::string& name, const std::any& value) {
        values[name] = value;
    }

    std::any get(const std::string& name) {
        if (values.find(name) != values.end()) {
            return values[name];
        }
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }

private:
    std::unordered_map<std::string, std::any> values;
};

#endif // ENVIRONMENT_HPP
