#include "DataSequence.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QStringList>

SingleData::SingleData()
    :m_StartTimeStr(""), m_StartTime(0), m_Duration(0), m_Value(0)
{
}

SingleData::SingleData(const QString & StartDateTimeStr, double duration, double value, bool *ok = 0)
    :m_StartTimeStr(StartDateTimeStr), m_StartTime(0), m_Duration(duration), m_Value(value)
{
    QDateTime date = QDateTime::fromString(StartDateTimeStr, "yyyy-MM-dd hh:mm:ss");
    if ( !date.isValid() && ok != 0 ) {
        (*ok) = false;
        return;
    }
    m_StartTime = (double)date.toTime_t()-(double)QDateTime( QDate(1987,12,24), QTime(8,0,0) ).toTime_t();
    m_StartTime /= 3600.0;
    m_Duration /= 60.0;
}

SingleData::SingleData(const QDateTime & StartDateTime, double duration, double value)
    :m_StartTimeStr(""), m_StartTime(0), m_Duration(duration), m_Value(value)
{
    m_StartTimeStr = StartDateTime.toString("yyyy-MM-dd hh:mm:ss");
    m_StartTime = (double)StartDateTime.toTime_t()-(double)QDateTime( QDate(1987,12,24), QTime(8,0,0) ).toTime_t();
    m_StartTime /= 3600.0;
    m_Duration /= 60.0;
}

SingleData & SingleData::operator =(const SingleData & rhl)
{
    m_StartTimeStr = rhl.m_StartTimeStr;
    m_StartTime = rhl.m_StartTime;
    m_Duration = rhl.m_Duration;
    m_Value = rhl.m_Value;
    return (*this);
}

DataSequence::DataSequence()
    :QVector<SingleData>()
{
}

DataSequence::DataSequence(DataSequence & rhl)
    :QVector<SingleData>()
{
    resize(rhl.size());
    for( int i = 0; i < size(); ++i ){
        (*this)[i] = rhl[i];
    }
}

void DataSequence::sort()
{
    qSort(begin(),end(),Comp);
}

void DataSequence::copy(const DataSequence & rhl)
{
    resize(rhl.size());
    for( int i = 0; i < size(); ++i ){
        (*this)[i] = rhl[i];
    }
}

bool DataSequence::read(const QString & fileName)
{
	clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        char sep = '\t';
        if ( line.count(',') >= 6 ) sep = ',';
        QStringList words = line.split(sep);
        bool ok = true;
        if ( words[0].toInt(&ok) && words.size() >= 7 ) {
            if ( words.size() < 7 )
                return false;

            QDateTime date = QDateTime::fromString(words[2], "yyyy-MM-dd hh:mm:ss");
            if ( !date.isValid() ) {
                date = QDateTime::fromString(words[2], "yyyy/M/d h:mm:ss");
            }
            if ( !date.isValid() ) {
                date = QDateTime::fromString(words[2], "yyyy/M/d h:mm");
            }
            if ( !date.isValid() ) {
                return false;
            }

            double duration = words[6].toFloat(&ok);
            if ( !ok || duration <= 0 )
                return false;

            double value = words[4].toFloat(&ok);
            if ( !ok || value < 0 )
                return false;

            push_back( SingleData(date,duration,value) );
        }
    }
    if ( size() == 0 ) return false;
    sort();
    return true;
}

void DataSequence::calibrate(double calibrationFactor)
{
    for( int i = 0; i < size(); ++i )
        (*this)[i].multiply(calibrationFactor);
}
