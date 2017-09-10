#ifndef PATCH_H
#define PATCH_H


// data structure Tree

#include "AMR/patchdata.h"
#include "box.h"




/**
 * @brief Represents a "node" in the patch Hierarchy.
 *
 * Patch holds a pointer to its (single) parent and to its children.
 * A Patch knows nothing about the data it encapsulates.
 * A Patch is a move-only object. Only two modules can create Patches:
 *      - the initial condition module, to initialize the simulation
 *      - the MLMD module, when refinement is needed.
 *
 *  Both modules will create temporary Patch object and std::move() them
 *  to a Hierarchy. Copy is not allowed as a Patch encapsulate potentially
 *  heavy PatchData object, which is also move-only for the same reason.
 */
class Patch
{
private:
    uint32 id_;
    Box coordinates_;
    double dt_;
    GridLayout layout_;
    PatchData data_;
    std::shared_ptr<Patch> parent_;
    std::vector<std::shared_ptr<Patch>> children_;


public:
    explicit Patch(Box coordinates, double dt, GridLayout const& layout, PatchData&& patchData)
        : coordinates_{coordinates}
        , dt_{dt}
        , layout_{layout}
        , data_{std::move(patchData)}
        , parent_{nullptr}
        , children_{}
    {
    }

    Patch(Patch&& source) = default;
    Patch& operator=(Patch&& source) = default;

    Patch(Patch const& source) = delete;
    Patch& operator=(Patch& source) = delete;

    ~Patch() = default;

    void init();

    bool hasChildren() const { return children_.size() > 0; }
    std::size_t nbrChildren() const { return children_.size(); }

    Box const& coordinates() const { return coordinates_; }

    double timeStep() const { return dt_; }

    GridLayout const& layout() const { return layout_; }

    std::shared_ptr<Patch> parent() const { return parent_; }
    std::shared_ptr<Patch> children(uint32 ik) const { return children_[ik]; }

    PatchData& data() { return data_; }
    PatchData const& data() const { return data_; }

    uint32 getID() const { return id_; }

    void addChild(std::shared_ptr<Patch> newChild);
};

#endif // PATCH_H
