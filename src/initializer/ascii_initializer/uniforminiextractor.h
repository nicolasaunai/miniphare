#ifndef UNIFORMINIEXTRACTOR_H
#define UNIFORMINIEXTRACTOR_H

#include "iniextractor.h"

/**
 * @brief INIData extractor for a UniformModel
 */
class UniformINIExtractor : public INIExtractor
{
public:
    UniformINIExtractor();

    virtual void initializeModel(INIData& iniData, InitModel* model) const override;

    virtual ~UniformINIExtractor() = default;
};

#endif // UNIFORMINIEXTRACTOR_H
