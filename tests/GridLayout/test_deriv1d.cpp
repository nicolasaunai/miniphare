
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>


#include "test_gridlayout.h"




struct DerivTestParams
{
    uint32 interpOrder;
    uint32 nbDim;

    HybridQuantity qty;
    uint32 iqty;
    std::string qtyName;

    HybridQuantity derivedQty;
    uint32 iderivedQty;
    std::string derivedQtyName;

    uint32 nbrCellsX;
    double dx;

    uint32 iGhostStartOperand;
    uint32 iGhostEndOperand;

    uint32 iPhysStartDerivative;
    uint32 iPhysEndDerivative;

    Point origin{0., 0., 0.};

    std::string functionName;

    std::vector<double> fieldXCoords;
    std::vector<double> fieldXValues;

    std::vector<double> derivedFieldXCoords;
    std::vector<double> derivedFieldXValues;


    DerivTestParams() = default;

    DerivTestParams(uint32 order, uint32 dim, uint32 hybridQty, uint32 nbrCells, double stepSize,
                    uint32 iGhostStart, uint32 iGhostEnd, uint32 iDerStart, uint32 iDerEnd,
                    std::array<double, 3> originPt, std::string fName)
        : interpOrder{order}
        , nbDim{dim}
        , iqty{hybridQty}
        , nbrCellsX{nbrCells}
        , dx{stepSize}
        , iGhostStartOperand{iGhostStart}
        , iGhostEndOperand{iGhostEnd}
        , iPhysStartDerivative{iDerStart}
        , iPhysEndDerivative{iDerEnd}
        , origin(originPt[0], originPt[1], originPt[2])
        , functionName{fName}
    {
    }
};




std::vector<DerivTestParams> getDerivInputsFromFile();




class GridLayoutDeriv1D : public ::testing::TestWithParam<DerivTestParams>
{
public:
    DerivTestParams inputs;

    double precision;

    std::vector<double> expected_array;
    std::vector<double> actual_array;

    void SetUp()
    {
        Point origin;
        inputs = GetParam();
        print(inputs);

        std::array<double, 3> dxdydz{{inputs.dx, 0., 0.}};
        std::array<uint32, 3> nbrCells{{inputs.nbrCellsX, 0, 0}};
        GridLayout gl{dxdydz, nbrCells, inputs.nbDim, "yee", origin, inputs.interpOrder};

        std::string qtyName = inputs.qtyName;

        std::string fctName = inputs.functionName;

        std::string filename{"../GridLayout/deriv1D_" + qtyName + "_" + fctName + ".txt"};

        std::cout << filename << std::endl;

        std::ifstream infile{filename};
        if (!infile)
        {
            std::cout << "Could not open file : " << filename << std::endl;
            exit(-1);
        }


        uint32 iStart   = inputs.iGhostStartOperand;
        uint32 iEnd     = inputs.iGhostEndOperand;
        uint32 nbrElems = iEnd - iStart + 1;

        for (uint32 ik = 0; ik < nbrElems; ++ik)
        {
            infile >> inputs.fieldXCoords[ik];
            infile >> inputs.fieldXValues[ik];
        }

        auto allocSizeOperand = gl.allocSize(inputs.qty);

        // inputs.qty is the important parameter
        Field operand{allocSizeOperand, inputs.qty, "operandField"};

        for (uint32 ik = iStart; ik <= iEnd; ++ik)
        {
            double x = inputs.fieldXCoords[ik];

            if (inputs.functionName == "linear")
            {
                operand(ik) = x;
            }
            else if (inputs.functionName == "square")
            {
                operand(ik) = x * x;
            }
            else if (inputs.functionName == "sin")
            {
                operand(ik) = sin(x);
            }
            else if (inputs.functionName == "compo01")
            {
                operand(ik) = x * x + sin(x);
            }
        }


        uint32 iDerStart       = inputs.iPhysStartDerivative;
        uint32 iDerEnd         = inputs.iPhysEndDerivative;
        uint32 nbrElemsCompare = iDerEnd - iDerStart + 1;

        for (uint32 ik = 0; ik < nbrElemsCompare; ++ik)
        {
            infile >> inputs.derivedFieldXCoords[ik];
            infile >> inputs.derivedFieldXValues[ik];
        }

        expected_array.assign(nbrElemsCompare, 0.);
        // We fill the expected array
        for (uint32 ix = 0; ix < nbrElemsCompare; ++ix)
        {
            expected_array[ix] = inputs.derivedFieldXValues[ix];
        }


        auto allocSizeDer = gl.allocSizeDerived(inputs.derivedQty, Direction::X);

        // inputs.qty is not the correct argument
        // this should not be relevant for the test
        Field derivative{allocSizeDer, inputs.derivedQty, "derivativeField"};


        // We now testing the GridLayout::Deriv method
        gl.deriv(operand, Direction::X, derivative);

        // this method has 2nd order precision
        precision = pow(gl.dx(), 2.);

        actual_array.assign(nbrElemsCompare, 0.);
        uint32 iPhysical = iDerStart;
        for (uint32 ix = 0; ix < nbrElemsCompare; ++ix)
        {
            actual_array[ix] = derivative(iPhysical);
            ++iPhysical;
        }
    }


    void print(DerivTestParams const& inputs)
    {
        std::cout << "interpOrder = " << inputs.interpOrder << " nbDim   = " << inputs.nbDim
                  << " qtyName  : " << inputs.qtyName
                  << " derivedQtyName  : " << inputs.derivedQtyName
                  << " nbrCells X : " << inputs.nbrCellsX << " dx   = " << inputs.dx
                  << " iGhostStartOperand = " << inputs.iGhostStartOperand
                  << " iGhostEndOperand   = " << inputs.iGhostEndOperand
                  << " iPhysStartDerivative = " << inputs.iPhysStartDerivative
                  << " iPhysEndDerivative   = " << inputs.iPhysEndDerivative
                  << " functionName = " << inputs.functionName << std::endl;
    }
};


/***********************************************************/
/* Testing Bx is enough                                    */
/* Our only goal here is to test the derivative operator   */
/*                                                         */
/* The numerically computed derivative is accurate to the  */
/* 2nd order                                               */
/*                                                         */
/***********************************************************/
TEST_P(GridLayoutDeriv1D, DerivedField)
{
    EXPECT_THAT(actual_array, ::testing::Pointwise(DoubleNear(precision), expected_array));
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutDeriv1D,
                        testing::ValuesIn(getDerivInputsFromFile()));




std::vector<DerivTestParams> getDerivInputsFromFile()
{
    std::string filename{"../GridLayout/deriv1D_summary.txt"};

    std::ifstream ifs1{filename};
    if (!ifs1)
    {
        std::cout << "Could not open file : " << filename << std::endl;
        exit(-1);
    }

    uint32 nbrTestCases = 0;
    ifs1 >> nbrTestCases;

    std::vector<DerivTestParams> params(nbrTestCases);

    // reading parameters relative to the test cases
    for (uint32 i = 0; i < nbrTestCases; ++i)
    {
        uint32 iqty, iderivedQty;

        ifs1 >> params[i].interpOrder >> params[i].nbDim >> iqty >> iderivedQty;

        for (uint32 idim = 0; idim < params[i].nbDim; ++idim)
        {
            ifs1 >> params[i].nbrCellsX;
            ifs1 >> params[i].dx;
        }

        ifs1 >> params[i].iGhostStartOperand;
        ifs1 >> params[i].iGhostEndOperand;
        ifs1 >> params[i].iPhysStartDerivative;
        ifs1 >> params[i].iPhysEndDerivative;
        ifs1 >> params[i].origin.x;
        ifs1 >> params[i].origin.y;
        ifs1 >> params[i].origin.z;
        ifs1 >> params[i].functionName;

        params[i].qty     = GetHybridQty(iqty);
        params[i].qtyName = GetHybridQtyName(iqty);
        params[i].iqty    = iqty;

        params[i].derivedQty     = GetHybridQty(iderivedQty);
        params[i].derivedQtyName = GetHybridQtyName(iderivedQty);
        params[i].iderivedQty    = iderivedQty;

        params[i].fieldXCoords.assign(MAX_SIZE, 0.);
        params[i].fieldXValues.assign(MAX_SIZE, 0.);
        params[i].derivedFieldXCoords.assign(MAX_SIZE, 0.);
        params[i].derivedFieldXValues.assign(MAX_SIZE, 0.);
    }

    return params;
}
