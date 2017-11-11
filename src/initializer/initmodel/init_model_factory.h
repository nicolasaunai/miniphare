#ifndef INIT_MODEL_FACTORY_H
#define INIT_MODEL_FACTORY_H


#include <memory>
#include <string>

#include "initializer/initmodel/uniform/uniform_model.h"
#include "initmodel.h"
#include <data/grid/gridlayout.h>

/**
 * @brief factory creating concrete InitModel objects depending on modelName
 */
class InitModelFactory
{
public:
    static std::unique_ptr<InitModel> createInitModel(std::string const& modelName,
                                                      GridLayout const& layout)
    {
        return std::unique_ptr<InitModel>(new UniformModel{layout});
    }
};

#endif // INIT_MODEL_FACTORY_H
