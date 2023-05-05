#ifndef DataAnalysis_H
#define DataAnalysis_H

#include "DataSequence.h"

class DataAnalysis
{
public:
    DataAnalysis();
    double CumulativeFunction(double time) const;
    double AveCumulativeFunction(double timeBegin, double timeEnd) const;
    double DecayFactorPb212(double timeBegin, double timeEnd) const;

    void costBG(const DataSequence & sourceData, DataSequence & newData, double bg );
    void costDecayBG(const DataSequence & sourceData, DataSequence & newData, double bg );
    void correctCumulativeEffect(const DataSequence & sourceData, DataSequence & newData, double & DecayEffectFactor );
private:
    const double L_Pb;
    const double L_Bi;
    double f1, f2;
};
\
#endif // DataAnalysis_H
