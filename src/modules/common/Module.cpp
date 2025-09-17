
#include "Module.hpp"

#include <unordered_map>

namespace nebula {

    typedef std::unordered_map<std::string, Module*> ModuleRegistry;

    ModuleRegistry *modRegistry = nullptr;

    ModuleRegistry &registryInstance() {
        if (!modRegistry) {
            modRegistry = new ModuleRegistry;
        }
        return *modRegistry;
    }

    void freeEmptyRegistry() {
        if (modRegistry && modRegistry->empty()) {
            delete modRegistry;
            modRegistry = nullptr;
        }
    }

    Module::Module(
        ModuleDef moduleDef, 
        const char *name
    ) 
    : moduleDef(moduleDef)
    , name(name) {
        registerInstance(this);
    }

    Module::~Module() {
        ModuleRegistry &registry = registryInstance();

        for(auto it = registry.begin(); it != registry.end(); ++it) {
            if (it->second == this) {
                registry.erase(it);
                break;
            }
        }

        freeEmptyRegistry();
    }

    ModuleDef Module::getModule() const {
        return moduleDef;
    }

    const char *Module::getName() const {
        return name.c_str();
    }



    //void Module::registerInstance(M)
}