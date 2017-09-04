

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "test_commons.h"

#include "test_pusher1d.h"

#include "Plasmas/particles.h"

#include "Interpolator/interpolator.h"

#include "pusher/modifiedboris.h"
#include "pusher/pusher.h"
#include "pusher/pusherfactory.h"

#include "BoundaryConditions/domainboundarycondition.h"
#include "BoundaryConditions/periodicdomainboundary.h"



void print(Push1dParams const& inputs);

std::unique_ptr<BoundaryCondition> createBoundary(GridLayout const& layout);

void allocEBVecFields(GridLayout const& layout, std::shared_ptr<VecField>& E_out,
                      std::shared_ptr<VecField>& B_out);

void initEVecField(VecField& E_out, double Ex, double Ey, double Ez);

void initBVecField(VecField& B_out, double Bx, double By, double Bz);

void readFieldsOnTheMesh(Push1dParams const& inputs, GridLayout const& layout, VecField& field,
                         uint32 ik);


uint32 Push1dParams::testCaseNbr = 0;


class Pusher1DTest : public ::testing::TestWithParam<Push1dParams>
{
public:
    Push1dParams inputs;

    double precision_x;
    double precision_v;

    std::vector<double> expected_x_part;
    std::vector<double> expected_vx_part;
    std::vector<double> expected_vy_part;
    std::vector<double> expected_vz_part;

    std::vector<double> actual_x_part;
    std::vector<double> actual_vx_part;
    std::vector<double> actual_vy_part;
    std::vector<double> actual_vz_part;


    ~Pusher1DTest() = default;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs);
        Point origin;

        GridLayout layout{inputs.dxdydz,  inputs.nbrCells, inputs.nbDim,
                          inputs.lattice, origin,          inputs.interpOrder};


        std::string file_x{"../Pusher/odepush_x_testCase" + std::to_string(inputs.testId) + ".txt"};

        std::string file_vx{"../Pusher/odepush_vx_testCase" + std::to_string(inputs.testId)
                            + ".txt"};
        std::string file_vy{"../Pusher/odepush_vy_testCase" + std::to_string(inputs.testId)
                            + ".txt"};
        std::string file_vz{"../Pusher/odepush_vz_testCase" + std::to_string(inputs.testId)
                            + ".txt"};

        std::cout << file_x << std::endl;
        std::cout << file_vx << std::endl;
        std::cout << file_vy << std::endl;
        std::cout << file_vz << std::endl;

        std::ifstream ifs_x{file_x};
        if (!ifs_x)
        {
            std::cout << "Could not open file : " << file_x << std::endl;
            exit(-1);
        }
        std::ifstream ifs_vx{file_vx};
        if (!ifs_vx)
        {
            std::cout << "Could not open file : " << file_vx << std::endl;
            exit(-1);
        }
        std::ifstream ifs_vy{file_vy};
        if (!ifs_vy)
        {
            std::cout << "Could not open file : " << file_vy << std::endl;
            exit(-1);
        }
        std::ifstream ifs_vz{file_vz};
        if (!ifs_vz)
        {
            std::cout << "Could not open file : " << file_vz << std::endl;
            exit(-1);
        }

        expected_x_part.assign(inputs.nstep + 1, 0.);
        expected_vx_part.assign(inputs.nstep + 1, 0.);
        expected_vy_part.assign(inputs.nstep + 1, 0.);
        expected_vz_part.assign(inputs.nstep + 1, 0.);
        for (uint32 ik = 0; ik < inputs.nstep + 1; ++ik)
        {
            ifs_x >> expected_x_part[ik];
            ifs_vx >> expected_vx_part[ik];
            ifs_vy >> expected_vy_part[ik];
            ifs_vz >> expected_vz_part[ik];
        }

        auto allocBx = layout.allocSize(HybridQuantity::Bx);
        auto allocBy = layout.allocSize(HybridQuantity::By);
        auto allocBz = layout.allocSize(HybridQuantity::Bz);
        auto allocEx = layout.allocSize(HybridQuantity::Ex);
        auto allocEy = layout.allocSize(HybridQuantity::Ey);
        auto allocEz = layout.allocSize(HybridQuantity::Ez);

        VecField Bfields(allocBx, allocBy, allocBz,
                         {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}}, "B");

        VecField Efields(allocEx, allocEy, allocEz,
                         {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}}, "E");

        double dt = (inputs.tend - inputs.tbegin) / inputs.nstep;

        std::unique_ptr<Pusher> pusher = PusherFactory::createPusher(layout, "modifiedBoris", dt);

        // Initialize the Particle to be pushed
        double weight = 1.;
        double mass   = 1.;

        int32 icellx0 = static_cast<int32>(std::floor(inputs.x0 / layout.dx()));

        Particle partic(weight, inputs.q, {{icellx0, 0, 0}},
                        {{static_cast<float>(inputs.x0 - icellx0 * layout.dx()), 0., 0.}},
                        {{inputs.vx0, inputs.vy0, inputs.vz0}});

        double posx = (partic.icell[0] + static_cast<double>(partic.delta[0])) * layout.dx();

        actual_x_part.push_back(posx);

        actual_vx_part.push_back(partic.v[0]);
        actual_vy_part.push_back(partic.v[1]);
        actual_vz_part.push_back(partic.v[2]);

        // we compute the time step

        // We need an interpolator
        std::unique_ptr<Interpolator> interpolator{new Interpolator{layout.order()}};

        std::vector<Particle> particArray{partic};

        std::unique_ptr<BoundaryCondition> bc{createBoundary(layout)};


        precision_x = 6. * dt * dt;
        precision_v = 2.45 * dt; // sqrt(6.) = 2.45

        for (uint32 ik = 1; ik < inputs.nstep + 1; ++ik)
        {
            // reading fields on the mesh
            readFieldsOnTheMesh(inputs, layout, Bfields, ik);
            readFieldsOnTheMesh(inputs, layout, Efields, ik);

            pusher->move(particArray, particArray, mass, Efields, Bfields, *interpolator, *bc);

            Particle const& iPart = particArray[0];

            double posx = (iPart.icell[0] + static_cast<double>(iPart.delta[0])) * layout.dx();

            actual_x_part.push_back(posx);

            actual_vx_part.push_back(iPart.v[0]);
            actual_vy_part.push_back(iPart.v[1]);
            actual_vz_part.push_back(iPart.v[2]);
        }


        std::cout << "dt = " << dt << std::endl;
        std::cout << "precision_x = " << precision_x << std::endl;
        std::cout << "precision_v = " << precision_v << std::endl;

        printTable(actual_x_part, "actual_x_part");
        printTable(expected_x_part, "expected_x_part");
    }
};



std::unique_ptr<BoundaryCondition> createBoundary(GridLayout const& layout)
{
    // return hard coded domain periodic boundary condition
    std::vector<DomainBoundaryCondition::BoundaryInfo> boundaries(2);

    // "first" is the edge coordinate
    boundaries[0].first = Edge::Xmin;
    boundaries[1].first = Edge::Xmax;

    // "second" is the type of boundary, here periodic
    boundaries[0].second = BoundaryType::Periodic;
    boundaries[1].second = BoundaryType::Periodic;

    std::unique_ptr<BoundaryCondition> bc{new DomainBoundaryCondition{layout, boundaries}};

    return bc;
}


void allocEBVecFields(GridLayout const& layout, std::shared_ptr<VecField>& E_out,
                      std::shared_ptr<VecField>& B_out)
{
    auto allocEx = layout.allocSize(HybridQuantity::Ex);
    auto allocEy = layout.allocSize(HybridQuantity::Ey);
    auto allocEz = layout.allocSize(HybridQuantity::Ez);
    auto allocBx = layout.allocSize(HybridQuantity::Bx);
    auto allocBy = layout.allocSize(HybridQuantity::By);
    auto allocBz = layout.allocSize(HybridQuantity::Bz);

    E_out = std::make_shared<VecField>(
        VecField(allocEx, allocEy, allocEz,
                 {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}}, "E"));

    B_out = std::make_shared<VecField>(
        VecField(allocBx, allocBy, allocBz,
                 {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}}, "B"));
}


/**
 * @brief initEVecField
 * @param E_out is set using Ex, Ey, Ez
 * @param Ex uniform value for Field E_out.x_
 * @param Ey uniform value for Field E_out.y_
 * @param Ez uniform value for Field E_out.z_
 */
void initEVecField(VecField& E_out, double uniformEx, double uniformEy, double uniformEz)
{
    Field& Ex = E_out.component(VecField::VecX);
    Field& Ey = E_out.component(VecField::VecY);
    Field& Ez = E_out.component(VecField::VecZ);

    for (auto it = Ex.begin(); it < Ex.end(); ++it)
        *it = uniformEx;

    for (auto it = Ey.begin(); it < Ey.end(); ++it)
        *it = uniformEy;

    for (auto it = Ez.begin(); it < Ez.end(); ++it)
        *it = uniformEz;
}


/**
 * @brief initBVecField
 * @param B_out is set using Bx, By, Bz
 * @param Bx uniform value for Field B_out.x_
 * @param By uniform value for Field B_out.y_
 * @param Bz uniform value for Field B_out.z_
 */
void initBVecField(VecField& B_out, double uniformBx, double uniformBy, double uniformBz)
{
    Field& Bx = B_out.component(VecField::VecX);
    Field& By = B_out.component(VecField::VecY);
    Field& Bz = B_out.component(VecField::VecZ);

    for (auto it = Bx.begin(); it < Bx.end(); ++it)
        *it = uniformBx;

    for (auto it = By.begin(); it < By.end(); ++it)
        *it = uniformBy;

    for (auto it = Bz.begin(); it < Bz.end(); ++it)
        *it = uniformBz;
}



void readFieldsOnTheMesh(Push1dParams const& inputs, GridLayout const& layout, VecField& vecfield,
                         uint32 ik)
{
    for (uint32 ifield = 0; ifield < 3; ifield++)
    {
        Field& field = vecfield.component(ifield);

        uint32 iqty = static_cast<uint32>(field.hybridQty());

        //    odepush_Ex_t108_testCase3.txt
        std::string fileFields{"../Pusher/odepush_" + GetHybridQtyName(iqty) + "_t"
                               + std::to_string(ik) + "_testCase" + std::to_string(inputs.testId)
                               + ".txt"};

        // std::cout << fileFields << std::endl ;

        std::ifstream ifsFields{fileFields};
        if (!ifsFields)
        {
            std::cout << "Could not open file : " << fileFields << std::endl;
            exit(-1);
        }

        uint32 ix0 = layout.physicalStartIndex(field, Direction::X);
        uint32 ix1 = layout.physicalEndIndex(field, Direction::X);

        // Reading the field on the whole mesh
        for (uint32 ix = ix0; ix <= ix1; ++ix)
            ifsFields >> field(ix);
    }
}




void print(Push1dParams const& inputs)
{
    std::cout << "tbegin = " << inputs.tbegin << " tend  = " << inputs.tend
              << " nstep = " << inputs.nstep << "\n"
              << " q = " << inputs.q << "\t"
              << " m = " << inputs.m << "\n"
              << " x0 = " << inputs.x0 << "\n"
              << " y0 = " << inputs.y0 << "\n"
              << " z0 = " << inputs.z0 << "\n"
              << " vx0 = " << inputs.vx0 << "\n"
              << " vy0 = " << inputs.vy0 << "\n"
              << " vz0 = " << inputs.vz0 << "\n"
              << " dxdydz[0] = " << inputs.dxdydz[0] << " dxdydz[1] = " << inputs.dxdydz[1]
              << " dxdydz[2] = " << inputs.dxdydz[2] << "\n"
              << " nbrCells[0] = " << inputs.nbrCells[0] << " nbrCells[1] = " << inputs.nbrCells[1]
              << " nbrCells[2] = " << inputs.nbrCells[2] << "\n"
              << " nbDim = " << inputs.nbDim << " lattice = " << inputs.lattice
              << " interpOrder : " << inputs.interpOrder << std::endl;
}




/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(Pusher1DTest, pusher1Dx3Dv)
{
    // it might look dirty to have 4 EXPECT_TRUE
    // but DO NOT change this !!
    // or you will break the tests

    // uint32 testCaseNbr is static
    // splitting in 4 TEST_P results in testCaseNbr taking
    // values greater than the actual number of test cases available

    EXPECT_TRUE(AreVectorsEqual(expected_x_part, actual_x_part, precision_x));
    EXPECT_TRUE(AreVectorsEqual(expected_vx_part, actual_vx_part, precision_v));
    EXPECT_TRUE(AreVectorsEqual(expected_vy_part, actual_vy_part, precision_v));
    EXPECT_TRUE(AreVectorsEqual(expected_vz_part, actual_vz_part, precision_v));
}


INSTANTIATE_TEST_CASE_P(Pusher1DTests, Pusher1DTest, testing::ValuesIn(getPush1dParamsFromFile()));
