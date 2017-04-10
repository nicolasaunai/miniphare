#ifndef EXPORTSTRATEGYFACTORY_H
#define EXPORTSTRATEGYFACTORY_H

#include <memory>

#include "Diagnostics/Export/exportstrategytypes.h"
#include "Diagnostics/Export/exportstrategy.h"
#include "Diagnostics/Export/ASCII/asciiexportstrategy.h"


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
