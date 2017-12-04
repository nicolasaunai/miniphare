#ifndef INI_EXTRACTOR_FACTORY_H
#define INI_EXTRACTOR_FACTORY_H

#include "iniextractor.h"
#include "uniforminiextractor.h"
#include <memory>
#include <string>

class INIExtractorFactory
{
    using INIExtractorPtr = std::unique_ptr<INIExtractor>;

public:
    static INIExtractorPtr createINIExtractor(std::string const& modelType)
    {
        if (modelType == "uniform")
        {
            return std::unique_ptr<INIExtractor>(new UniformINIExtractor{});
        }
        return nullptr;
    }
};



#endif // INI_EXTRACTOR_FACTORY_H
