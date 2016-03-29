#ifndef FIELD_H
#define FIELD_H

#include <string>

class Field
{
public:
    Field();
    virtual ~Field();

private:
    std::string name;
    unsigned int nbDims;
    double *data;


    virtual void allocate();

};

#endif // FIELD_H
