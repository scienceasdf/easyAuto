#include "widget.h"
#include "ui_widget.h"
#include <string>
#include <sstream>
#include <fstream>
#include "trans.h"
#include "form.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    chart = new QChart();
}

Widget::~Widget()
{
    delete ui;
}

std::vector<double> poly(const QString& str)
{
    std::string myString = str.toStdString();
    std::stringstream iss( myString );

    double number;
    std::vector<double> res;
    while ( iss >> number )
      res.push_back( number );

    return res;
}

void Widget::on_plotButton_clicked()
{
    std::vector<double> num = poly(ui->numInput->text());
    std::vector<double> den = poly(ui->denInput->text());
    std::reverse(num.begin(),num.end());
    std::reverse(den.begin(),den.end());



    int big;
    if(num.size() > den.size()){
        big = num.size();
    }
    else{
        big = den.size();
    }

    std::vector<std::complex<double>> polyRoot;

    //std::ofstream fout("D://roots.txt");
    //if(!fout.is_open()){

    //}

    QLineSeries **series = new QLineSeries*[big];
    for(int i=0;i<big;++i){
        series[i] = new QLineSeries();

    }

    double kMax = ui->KmaxInput->text().toDouble();

    for(int i=0; i<1000; ++i){
        getCoeffs(num,den,.001 * kMax * i);
        polyRoot = roots(coeffs);
        for(int j=0; j<big-1; ++j){
            series[j]->append(polyRoot[j].real(),polyRoot[j].imag());
            //fout<<polyRoot[j]<<"\t";
        }
        //fout<<"\n";

    }


    /*for(int i=1; i<80; ++i){
        getCoeffs(num,den,.2 * kMax + .1 * i);
        polyRoot = roots(coeffs);
        for(int j=0; j<big-1; ++j){
            series[j]->append(polyRoot[j].real(),polyRoot[j].imag());
            //fout<<polyRoot[j]<<"\t";
        }
        //fout<<"\n";
    }*/

    //fout.close();

    ui->plot->chart()->removeAllSeries();

    chart->legend()->hide();
    for(int i=0; i<big-1; ++i){
        series[i]->setUseOpenGL(true);
        //QPen pen = series[i]->pen();
        //pen.setWidth(3);
        //series[i]->setPen(pen);
        //series[i]->setPen(pen2);
        chart->addSeries(series[i]);

    }

    chart->createDefaultAxes();
    //chart->axisX()->setTitleText("Re");
    //chart->axisY()->setTitleText("Im");
    //chart->axisX()->setRange(-5,2);
    //chart->axisY()->setRange(-3.5,3.5);

    ui->plot->setChart(chart);
    ui->plot->setRenderHint(QPainter::Antialiasing);

    if(num.size() < den.size()){

        trans tsfunc(num,den);
        double phaseMargin, freq1;
        tsfunc.phaseMargin(&phaseMargin,&freq1);
        double gainMargin, freq2;
        tsfunc.gainMargin(&gainMargin,&freq2);

        ui->freq1Label->setText("剪切频率:" + QString::number(freq1) + "Hz");
        ui->phaseMarginLabel->setText("相位裕度:" + QString::number(phaseMargin)+"°");
        ui->freq2Label->setText("穿越频率:" + QString::number(freq2) + "Hz");
        ui->gainMarginLabel->setText("幅值裕度:" + QString::number(gainMargin)+"dB");
    }

}

void Widget::getCoeffs(const std::vector<double> &num, const std::vector<double> &den, double gain)
{
    int big,small;
    bool flag;
    if(num.size() > den.size()){
        big = num.size();
        small = den.size();
        flag = true;
    }
    else{
        big = den.size();
        small = num.size();
        flag = false;
    }

    coeffs.resize(big);

    for(int i=0; i<big; ++i){
        if(i < small){
            coeffs[i] = gain * num[i] + den[i];
        }
        else{
            coeffs[i] = (flag)?gain * num[i]:den[i];
        }
    }

}

void Widget::on_donateButton_clicked()
{
    Form* dnt = new Form();
    dnt->show();
}
