#ifndef INIEXTRACTOR_H
#define INIEXTRACTOR_H


#include "inidata.h"
#include <initializer/initmodel/initmodel.h>

/**
 * @brief Interface to initialize an InitModel from an INI file. Any model must
 * come with a class implementing this interface.
 */
class INIExtractor
{
public:
    /** set parameters of specific InitModel objects from an INIData object
        concrete class take specific InitModel and set their parameters
        by browsing the INIData */
    virtual void initializeModel(INIData& iniData, InitModel* model) const = 0;

    virtual ~INIExtractor() = 0;
};

#endif // INIEXTRACTOR_H
