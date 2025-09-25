#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>

class Environment {
    std::unordered_map<std::string, int> vars;
public:
    Environment() = default;
    void set(const std::string &name, int value);
    int get(const std::string &name) const;
    bool exists(const std::string &name) const;
};

#endif
