#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "qcustomplot.h"
#include "SettingDialog.h"

#include "DataAnalysis/DataAnalysis.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void read();
    void clearAll();
    void save();
    void setParameterDialog();
    void setParameter();
    void selectBG();
    void selectConData();
    void analyzeSelectedData();

private:
    void correctMeasureData();
    bool writeToFile(const QString & fileName);

    void refurbishOriginTable();
    void refurbishBGTable();
    void refurbishDataTable();
    void plotData();


private:
    QTableWidget  * OriginTable;
    QTableWidget  * BGTable;
    QTableWidget  * DataTable;
    QCustomPlot * customPlot;

    QLineEdit * meanValue;
    QLineEdit * sdValue;
    QLineEdit * slope;
    QLineEdit * outof2KPercent;
    SettingDialog * settingDialog;

private:
    DataSequence OriginData;
    DataSequence BGData;
    DataSequence MeasureData;
    DataSequence CostBGData, CostDecayBGData, CorrectedData;

private:
    double background;
    double depositionEffectCoefficient;
    double calibrationFactor;
};

#endif // MAINWINDOW_H
