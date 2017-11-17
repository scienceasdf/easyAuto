#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include "trans.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString strCssFile = ":/skin.qss";
    QFile fCss(strCssFile);
    if( !fCss.open(QFile::ReadOnly))
    {
            //qDebug("css File %s load false",strCssFile);

    }
    QString strCssContent(fCss.readAll());
    a.setStyleSheet(strCssContent);
    fCss.close();

    Widget w;
    w.show();


    /*std::vector<double> p = {2,2,2};
    auto res = roots(p);
    for(int i=0; i<2; ++i){
        qDebug()<<res[i].real()<<res[i].imag();
    }*/
    return a.exec();
}
