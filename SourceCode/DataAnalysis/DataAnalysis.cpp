#include "DataAnalysis.h"
#include <cmath>
#include <QDateTime>

DataAnalysis::DataAnalysis()
    :L_Pb(0.0654), L_Bi(0.6863)
{
    f1 = -L_Bi/(L_Bi-L_Pb);
    f2 = L_Pb/(L_Bi-L_Pb);
}

double DataAnalysis::CumulativeFunction(double time) const
{
    return 1 + f1 * exp(-L_Pb*time) + f2 * exp(-L_Bi*time);
}

double DataAnalysis::AveCumulativeFunction(double timeBegin, double timeEnd) const
{
    double ave = 0;
    ave += f1/L_Pb * ( exp(-L_Pb*timeBegin) - exp(-L_Pb*timeEnd) );
    ave += f2/L_Bi * ( exp(-L_Bi*timeBegin) - exp(-L_Bi*timeEnd) );
    ave /= (timeEnd-timeBegin);
    return 1 + ave;
}

double DataAnalysis::DecayFactorPb212(double timeBegin, double timeEnd) const
{
    double decayFactor = exp(-L_Pb*timeBegin) - exp(-L_Pb*timeEnd);
    decayFactor /= L_Pb*(timeEnd-timeBegin);
    return decayFactor;
}

void DataAnalysis::costBG(const DataSequence & sourceData, DataSequence & newData, double bg )
{
    newData.copy(sourceData);
    for( int i = 0; i < newData.size(); ++i )
    {
        newData[i].minus(bg);
    }
}

void DataAnalysis::costDecayBG(const DataSequence & sourceData, DataSequence & newData, double bg )
{
    newData.copy(sourceData);
    double startTime, endTime;
    double t0 = 0;
    if ( newData.size() >= 1 ) t0 = newData[0].startTime();
    for( int i = 0; i < newData.size(); ++i )
    {
        startTime   = newData[i].startTime() - t0;
        endTime     = newData[i].endTime() - t0;
        newData[i].minus(bg*DecayFactorPb212(startTime,endTime));
    }
}

void DataAnalysis::correctCumulativeEffect(const DataSequence & sourceData, DataSequence & newData, double & DecayEffectFactor )
{
    newData.copy(sourceData);
    double TimeFunction_ij = 0;
    for( int i = 0; i < newData.size(); ++i )
    {

        for ( int j = 0; j < i; ++j )
        {
            double t1 = newData[i].startTime()-newData[j].startTime();
            double t2 = newData[i].endTime()-newData[j].startTime();
            double t3 = newData[i].startTime()-newData[j].endTime();
            double t4 = newData[i].endTime()-newData[j].endTime();
            TimeFunction_ij = AveCumulativeFunction(t1,t2)-AveCumulativeFunction(t3,t4);
            newData[i].minus(newData[j].value()*TimeFunction_ij*DecayEffectFactor);
        }
        double AveCumulativeFactor = AveCumulativeFunction(0,newData[i].duration());
        newData[i].divide(1+AveCumulativeFactor*DecayEffectFactor);
    }
}
