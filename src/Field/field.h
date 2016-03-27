#ifndef FIELD_H
#define FIELD_H

#include <string>

class Field
{
public:
    Field();
    ~Field();

private:
    std::string name;
    unsigned int nbDims;
    double *data;

};

#endif // FIELD_H
