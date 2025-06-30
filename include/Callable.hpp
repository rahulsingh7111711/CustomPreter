/*#ifndef CALLABLE_HPP
#define CALLABLE_HPP

#include <any>
#include <vector>
#include <string>

class Interpreter; // forward declaration

class Callable {
public:
    virtual std::any call(Interpreter* interpreter, const std::vector<std::any>& arguments) = 0;
    virtual int arity() const = 0;
    virtual std::string toString() const = 0;
    virtual ~Callable() = default;
};

#endif // CALLABLE_HPP
*/