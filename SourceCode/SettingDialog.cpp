#include "SettingDialog.h"


#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent), bAccepted(true)
{
    this->setWindowTitle("Setting");
    backgroundEdit = new QLineEdit;
    CDEEdit = new QLineEdit;
    CFEdit = new QLineEdit;
    setParameter2Default();

    QLabel * backgroundLabel = new QLabel("Intrinsic background (cpm)");
    QLabel * CDELabel = new QLabel("Coefficient of deposition effect");
    QLabel * CFLabel = new QLabel("Calibration Factor (Bq m-3 cpm-1)");

    QGridLayout * parameterLayout = new QGridLayout;
    parameterLayout->addWidget(backgroundLabel,0,0,1,1);
    parameterLayout->addWidget(backgroundEdit,0,1,1,1);
    parameterLayout->addWidget(CDELabel,1,0,1,1);
    parameterLayout->addWidget(CDEEdit,1,1,1,1);
    parameterLayout->addWidget(CFLabel,2,0,1,1);
    parameterLayout->addWidget(CFEdit,2,1,1,1);

    QPushButton * defaultButton = new QPushButton("Default");
    QPushButton * acceptButton = new QPushButton("Ok");
    QPushButton * rejectButton = new QPushButton("Cancel");

    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(defaultButton);
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(rejectButton);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(parameterLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(defaultButton, SIGNAL(clicked()), this, SLOT(setParameter2Default()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptSetting()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(rejectSetting()));
}

void SettingDialog::setParameter2Default()
{
    backgroundEdit->setText("1.02");
    CDEEdit->setText("0.314");
    CFEdit->setText("42.37");
}

void SettingDialog::acceptSetting()
{
    bAccepted = true;
    hide();
    emit hided();
}

void SettingDialog::rejectSetting()
{
    bAccepted = false;
    hide();
    emit hided();
}
