#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
    bool accepted() const {return bAccepted;}
    double background() const {return backgroundEdit->text().toFloat();}
    double depositionEffectCoefficient() const {return CDEEdit->text().toFloat();}
    double calibrationFactor() const {return CFEdit->text().toFloat();}

signals:
    void hided();

public slots:
    void setParameter2Default();
    void acceptSetting();
    void rejectSetting();


private:
    bool bAccepted;
    QLineEdit * backgroundEdit;
    QLineEdit * CDEEdit;
    QLineEdit * CFEdit;
};

#endif // SETTINGDIALOG_H
