
#include "uniforminiextractor.h"
#include "initializer/initmodel/uniform/uniform_model.h"
#include "utilities/types.h"

UniformINIExtractor::UniformINIExtractor()
{
}



void UniformINIExtractor::initializeModel(INIData& iniData, InitModel* model) const
{
    if (UniformModel* uniformModel = static_cast<UniformModel*>(model))
    {
        auto nbrSpecies = iniData.nbrSpecies;

        double bx = iniData.reader.GetReal("model", "bx", 1);
        double by = iniData.reader.GetReal("model", "by", 0);
        double bz = iniData.reader.GetReal("model", "bz", 0);

        uniformModel->setB0(bx, by, bz);

        double ex = iniData.reader.GetReal("model", "ex", 0);
        double ey = iniData.reader.GetReal("model", "ey", 0);
        double ez = iniData.reader.GetReal("model", "ez", 0);

        uniformModel->setE0(ex, ey, ez);

        uniformModel->setNbrSpecies(nbrSpecies);

        for (auto speciesIndex = 0u; speciesIndex < nbrSpecies; ++speciesIndex)
        {
            auto indexStr = std::to_string(speciesIndex);

            double n = iniData.reader.GetReal("model", "density" + indexStr, 1.);

            uniformModel->setDensity(n, speciesIndex);

            double vx = iniData.reader.GetReal("model", "vx" + indexStr, 1.);
            double vy = iniData.reader.GetReal("model", "vy" + indexStr, 1.);
            double vz = iniData.reader.GetReal("model", "vz" + indexStr, 1.);

            uniformModel->setV0(vx, vy, vz, speciesIndex);

            double beta = iniData.reader.GetReal("model", "betaRatio" + indexStr, 1.);

            uniformModel->setBeta(beta, speciesIndex);

            double aniso = iniData.reader.GetReal("model", "anisotropy" + indexStr, 1.);
            uniformModel->setAnisotropy(aniso, speciesIndex);

            auto basisStr = iniData.reader.Get("model", "basis" + indexStr, "magnetic");
            auto basis    = (basisStr == "magnetic") ? Basis::Magnetic : Basis::Cartesian;
            uniformModel->setBasis(basis, speciesIndex);

            auto mass = iniData.reader.GetReal("model", "mass" + indexStr, 1);
            uniformModel->setMass(mass, speciesIndex);

            auto charge = iniData.reader.GetReal("model", "charge" + indexStr, 1);
            uniformModel->setCharges(charge, speciesIndex);

            auto nbrParticles
                = iniData.reader.GetInteger("model", "nbrParticlesPerCell" + indexStr, 1);

            uniformModel->setNbrParticlesPerCell(static_cast<uint32>(nbrParticles), speciesIndex);
        }
    }
}
