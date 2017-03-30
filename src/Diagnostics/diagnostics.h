#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "types.h"
#include "Time/time.h"
#include "Field/field.h"
#include "utilityphare.h"
#include "amr/patchdata.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "Electromag/electromag.h"


// all data that will ever be written by PHARE
// will have to be put in there.
// this is the most general form of data we
// can use, that enable ExportStrategy to write
// any kind of concrete Diagnostic
// and therefore be implemented as a Bridge pattern

// this can take field data (depends=x, y, z; data= Ex, Ey, Ez)
// or particle data (depends = id_particle; data = x,y,z,vx,vy,vz)
// or 1 orbit data (depends=time; data=x,y,z)
// etc.
struct DiagPack
{
    std::unordered_map<std::string, std::vector<float>>  depends;
    std::unordered_map<std::string, std::vector<float>>  data;
    std::unordered_map<std::string, std::array<uint32,3>> nbrNodes;
};


enum class DiagType {EM, Ni};



// that's the standard Diagnostic interface
// all diagnostics must respect this
// this interface is used by the DiagnosticManager
class Diagnostic
{
protected:
    std::string name_;
    std::vector<DiagPack> diagPack_; // one diagData per patch
    //GridLayout layout_;

public:
    Diagnostic(std::string name)
        :name_{name}{}

    std::string const& name() const {return name_;}

    std::vector<DiagPack> const& data() const
    {
        std::cout << "getting Diganostic data" << std::endl;
        return diagPack_;
    }


    virtual void compute(PatchData const& patchData, GridLayout const& layout) = 0;



    virtual ~Diagnostic() = default;

};



class FieldDiagnostic : public Diagnostic
{

protected:



    void fillDiagData1D_(Field const& field, GridLayout const& layout, DiagPack& pack)
    {
        // this function will add an element to the container of DiagData
        // typically here this will be the data corresponding to a patch
        uint32 iStart   = layout.physicalStartIndex(field, Direction::X);
        uint32 iEnd     = layout.physicalEndIndex(field, Direction::X);
        Point origin    = layout.origin();

        for (uint32 ix=iStart; ix <= iEnd;  ++ix )
        {
            Point pos = layout.fieldNodeCoordinates(field, origin, ix, 0, 0);
            pack.data[ field.name() ][ix] = static_cast<float>( field(ix) );
            pack.depends["x_" + field.name()][ix] = static_cast<float>(pos.x_);
        }
    }



    void fillDiagData2D_(Field const& field, GridLayout const& layout, DiagPack& pack)
    {
        (void)field;
        throw std::runtime_error("Not Implemented");
    }


    void fillDiagData3D_(Field const& field, GridLayout const& layout, DiagPack& pack)
    {
        (void)field;
        throw std::runtime_error("Not Implemented");
    }



    void addVecField_(VecField const& vecField,
                      GridLayout const& layout,
                      std::array<HybridQuantity,3> qty)
    {
        std::array<uint32, 3> nbrNodesFx = layout.nbrPhysicalNodes(qty[0]);
        std::array<uint32, 3> nbrNodesFy = layout.nbrPhysicalNodes(qty[1]);
        std::array<uint32, 3> nbrNodesFz = layout.nbrPhysicalNodes(qty[2]);

        Field const& fx = vecField.component(VecField::VecX);
        Field const& fy = vecField.component(VecField::VecY);
        Field const& fz = vecField.component(VecField::VecZ);

        DiagPack pack;
        pack.depends.insert( {"x_" + fx.name(), std::vector<float>(nbrNodesFx[0])} );
        pack.depends.insert( {"x_" + fy.name(), std::vector<float>(nbrNodesFy[0])} );
        pack.depends.insert( {"x_" + fz.name(), std::vector<float>(nbrNodesFz[0])} );

        uint64 totalSizeEx = nbrNodesFx[0]*nbrNodesFx[1]*nbrNodesFx[2];
        uint64 totalSizeEy = nbrNodesFy[0]*nbrNodesFy[1]*nbrNodesFy[2];
        uint64 totalSizeEz = nbrNodesFz[0]*nbrNodesFz[1]*nbrNodesFz[2];

        pack.data.insert( {fx.name(), std::vector<float>(totalSizeEx)} );
        pack.data.insert( {fy.name(), std::vector<float>(totalSizeEy)} );
        pack.data.insert( {fz.name(), std::vector<float>(totalSizeEz)} );

        pack.nbrNodes.insert( {"n_" + fx.name(), std::array<uint32,3>{ nbrNodesFx } } );
        pack.nbrNodes.insert( {"n_" + fy.name(), std::array<uint32,3>{ nbrNodesFy } } );
        pack.nbrNodes.insert( {"n_" + fz.name(), std::array<uint32,3>{ nbrNodesFz } } );

        if (layout.nbDimensions() >= 2)
        {
            pack.depends.insert( {"y_" + fx.name(), std::vector<float>(nbrNodesFx[1])} );
            pack.depends.insert( {"y_" + fy.name(), std::vector<float>(nbrNodesFy[1])} );
            pack.depends.insert( {"y_" + fz.name(), std::vector<float>(nbrNodesFz[1])} );
        }

        if (layout.nbDimensions() == 3)
        {
            pack.depends.insert( {"z_" + fx.name(), std::vector<float>(nbrNodesFx[2])} );
            pack.depends.insert( {"z_" + fy.name(), std::vector<float>(nbrNodesFy[2])} );
            pack.depends.insert( {"z_" + fz.name(), std::vector<float>(nbrNodesFz[2])} );
        }


        switch (layout.nbDimensions() )
        {
            case 1:
            fillDiagData1D_(vecField.component(0), layout, pack);
            fillDiagData1D_(vecField.component(1), layout, pack);
            fillDiagData1D_(vecField.component(2), layout, pack);
            break;

            case 2:
            fillDiagData2D_(vecField.component(0), layout, pack);
            fillDiagData2D_(vecField.component(1), layout, pack);
            fillDiagData2D_(vecField.component(2), layout, pack);
            break;

            case 3:
            fillDiagData3D_(vecField.component(0), layout, pack);
            fillDiagData3D_(vecField.component(1), layout, pack);
            fillDiagData3D_(vecField.component(2), layout, pack);
            break;
        }


        diagPack_.push_back( std::move(pack) );

    }

    FieldDiagnostic(std::string diagName)
        : Diagnostic{diagName}
    {}

};






// an example of diagnostic for the electromagnetic field
// the class implement compute() and data()
// compute() will put EM data as a DiagData
// data() return the reference to this DiagData
class ElectromagDiagnostic : public FieldDiagnostic
{
private:
    std::array<HybridQuantity, 3> Eqty_;
    std::array<HybridQuantity, 3> Bqty_;

public:

    // somehow we need to initialize data_
    //
    ElectromagDiagnostic()
        : FieldDiagnostic{"EM"},
          Eqty_{ {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
          Bqty_{ {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} }
    { }




    virtual void compute(PatchData const& patchData , GridLayout const& layout) final
    {
        std::cout << "computing EM diags" << std::endl;
        Electromag const& em = patchData.EMfields();

        addVecField_(em.getE(), layout, Eqty_);
        addVecField_(em.getB(), layout, Bqty_);
    }


    virtual ~ElectromagDiagnostic() = default;
};







// interface used to write data on disk
// it is used by the DiagnosticManager, which does
// not know which concrete strategy is used to write data on disk
// all concrete ExportStrategy will implement the save() method
// which will take, for a diagnostic 'diag' a ref to a standard DiagData
// structure and know how to write it in a concrete file format
// this is implemented as a bridge pattern
class ExportStrategy
{
private:

public:
    virtual void save(Diagnostic const& diag, Time const& timeManager) = 0;

};




// a concrete ExportStrategy is to write Ascii files
// here we will write one file per time step and per diagnostic
// this is the simplest and dumbest we can do
// any smarter ascii format will just be a pain in the ass to read
// this is mainly for 1D debug stuff... not for real usage
class AsciiExportStrategy : public ExportStrategy
{
public:


    virtual void save(Diagnostic const& diag, Time const& timeManager) final
    {
        std::string const& name = diag.name();
        double time = timeManager.currentTime();
       // std::vector<DiagData> const& data = diag.data();


        // open file

        // now write data
       // fprintf(fp, "", di);

        // close file

    }
};




// A concrete ExportStrategy that writes Diagnostic objects
// into an HDF5 file
// the save() routine here will take a Diagnostic and a Time object
// from that it will be able to create a group, dataset etc. compliant to
// the specific format we've chosen.
class HDF5ExportStrategy : public ExportStrategy
{

};


enum class ExportStrategyType {ASCII, HDF5NATIVE, OPENPMD};


class ExportStrategyFactory
{
public:
    static std::unique_ptr<ExportStrategy> makeExportStrategy(ExportStrategyType type)
    {
        if (type == ExportStrategyType::ASCII)
        {
            return std::unique_ptr<ExportStrategy> {new AsciiExportStrategy{} };
        }

        return nullptr;
    }
};





// trick seen here
// http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
// to use an enum class as a key in unordered_map
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using DiagUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;









// this object is in charge of saying TRUE or FALSE if a particular
// diagnostic is to be computed or written.
// each of the diagnostics in the DiagnosticManager will have to
// register its compute and dump iteration numbers to the scheduler
// these vectors will at some point come from the initialization factory
// this object is manipulated by a DiagnosticManager
class DiagnosticScheduler
{
private:

    //std::unordered_map< uint32, std::vector<uint32> > computingIterations_;
    //std::unordered_map< uint32, std::vector<uint32> > writingIterations_;

    DiagUnorderedMap<DiagType, std::vector<uint32> > computingIterations_;
    DiagUnorderedMap<DiagType, std::vector<uint32> > writingIterations_;

public:

    DiagnosticScheduler() = default;

    void registerDiagnostic(DiagType type,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations)
    {
        computingIterations_.insert({type, computingIterations}  );
        writingIterations_.insert(  {type, writingIterations} );
    }


    bool timeToWrite(Time const& timeManager, DiagType type)
    {
        return writingIterations_[type][timeManager.currentIteration()];
    }


    bool timeToCompute(Time const& timeManager, DiagType type)
    {
        return computingIterations_[type][timeManager.currentIteration()];
    }

};




class DiagnosticFactory
{
public:

    // for now no other option than the type.
    static std::unique_ptr<Diagnostic> makeDiagnostic(DiagType type)
    {
        if (type == DiagType::EM )
        {
            return std::unique_ptr<Diagnostic> {new ElectromagDiagnostic{} };
        }

        return nullptr;
    }
};






/**
 * @brief encapsulate all diagnostics (fields, particle, globals, etc.)
 */

class DiagnosticsManager
{
private:
    DiagUnorderedMap< DiagType, std::shared_ptr<Diagnostic> > diags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:


    // hard coded for now in the initialization list
    // will have to have a add_diag() function at some point
    // this will come from the factory..
    DiagnosticsManager(ExportStrategyType exportType)
        : diags_{},
          exportStrat_{ExportStrategyFactory::makeExportStrategy(exportType)},
          scheduler_{}
    {

    }


    void newDiagnostic(DiagType type,
                       std::vector<uint32> const& computingIterations,
                       std::vector<uint32> const& writingIterations)
    {
        // create a new diagnostic of type 'type'
        std::shared_ptr<Diagnostic> newDiag = DiagnosticFactory::makeDiagnostic(type);
        diags_.insert( {type, newDiag} );

        // register to the scheduler
        scheduler_.registerDiagnostic(type, computingIterations, writingIterations);
    }



    void compute(Time const& timeManager, PatchData const& patchData, GridLayout const& layout)
    {
        for (auto const& diagPair : diags_)
        {
            DiagType type    =  diagPair.first;
            Diagnostic& diag = *diagPair.second;

            if (scheduler_.timeToCompute(timeManager, type) )
            {
                diag.compute(patchData, layout);
            }
        }
    }


    void save(Time const& timeManager)
    {
        for (auto const& diagPair : diags_)
        {
            exportStrat_->save(*diagPair.second, timeManager);
        }
    }

    ~DiagnosticsManager() = default;
};








#endif // DIAGNOSTICS_H
