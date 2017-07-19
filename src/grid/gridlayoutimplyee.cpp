#include <stdexcept>
#include <tuple>

#include "gridlayoutimplyee.h"




GridLayoutImplYee::GridLayoutImplYee(uint32 nbDims, Point origin, uint32 interpOrder,
                                     std::array<uint32, 3> nbrCellsXYZ,
                                     std::array<double, 3> dxdydz)
    : GridLayoutImplInternals(nbDims, origin, interpOrder, nbrCellsXYZ, dxdydz)
{
    gridDataT gridData{};

    initLayoutCentering_(gridData);

    // all methods MUST BE CALLED AFTER initLayoutCentering()
    // because they USE data in hybridQtycentering_
    initPhysicalStart(gridData);
    initPhysicalEnd(gridData);
    initGhostEnd(gridData);
    initLinearCombinations_();
}


void GridLayoutImplYee::initLinearCombinations_()
{
    // cf https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/Ohm
    // for how to calculate coefficients and shift indexes.

    int dualToPrimal;
    int primalTodual;

    if (interpOrder_ == 1 || interpOrder_ == 2 || interpOrder_ == 4)
    {
        dualToPrimal = -1;
        primalTodual = 1;
    }
    else if (interpOrder_ == 3)
    {
        dualToPrimal = 1;
        primalTodual = -1;
    }
    else
    {
        throw std::runtime_error("GridLayout Yee cannot be initialized: wrong interpolation order");
    }


    WeightPoint P1;
    WeightPoint P2;

    // moment to Ex is Ppp to Dpp
    // shift only in X
    // the average is done for all simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = 0.5;
    P2.ix   = primalTodual;
    P2.iy   = 0;
    P2.iz   = 0;
    P2.coef = 0.5;
    momentsToEx_.push_back(P1);
    momentsToEx_.push_back(P2);


    // moment to Ey is pPp to pDp
    // shift only in Y
    // the average is done only for 2D and 3D simulation
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ >= 2) ? 0.5 : 1.;
    momentsToEy_.push_back(P1);

    // in 2 and 3D, add another point and average
    if (nbdims_ >= 2)
    {
        P2.ix   = 0;
        P2.iy   = primalTodual;
        P2.iz   = 0;
        P2.coef = 0.5;
        momentsToEy_.push_back(P2);
    }


    // moment to Ez is ppP to ppD
    // shift only in Z
    // the average is done only for 3D simulation
    // hence for 1D and 2D runs coef==1
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ == 3) ? 0.5 : 1;
    momentsToEz_.push_back(P1);

    if (nbdims_ == 3)
    {
        P2.ix   = 0;
        P2.iy   = 0;
        P2.iz   = primalTodual;
        P2.coef = 0.5;
        momentsToEz_.push_back(P2);
    }




    // Bx to Ey is pdD to pdP
    // shift only in Z
    // the average is done only for 3D simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ == 3) ? 0.5 : 1;
    BxToEy_.push_back(P1);

    if (nbdims_ == 3)
    {
        P2.ix   = 0;
        P2.iy   = 0;
        P2.iz   = dualToPrimal;
        P2.coef = 0.5;
        BxToEy_.push_back(P2);
    }



    // Bx to Ez is pDd to pPd
    // shift in the Y direction only
    // the average is done for 2D and 3D simulations
    // hence for 1D simulations coef is 1
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ >= 2) ? 0.5 : 1;
    BxToEz_.push_back(P1);

    if (nbdims_ >= 2)
    {
        P2.ix   = 0;
        P2.iy   = dualToPrimal;
        P2.iz   = 0;
        P2.coef = 0.5;
        BxToEz_.push_back(P2);
    }


    // By to Ex is dpD to dpP
    // shift only in the Z direction
    // averaging is done only for 3D simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ == 3) ? 0.5 : 1;
    ByToEx_.push_back(P1);

    if (nbdims_ == 3)
    {
        P2.ix   = 0;
        P2.iy   = 0;
        P2.iz   = dualToPrimal;
        P2.coef = 0.5;
        ByToEx_.push_back(P2);
    }

    // By to Ez is Dpd to Ppd
    // shift only in the X direction
    // the averaging is done in all simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = 0.5;
    P2.ix   = dualToPrimal;
    P2.iy   = 0;
    P2.iz   = 0;
    P2.coef = 0.5;
    ByToEz_.push_back(P1);
    ByToEz_.push_back(P2);


    // Bz to Ex is dDp to dPp
    // shift only in the Y direction
    // the averaging is done for 2D and 3D simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ >= 2) ? 0.5 : 1;
    BzToEx_.push_back(P1);

    if (nbdims_ >= 2)
    {
        P2.ix   = 0;
        P2.iy   = dualToPrimal;
        P2.iz   = 0;
        P2.coef = 0.5;
        BzToEx_.push_back(P2);
    }


    // Bz to Ey is Ddp to Pdp
    // shift only in the X direction
    // the averaging is done for all simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = 0.5;
    BzToEy_.push_back(P1);
    P2.ix   = dualToPrimal;
    P2.iy   = 0;
    P2.iz   = 0;
    P2.coef = 0.5;
    BzToEy_.push_back(P2);


    // Ex to Moment is Dpp to Ppp
    // shift only in the X direction
    // the averaging is done for all simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = 0.5;
    ExToMoment_.push_back(P1);
    P2.ix   = dualToPrimal;
    P2.iy   = 0;
    P2.iz   = 0;
    P2.coef = 0.5;
    ExToMoment_.push_back(P2);


    // Ey to Moment is pDp to PPP
    // shift tis only in the Y direction
    // the averaging is done for 2D and 3D simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ >= 2) ? 0.5 : 1;
    EyToMoment_.push_back(P1);

    if (nbdims_ >= 2)
    {
        P2.ix   = 0;
        P2.iy   = dualToPrimal;
        P2.iz   = 0;
        P2.coef = 0.5;
        EyToMoment_.push_back(P2);
    }


    // Ez to Moment is ppD on ppP
    // shift only in the Z direction
    // the averaging is only for 3D simulations
    P1.ix   = 0;
    P1.iy   = 0;
    P1.iz   = 0;
    P1.coef = (nbdims_ == 3) ? 0.5 : 1;
    EzToMoment_.push_back(P1);

    if (nbdims_ == 3)
    {
        P2.ix   = 0;
        P2.iy   = 0;
        P2.iz   = dualToPrimal;
        P2.coef = 0.5;
        EzToMoment_.push_back(P2);
    }
}



/**
 * @brief GridLayoutImplYee::initLayoutCentering_ initialize the table
 * hybridQuantityCentering_. This is THE important array in the GridLayout
 * module. This table knows which quantity is primal/dual along each direction.
 * It is **this** array that **defines** what a Yee Layout is.
 * Once this array is defined, the rest of the GridLayout
 * needs this array OK and can go on from here... hence all other functions
 * in the Yee interface are just calling private implementation common
 * to all layouts
 */
void GridLayoutImplYee::initLayoutCentering_(const gridDataT& data)
{
    hybridQtyCentering_[data.iBx][data.idirX] = data.primal;
    hybridQtyCentering_[data.iBx][data.idirY] = data.dual;
    hybridQtyCentering_[data.iBx][data.idirZ] = data.dual;
    hybridQtyCentering_[data.iBy][data.idirX] = data.dual;
    hybridQtyCentering_[data.iBy][data.idirY] = data.primal;
    hybridQtyCentering_[data.iBy][data.idirZ] = data.dual;
    hybridQtyCentering_[data.iBz][data.idirX] = data.dual;
    hybridQtyCentering_[data.iBz][data.idirY] = data.dual;
    hybridQtyCentering_[data.iBz][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iEx][data.idirX] = data.dual;
    hybridQtyCentering_[data.iEx][data.idirY] = data.primal;
    hybridQtyCentering_[data.iEx][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iEy][data.idirX] = data.primal;
    hybridQtyCentering_[data.iEy][data.idirY] = data.dual;
    hybridQtyCentering_[data.iEy][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iEz][data.idirX] = data.primal;
    hybridQtyCentering_[data.iEz][data.idirY] = data.primal;
    hybridQtyCentering_[data.iEz][data.idirZ] = data.dual;

    hybridQtyCentering_[data.iJx][data.idirX] = data.dual;
    hybridQtyCentering_[data.iJx][data.idirY] = data.primal;
    hybridQtyCentering_[data.iJx][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iJy][data.idirX] = data.primal;
    hybridQtyCentering_[data.iJy][data.idirY] = data.dual;
    hybridQtyCentering_[data.iJy][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iJz][data.idirX] = data.primal;
    hybridQtyCentering_[data.iJz][data.idirY] = data.primal;
    hybridQtyCentering_[data.iJz][data.idirZ] = data.dual;

    hybridQtyCentering_[data.irho][data.idirX] = data.primal;
    hybridQtyCentering_[data.irho][data.idirY] = data.primal;
    hybridQtyCentering_[data.irho][data.idirZ] = data.primal;
    hybridQtyCentering_[data.iV][data.idirX]   = data.primal;
    hybridQtyCentering_[data.iV][data.idirY]   = data.primal;
    hybridQtyCentering_[data.iV][data.idirZ]   = data.primal;
    hybridQtyCentering_[data.iP][data.idirX]   = data.primal;
    hybridQtyCentering_[data.iP][data.idirY]   = data.primal;
    hybridQtyCentering_[data.iP][data.idirZ]   = data.primal;
}




AllocSizeT GridLayoutImplYee::allocSize(HybridQuantity qty) const
{
    return allocSize_(qty);
}




// TODO : WARNING 1st order only
// Can it be moved to ImplInternals?
AllocSizeT GridLayoutImplYee::allocSizeDerived(HybridQuantity qty, Direction dir) const
{
    return allocSizeDerived_(qty, dir);
}




// start and end index used in computing loops
uint32 GridLayoutImplYee::physicalStartIndex(QtyCentering centering, Direction direction) const
{
    return physicalStartIndex_(centering, direction);
}


uint32 GridLayoutImplYee::physicalStartIndex(HybridQuantity const& hybridQuantity,
                                             Direction direction) const
{
    return physicalStartIndex_(hybridQuantity, direction);
}

uint32 GridLayoutImplYee::physicalStartIndex(Field const& field, Direction direction) const
{
    return physicalStartIndex_(field, direction);
}


uint32 GridLayoutImplYee::physicalEndIndex(QtyCentering centering, Direction direction) const
{
    return physicalEndIndex_(centering, direction);
}


uint32 GridLayoutImplYee::physicalEndIndex(HybridQuantity const& hybridQuantity,
                                           Direction direction) const
{
    return physicalEndIndex_(hybridQuantity, direction);
}

uint32 GridLayoutImplYee::physicalEndIndex(Field const& field, Direction direction) const
{
    return physicalEndIndex_(field, direction);
}



uint32 GridLayoutImplYee::ghostStartIndex(QtyCentering centering, Direction direction) const
{
    // should we directly return 0 and remove ghostStartIndex_ ?
    return ghostStartIndex_(centering, direction);
}

uint32 GridLayoutImplYee::ghostStartIndex(HybridQuantity const& hybridQuantity,
                                          Direction direction) const
{
    // should we directly return 0 and remove ghostStartIndex_ ?
    return ghostStartIndex_(hybridQuantity, direction);
}

uint32 GridLayoutImplYee::ghostStartIndex(Field const& field, Direction direction) const
{
    // should we directly return 0 and remove ghostStartIndex_ ?
    return ghostStartIndex_(field, direction);
}



uint32 GridLayoutImplYee::ghostEndIndex(QtyCentering centering, Direction direction) const
{
    return ghostEndIndex_(centering, direction);
}

uint32 GridLayoutImplYee::ghostEndIndex(HybridQuantity const& hybridQuantity,
                                        Direction direction) const
{
    return ghostEndIndex_(hybridQuantity, direction);
}

uint32 GridLayoutImplYee::ghostEndIndex(Field const& field, Direction direction) const
{
    return ghostEndIndex_(field, direction);
}




Point GridLayoutImplYee::fieldNodeCoordinates(const Field& field, const Point& origin, uint32 ix,
                                              uint32 iy, uint32 iz) const
{
    return fieldNodeCoordinates_(field, origin, ix, iy, iz);
}




Point GridLayoutImplYee::cellCenteredCoordinates(uint32 ix, uint32 iy, uint32 iz) const
{
    return cellCenteredCoordinates_(ix, iy, iz);
}



QtyCentering GridLayoutImplYee::fieldCentering(Field const& field, Direction dir) const
{
    return fieldCentering_(field, dir);
}


uint32 GridLayoutImplYee::nbrGhostNodes(QtyCentering centering) const
{
    return nbrGhosts(centering);
}


std::array<uint32, NBR_COMPO> GridLayoutImplYee::nbrPhysicalNodes(Field const& field) const
{
    std::array<QtyCentering, NBR_COMPO> centerings
        = {{fieldCentering_(field, Direction::X), fieldCentering_(field, Direction::Y),
            fieldCentering_(field, Direction::Z)}};

    return physicalNodeNbrFromCentering_(centerings);
}


std::array<uint32, NBR_COMPO> GridLayoutImplYee::nbrPhysicalNodes(HybridQuantity hybQty) const
{
    QtyCentering centerX
        = hybridQtyCentering_[static_cast<uint32>(hybQty)][static_cast<uint32>(Direction::X)];
    QtyCentering centerY
        = hybridQtyCentering_[static_cast<uint32>(hybQty)][static_cast<uint32>(Direction::Y)];
    QtyCentering centerZ
        = hybridQtyCentering_[static_cast<uint32>(hybQty)][static_cast<uint32>(Direction::Z)];

    std::array<QtyCentering, NBR_COMPO> centerings = {{centerX, centerY, centerZ}};

    return physicalNodeNbrFromCentering_(centerings);
}




/**
 * @brief GridLayoutImplYee::deriv1D It was decided to compute the
 * derivative on the entire physical domain.
 * In the case of Maxwell Ampere (dual centering of the operand),
 * it will therefore be necessary to get the values
 * of the operand outside the physical domain before applying
 * Maxwell Ampere
 *
 * @param operand is always primal in the case of Maxwell Faraday
 * rotational operator (dEz/dy, dEy/dz, dEx/dz, dEz/dx, dEy/dx, dEx/dy)
 * operand is always dual in the case of Maxwell Ampere
 * rotational operator (dBz/dy, dBy/dz, dBx/dz, dBz/dx, dBy/dx, dBx/dy)
 *
 * @param derivative
 */
void GridLayoutImplYee::deriv1D(Field const& operand, Field& derivative) const
{
    deriv1D_(operand, derivative);
}




LinearCombination const& GridLayoutImplYee::momentsToEx() const
{
    return momentsToEx_;
}


LinearCombination const& GridLayoutImplYee::momentsToEy() const
{
    return momentsToEy_;
}


LinearCombination const& GridLayoutImplYee::momentsToEz() const
{
    return momentsToEz_;
}

LinearCombination const& GridLayoutImplYee::ByToEx() const
{
    return ByToEx_;
}

LinearCombination const& GridLayoutImplYee::ByToEz() const
{
    return ByToEz_;
}

LinearCombination const& GridLayoutImplYee::BxToEy() const
{
    return BxToEy_;
}

LinearCombination const& GridLayoutImplYee::BxToEz() const
{
    return BxToEz_;
}

LinearCombination const& GridLayoutImplYee::BzToEy() const
{
    return BzToEy_;
}

LinearCombination const& GridLayoutImplYee::BzToEx() const
{
    return BzToEx_;
}



LinearCombination const& GridLayoutImplYee::ExToMoment() const
{
    return ExToMoment_;
}


LinearCombination const& GridLayoutImplYee::EyToMoment() const
{
    return EyToMoment_;
}


LinearCombination const& GridLayoutImplYee::EzToMoment() const
{
    return EzToMoment_;
}
