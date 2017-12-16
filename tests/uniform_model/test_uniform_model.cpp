#include <array>
#include <memory>
#include <vector>

#include <data/Plasmas/ionsinitializer.h>
#include <data/Plasmas/particleinitializer.h>
#include <data/Plasmas/particles.h>
#include <data/grid/gridlayout.h>
#include <initializer/fluidparticleinitializer.h>
#include <initializer/initmodel/uniform/uniform_model.h>
#include <utilities/Time/pharetime.h>
#include <utilities/types.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"


class TestUniformParticleLoading : public ::testing::Test
{
public:
    TestUniformParticleLoading()
        : layout({{0.1, 0., 0.}}, {{100000, 0, 0}}, 1, "yee", Point{0., 0., 0.}, 1.)
        , model{layout}
    {
        model.setB0(1., 0., 0.);
        model.setE0(0., 0., 0.);
        model.setNbrSpecies(1.);
        model.setV0(0., 0., 0., 0);
        model.setNbrParticlesPerCell(10, 0);
        model.setMass(1., 0);
        model.setCharges(1., 0);
        model.setDensity(1., 0);
    }

    void SetUp() {}

    void TearDown()
    {
        // code here will be called just after the test completes
        // ok to through exceptions from here if need be
    }

    void setBeta(double beta) { model.setBeta(beta, 0); }

    void setAnisotropy(double anisotropy) { model.setAnisotropy(anisotropy, 0); }

    void setBasis(Basis basis) { model.setBasis(basis, 0); }


    std::array<double, 3> get_vth(std::vector<Particle>& particles)
    {
        std::array<double, 3> vth;

        for (auto comp = 0u; comp < 3; ++comp)
        {
            auto vmean = std::accumulate(std::begin(particles), std::end(particles), 0.,
                                         [comp](double currentMean, Particle const& p) {
                                             return currentMean + p.v[comp];
                                         })
                         / particles.size();


            double sqv = std::accumulate(std::begin(particles), std::end(particles), 0.,
                                         [comp](double val, Particle& p) {
                                             return val + p.v[comp] * p.v[comp];
                                         })
                         / particles.size();


            vth[comp] = std::sqrt(sqv - vmean * vmean);
        }

        return vth;
    }


    ~TestUniformParticleLoading()
    {
        // cleanup any pending stuff, but no exceptions allowed
    }

    GridLayout layout;
    UniformModel model;
    // std::unique_ptr<ParticleInitializer> initializer;
};



TEST_F(TestUniformParticleLoading, anisotropicVth)
{
    auto aniso = 0.2;
    auto beta  = 1;

    setBeta(beta);
    setBasis(Basis::Magnetic);
    setAnisotropy(aniso); // Pperp/Ppara = 0.2

    auto initializers = model.particleInitializers();
    auto initializer  = std::move(initializers[0]);

    // trace P = 3/2*B^2*beta
    auto traceP          = 3. / 2.;
    auto expectedVthPara = std::sqrt(traceP / (1. + 2 * aniso));
    auto expectedVthPerp = std::sqrt(aniso * traceP / (1. + 2 * aniso));

    std::vector<Particle> particles;

    initializer->loadParticles(particles);
    std::cout << particles[0].v[0] << "\n";
    auto vth = get_vth(particles);
    std::cout << "Vth = (" << vth[0] << ", " << vth[1] << ", " << vth[2] << ")\n";
    std::cout << "ExpVth = (" << expectedVthPara << ", " << expectedVthPerp << ")\n";
    ASSERT_NEAR(expectedVthPara, vth[0], 0.01);
    ASSERT_NEAR(expectedVthPerp, vth[1], 0.01);
    ASSERT_NEAR(expectedVthPerp, vth[2], 0.01);
}




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
