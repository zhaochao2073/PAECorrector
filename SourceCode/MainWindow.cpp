#include "MainWindow.h"

#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settingDialog(0), background(1.08), depositionEffectCoefficient(0.314), calibrationFactor(42.37)
{
    this->setWindowTitle("ProgenyAccumulationEffectCorrecter");

    QToolBar *toolBar = addToolBar(tr("&File and Data"));
    QAction * read = toolBar->addAction("&Read Data");
    QAction * clear = toolBar->addAction("&Clear Data");
    QAction * save = toolBar->addAction("&Save Data");
    QAction * set = toolBar->addAction("&Setting");

    OriginTable  = new QTableWidget();
    BGTable      = new QTableWidget();
    DataTable    = new QTableWidget();
    OriginTable->setColumnCount(3);
    BGTable->setColumnCount(3);
    DataTable->setColumnCount(6);
    OriginTable->setColumnWidth(0,150);
    OriginTable->setColumnWidth(1,90);
    OriginTable->setColumnWidth(2,150);
    OriginTable->setMinimumSize(450,300);
    OriginTable->resize(450,300);
    BGTable->setColumnWidth(0,150);
    BGTable->setColumnWidth(1,90);
    BGTable->setColumnWidth(2,150);
    BGTable->setMinimumWidth(450);
    BGTable->setMinimumHeight(60);
    BGTable->resize(450,60);
    DataTable->setColumnWidth(0,150);
    DataTable->setColumnWidth(1,90);
    DataTable->setColumnWidth(2,150);
    DataTable->setColumnWidth(3,170);
    DataTable->setMinimumSize(620,240);
    DataTable->resize(620,300);
    OriginTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    BGTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    DataTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    customPlot = new QCustomPlot;
    customPlot->setMinimumSize(620,300);

    QLabel * meanValueLabel = new QLabel("Mean (Bq/m^3)");
    QLabel * sdValueLabel = new QLabel("\t\tSD (Bq/m^3)");
    QLabel * slopeLabel = new QLabel("Slope (Bq/m^3/h)");
    QLabel * outof2KPercentLabel = new QLabel("\t\tPercent of data out of 2K range (%)");
    QPushButton * analyze = new QPushButton("Analyze");

    meanValue = new QLineEdit;
    sdValue = new QLineEdit;
    slope = new QLineEdit;
    outof2KPercent = new QLineEdit;
    meanValue->setReadOnly(true);
    sdValue->setReadOnly(true);
    slope->setReadOnly(true);
    outof2KPercent->setReadOnly(true);

    QGridLayout * statisticsLayout = new QGridLayout;
    QGroupBox * statisticsGroup = new QGroupBox("Analyze selected data");
    statisticsLayout->addWidget(meanValueLabel,0,0,1,1);
    statisticsLayout->addWidget(meanValue,0,1,1,1);
    statisticsLayout->addWidget(sdValueLabel,0,2,1,1);
    statisticsLayout->addWidget(sdValue,0,3,1,1);
    statisticsLayout->addWidget(slopeLabel,1,0,1,1);
    statisticsLayout->addWidget(slope,1,1,1,1);
    statisticsLayout->addWidget(outof2KPercentLabel,1,2,1,1);
    statisticsLayout->addWidget(outof2KPercent,1,3,1,1);
    statisticsLayout->addWidget(analyze,2,3,1,1);
    statisticsGroup->setLayout(statisticsLayout);



    QVBoxLayout * fileDateLayout = new QVBoxLayout;
    QGroupBox * fileDateGroup = new QGroupBox("File Data");
    fileDateLayout->addWidget(OriginTable);
    fileDateGroup->setLayout(fileDateLayout);

    QVBoxLayout * bgDateLayout = new QVBoxLayout;
    QGroupBox * bgDateGroup = new QGroupBox("Background Data");
    bgDateLayout->addWidget(BGTable);
    bgDateGroup->setLayout(bgDateLayout);

    QVBoxLayout * conDateLayout = new QVBoxLayout;
    QGroupBox * conDateGroup = new QGroupBox("Concerned Data");
    conDateLayout->addWidget(DataTable);
    conDateGroup->setLayout(conDateLayout);

    QVBoxLayout * figLayout = new QVBoxLayout;
    QGroupBox * figGroup = new QGroupBox("Plotting");
    figLayout->addWidget(customPlot);
    figGroup->setLayout(figLayout);

    QPushButton * selectBGButton = new QPushButton("⇩");
    QPushButton * selectConButton = new QPushButton("⇨");
    QFont font("courier");
    font.setPointSize(24);
    selectBGButton->setFont(font);
    selectConButton->setFont(font);
    selectBGButton->setFixedSize(50,30);
    selectConButton->setFixedSize(30,50);

    QHBoxLayout * selectBGbuttonLayout = new QHBoxLayout;
    selectBGbuttonLayout->addStretch();
    selectBGbuttonLayout->addWidget(selectBGButton);
    selectBGbuttonLayout->addStretch();

    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addWidget(fileDateGroup,1);
    leftLayout->addLayout(selectBGbuttonLayout);
    leftLayout->addWidget(bgDateGroup,0.2);

    QVBoxLayout * selectDatabuttonLayout = new QVBoxLayout;
    selectDatabuttonLayout->addStretch();
    selectDatabuttonLayout->addWidget(selectConButton);
    selectDatabuttonLayout->addStretch();
    QHBoxLayout * conDataLayout = new QHBoxLayout;
    conDataLayout->addLayout(selectDatabuttonLayout);
    conDataLayout->addWidget(conDateGroup);
    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->addLayout(conDataLayout);
    rightLayout->addWidget(statisticsGroup);
    rightLayout->addWidget(figGroup);

    QWidget * mainWidget = new QWidget;
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    clearAll();
    connect(read, SIGNAL(triggered()), this, SLOT(read()));
    connect(clear, SIGNAL(triggered()), this, SLOT(clearAll()));
    connect(save, SIGNAL(triggered()), this, SLOT(save()));
    connect(set, SIGNAL(triggered()), this, SLOT(setParameterDialog()));
    connect(selectBGButton, SIGNAL(clicked()), this, SLOT(selectBG()));
    connect(selectConButton, SIGNAL(clicked()), this, SLOT(selectConData()));
    connect(analyze, SIGNAL(clicked()), this, SLOT(analyzeSelectedData()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::read()
{
    clearAll();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Data File"));
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        if (!OriginData.read(path)) {
            QMessageBox::information(NULL, tr("Path"), tr("Format of the selected file is not correct!"));
            return;
        }
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        return;
    }
    refurbishOriginTable();
}

void MainWindow::clearAll()
{
    OriginData.clear();
    BGData.clear();
    MeasureData.clear();
    CostBGData.clear();
    CostDecayBGData.clear();
    CorrectedData.clear();

    meanValue->clear();
    sdValue->clear();
    slope->clear();
    outof2KPercent->clear();

    refurbishOriginTable();
    refurbishBGTable();
    refurbishDataTable();
    plotData();
}

void MainWindow::refurbishOriginTable()
{
    OriginTable->clear();
    OriginTable->setRowCount(0);

    OriginTable->clear();
    QString tableLabels = "Start Time,Duration(m),Origin cpm";
    OriginTable->setHorizontalHeaderLabels(tableLabels.split(','));
    OriginTable->setRowCount(OriginData.size());
    for( int i = 0; i < OriginData.size(); ++i ) {
        OriginTable->setItem(i,0,new QTableWidgetItem(OriginData[i].startTimeStr()));
        OriginTable->setItem(i,1,new QTableWidgetItem((QString::number(OriginData[i].duration()*60))));
        OriginTable->setItem(i,2,new QTableWidgetItem((QString::number(OriginData[i].value()))));
        OriginTable->setRowHeight(i,18);
    }

}

void MainWindow::refurbishBGTable()
{
    BGTable->clear();
    QString tableLabels = "Start Time,Duration(m),Bg cpm";
    BGTable->setHorizontalHeaderLabels(tableLabels.split(','));
    BGTable->setRowCount(BGData.size());
    for( int i = 0; i < BGData.size(); ++i ) {
        BGTable->setItem(i,0,new QTableWidgetItem(BGData[i].startTimeStr()));
        BGTable->setItem(i,1,new QTableWidgetItem((QString::number(BGData[i].duration()*60))));
        BGTable->setItem(i,2,new QTableWidgetItem((QString::number(BGData[i].value()))));
        BGTable->setRowHeight(i,18);
    }
}

void MainWindow::refurbishDataTable()
{
    DataTable->clear();

    QString tableLabels = "Start Time,Duration(m),Origin cpm,UnCorrected Conc.(Bq/m^3),Corrected cpm,Corrected Conc.(Bq/m^3)";
    DataTable->setHorizontalHeaderLabels(tableLabels.split(','));
    DataTable->setRowCount(MeasureData.size());
    for( int i = 0; i < MeasureData.size(); ++i ) {
        DataTable->setItem(i,0,new QTableWidgetItem(MeasureData[i].startTimeStr()));
        DataTable->setItem(i,1,new QTableWidgetItem((QString::number(MeasureData[i].duration()*60))));
        DataTable->setItem(i,2,new QTableWidgetItem((QString::number(MeasureData[i].value()))));
        DataTable->setItem(i,3,new QTableWidgetItem((QString::number(MeasureData[i].value()*calibrationFactor))));
        if ( i < CorrectedData.size() )
            DataTable->setItem(i,4,new QTableWidgetItem((QString::number(CorrectedData[i].value()))));
            DataTable->setItem(i,5,new QTableWidgetItem((QString::number(CorrectedData[i].value()*calibrationFactor))));
        DataTable->setRowHeight(i,18);
    }
}

void MainWindow::plotData()
{
    customPlot->clearGraphs();
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("concentration (Bq/m^3)");
    customPlot->xAxis->setRange(QDateTime(QDate::currentDate(),QTime(8,0)).toTime_t(), QDateTime(QDate::currentDate(),QTime(18,0)).toTime_t());
    customPlot->yAxis->setRange(0, 10000);

    QCPAxisTickerDateTime * DateTimeTicker = new QCPAxisTickerDateTime();
    DateTimeTicker->setDateTimeFormat("hh:mm\nMM-dd\nyyyy");
    QSharedPointer<QCPAxisTicker> ptr = QSharedPointer<QCPAxisTicker> ( DateTimeTicker );

    customPlot->xAxis->setTicker(ptr);
    QFont font("courier");
    //font.setStyleHint(QFont::Courier);
    font.setPointSize(12);
    customPlot->xAxis->setLabelFont(font);
    customPlot->yAxis->setLabelFont(font);
    font.setPointSize(10);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setVisible(false);
    customPlot->replot();


    if ( MeasureData.size() == 0 || MeasureData.size() != CorrectedData.size() )
    {
        return;
    }
    const long size = MeasureData.size();

    QVector<double> time(size);
    QVector<double> con1(size), con2(size);
    double maxValue = 0;
    double minValue = 0;
    for( int i = 0; i < size; ++i ) {
        time[i] = QDateTime::fromString(MeasureData[i].startTimeStr(), "yyyy-MM-dd hh:mm:ss").toTime_t();
        con1[i] = MeasureData[i].value()*calibrationFactor;
        con2[i] = CorrectedData[i].value()*calibrationFactor;
        if ( con1[i] > maxValue ) maxValue = con1[i];
        if ( con2[i] > maxValue ) maxValue = con2[i];

        if ( con1[i] < minValue ) minValue = con1[i];
        if ( con2[i] < minValue ) minValue = con2[i];
    }
    if (minValue < 0)
        minValue *= 1.2;
    else
        minValue *= 0.8;
    if (maxValue > 0)
        maxValue *= 1.5;
    else
        maxValue *= 0.5;

    //Plot
    customPlot->xAxis->setRange(time[0], time[size-1]);
    customPlot->yAxis->setRange(minValue, maxValue);
    customPlot->addGraph();
    customPlot->addGraph();
    QPen pen0(QColor(0,0,0));
    pen0.setWidth(3);
    QPen pen1(QColor(255,0,0));
    pen1.setWidth(3);
    customPlot->graph(0)->setPen(pen0);
    customPlot->graph(1)->setPen(pen1);
    customPlot->graph(0)->setData(time, con1);
    customPlot->graph(1)->setData(time, con2);
    customPlot->graph(0)->setName("Measured data");
    customPlot->graph(1)->setName("Corrected data");
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QBrush(QColor(10,10,100,20)));
    customPlot->replot();
}

void MainWindow::save()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Save Data to File"));
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        if (!writeToFile(path)) {
            QMessageBox::information(NULL, tr("Warnning"), tr("Unable to write data into file!"));
            return;
        } else {
            QMessageBox::information(NULL, tr("Ok"), tr("Data have been saved into file"));
            return;
        }
    } else {
        return;
    }

}

bool MainWindow::writeToFile(const QString & fileName)
{
    if ( MeasureData.size() != CorrectedData.size() )
        return false;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    char sep = ',';
    char endl = '\n';
    out << "No." << sep
        << "start time" << sep
        << "duration(min)" << sep
        << "origin counting rate(cpm)" << sep
        << "net counting rate(cpm)" << sep
        << "net counting rate2(cpm)" << sep
        << "corrected counting rate2(cpm)" << sep
        << "uncorrected conc.(Bq/m^3)" << sep
        << "corrected conc.(Bq/m^3)" << sep << sep
        << "Intrinsic background(cpm)" << sep
        << "Deposition effect coefficient" << sep
        << "calibration factor(Bq/m^3/cpm)" << endl;

    for ( int i = 0; i < MeasureData.size(); ++i )
    {

        out << i+1 << sep
            << MeasureData[i].startTimeStr() << sep
            << MeasureData[i].duration()*60 << sep
            << MeasureData[i].value() << sep
            << CostBGData[i].value() << sep
            << CostDecayBGData[i].value() << sep
            << CorrectedData[i].value() << sep
            << MeasureData[i].value()*calibrationFactor << sep
            << CorrectedData[i].value()*calibrationFactor;

        if( i == 0 ) {
            out << sep << sep
                << background << sep
                << depositionEffectCoefficient << sep
                << calibrationFactor << endl;
        } else {
            out << endl;
        }
    }
    return true;
}

void MainWindow::setParameterDialog()
{
    if ( settingDialog == 0 ) {
        settingDialog = new SettingDialog(this);
    }
    settingDialog->show();

    connect(settingDialog, SIGNAL(hided()), this, SLOT(setParameter()));
}

void MainWindow::setParameter()
{
    if ( settingDialog->accepted() ) {
        background = settingDialog->background();
        depositionEffectCoefficient = settingDialog->depositionEffectCoefficient();
        calibrationFactor = settingDialog->calibrationFactor();
        correctMeasureData();
    }
}

void MainWindow::selectBG()
{
    QList<QTableWidgetItem *> selectedItems = OriginTable->selectedItems();
    QSet<long> selectedRows;
    QListIterator<QTableWidgetItem *> iter(selectedItems);
    while (iter.hasNext()) {
        long row = iter.next()->row();
        selectedRows.insert(row);
    }

    BGData.resize(selectedRows.count());
    QSetIterator<long> iterSet(selectedRows);
    int i = 0;
    while (iterSet.hasNext()) {
        long row = iterSet.next();
        BGData[i] = OriginData[row];
        ++i;
    }
    BGData.sort();
    refurbishBGTable();
    correctMeasureData();
}

void MainWindow::selectConData()
{
    QList<QTableWidgetItem *> selectedItems = OriginTable->selectedItems();
    QSet<long> selectedRows;
    QListIterator<QTableWidgetItem *> iter(selectedItems);
    while (iter.hasNext()) {
        long row = iter.next()->row();
        selectedRows.insert(row);
    }

    MeasureData.resize(selectedRows.count());
    QSetIterator<long> iterSet(selectedRows);
    int i = 0;
    while (iterSet.hasNext()) {
        long row = iterSet.next();
        MeasureData[i] = OriginData[row];
        ++i;
    }
    MeasureData.sort();
    correctMeasureData();
}

void MainWindow::correctMeasureData()
{
    DataAnalysis ansDemo;

    //cost BG
    double decayBG = 0;
    if ( BGData.size() > 0 && MeasureData.size() > 0 ) {
        double t0 = MeasureData[0].startTime();
        DataSequence NetDecayBGData;
        ansDemo.costBG(BGData,NetDecayBGData,background);

        double decayFactor = 1;
        for ( int i = 0; i < BGData.size(); ++i ) {
            decayFactor = ansDemo.DecayFactorPb212(BGData[i].startTime()-t0,BGData[i].endTime()-t0);
            decayBG += NetDecayBGData[i].value()/decayFactor;
        }
        decayBG /= BGData.size();
    }

    ansDemo.costBG(MeasureData,CostBGData,background);


    if ( decayBG > 0  ) {
        DataSequence CostBGDataTemp(CostBGData);
        ansDemo.costDecayBG(CostBGData,CostDecayBGData,decayBG);
    } else {
        CostDecayBGData.copy(CostBGData);
    }

    //correction
    ansDemo.correctCumulativeEffect(CostDecayBGData,CorrectedData,depositionEffectCoefficient);

    //refresh and plot
    refurbishDataTable();
    plotData();
}

void MainWindow::analyzeSelectedData()
{
    QList<QTableWidgetItem *> selectedItems = DataTable->selectedItems();
    QSet<long> selectedRows;
    QListIterator<QTableWidgetItem *> iter(selectedItems);
    while (iter.hasNext()) {
        long row = iter.next()->row();
        selectedRows.insert(row);
    }
    if ( selectedRows.size() == 0  ) {
        return;
    }

    if ( (*selectedRows.end()) >= CorrectedData.size() ) {
        QMessageBox::information(NULL, tr("Error"), tr("Overflow error! clear all!"));
        clearAll();
        return;
    }

    double t(0), v(0), t_sum(0), v_sum(0), tt_sum(0), vv_sum(0), tv_sum(0);
    double N = selectedRows.size();
    QSetIterator<long> iterSet(selectedRows);
    while (iterSet.hasNext()) {
        long row = iterSet.next();
        t = CorrectedData[row].startTime();
        v = CorrectedData[row].value()*calibrationFactor;

        t_sum += t;
        v_sum += v;
        tt_sum += t*t;
        vv_sum += v*v;
        tv_sum += t*v;
    }

    double d_meanValue =  v_sum/N;
    double d_sdValue =  sqrt(vv_sum/N - d_meanValue*d_meanValue);
    double d_slope =  (tv_sum - t_sum*v_sum/N) / (tt_sum - t_sum*t_sum/N);

    double outCount = 0;
    QSetIterator<long> iterSet2(selectedRows);
    while (iterSet2.hasNext()) {
        long row = iterSet2.next();
        v = CorrectedData[row].value()*calibrationFactor;
        if ( v < d_meanValue - 2*d_sdValue || v > d_meanValue + 2*d_sdValue  )
            outCount += 1;
    }
    double d_outof2KPercent = outCount/N*100;

    meanValue->setText(QString::number(d_meanValue));
    sdValue->setText(QString::number(d_sdValue));
    slope->setText(QString::number(d_slope));
    outof2KPercent->setText(QString::number(d_outof2KPercent));
}
