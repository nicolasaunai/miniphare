#ifndef EXPORTSTRATEGYFACTORY_H
#define EXPORTSTRATEGYFACTORY_H

#include <memory>

#include "exportstrategy.h"
#include "asciiexportstrategy.h"
#include "Diagnostics/exportstrategytypes.h"


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
