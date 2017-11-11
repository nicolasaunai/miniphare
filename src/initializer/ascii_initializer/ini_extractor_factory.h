#ifndef INI_EXTRACTOR_FACTORY_H
#define INI_EXTRACTOR_FACTORY_H

#include <memory>
#include <string>
#include "iniextractor.h"
#include "uniforminiextractor.h"

class INIExtractorFactory
{
    using INIExtractorPtr = std::unique_ptr<INIExtractor>;

public :

    static INIExtractorPtr createINIExtractor(std::string const& modelType)
    {
        if (modelType == "uniform")
        {
            return std::unique_ptr<INIExtractor>(new UniformINIExtractor{});
        }
    }
};



#endif // INI_EXTRACTOR_FACTORY_H
