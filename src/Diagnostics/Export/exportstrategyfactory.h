#ifndef EXPORTSTRATEGYFACTORY_H
#define EXPORTSTRATEGYFACTORY_H

#include <memory>

#include "diagnostics/Export/ASCII/asciiexportstrategy.h"
#include "diagnostics/Export/exportstrategy.h"
#include "diagnostics/Export/exportstrategytypes.h"


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
