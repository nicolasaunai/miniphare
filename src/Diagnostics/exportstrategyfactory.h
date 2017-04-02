#ifndef EXPORTSTRATEGYFACTORY_H
#define EXPORTSTRATEGYFACTORY_H

#include <memory>

#include "exportstrategy.h"
#include "asciiexportstrategy.h"

enum class ExportStrategyType {ASCII /*, HDF5NATIVE, OPENPMD*/ };


class ExportStrategyFactory
{
public:
    static std::unique_ptr<ExportStrategy> makeExportStrategy(ExportStrategyType type)
    {
        if (type == ExportStrategyType::ASCII)
        {
            return std::unique_ptr<ExportStrategy> {new AsciiExportStrategy{} };
        }

        return nullptr;
    }
};



#endif // EXPORTSTRATEGYFACTORY_H
