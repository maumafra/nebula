#pragma once

#include <string>

namespace nebula {

enum ModuleDef {
    DATA,
    ECS,
    GRAPHICS,
    WINDOW
};

class Module {
public:
    Module(ModuleDef module, const char *name);
    ~Module();

    ModuleDef getModule() const;

    const char *getName() const;

    static Module *getInstance(const std::string &name);

    //template <typename T>
    //static T *getInstance(ModuleDef module) {
    //    //return t
    //}
private:
    static void registerInstance(Module *instance);

    ModuleDef moduleDef;
    std::string name;
};

} // nebula