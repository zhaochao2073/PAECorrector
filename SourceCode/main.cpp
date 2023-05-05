#include <QApplication>
#include <QTextCodec>

#include "DataAnalysis/DataAnalysis.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QTextCodec * code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
