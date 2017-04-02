
#include <string>

#include "asciiexportstrategy.h"


void AsciiExportStrategy::save(Diagnostic const& diag, Time const& timeManager)
{

    std::string const& name = diag.name();
    double time = timeManager.currentTime();

    // patchID_DiagID_Time.txt

   // std::vector<DiagData> const& data = diag.data();


    // open file

    // now write data
   // fprintf(fp, "", di);

    // close file
}
