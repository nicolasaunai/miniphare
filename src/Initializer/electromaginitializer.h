#ifndef ELECTROMAGINITIALIZER_H
#define ELECTROMAGINITIALIZER_H


class ElectromagInitializer
{

    using Function = double (*) (double x, double y, double z);

public:

    ElectromagInitializer();

    Function electricField;
    Function magneticField;

};

#endif // ELECTROMAGINITIALIZER_H
