#ifndef DATASEQUENCE_H
#define DATASEQUENCE_H

#include <QString>
#include <QVector>
#include <QDateTime>

class SingleData
{
public:
    SingleData();
    SingleData(const QString & StartDateTimeStr, double duration, double value, bool *ok);
    SingleData(const QDateTime & StartDateTime, double duration, double value);
    SingleData & operator =(const SingleData & rhl);
    bool operator ==(const SingleData & rhl) const { return m_StartTime == rhl.m_StartTime; }
    bool operator >(const SingleData & rhl) const { return m_StartTime > rhl.m_StartTime; }
    bool operator <(const SingleData & rhl) const { return m_StartTime < rhl.m_StartTime; }
    bool operator >=(const SingleData & rhl) const { return m_StartTime <= rhl.m_StartTime; }
    bool operator <=(const SingleData & rhl) const { return m_StartTime <= rhl.m_StartTime; }

    const QString & startTimeStr() const {return m_StartTimeStr;}
    double startTime() const {return m_StartTime;}
    double endTime() const {return m_StartTime+m_Duration;}
    double duration() const {return m_Duration;}
    double value() const { return m_Value; }

    void minus(double s) { m_Value -= s; }
    void divide(double f) { m_Value /= f; }
    void multiply(double f) { m_Value *= f; }

private:
    QString m_StartTimeStr;
    double m_StartTime;
    double m_Duration;
    double m_Value;
};

class DataSequence : public QVector<SingleData>
{
public:
    DataSequence();
    DataSequence(DataSequence & rhl);
    void copy(const DataSequence & rhl);
    void sort();
    bool read(const QString & fileName);
    void calibrate(double calibrationFactor);
};

inline bool Comp(const SingleData &a, const SingleData &b)
{
    return a < b ;
}

#endif // DATASEQUENCE_H
