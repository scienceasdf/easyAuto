#include "widget.h"
#include "ui_widget.h"
#include <string>
#include <sstream>
#include <fstream>
#include "trans.h"
#include "form.h"
#include "strOp.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    chart = new QChart();

    ui->numLabel->setWordWrap(true);
    ui->denLabel->setWordWrap(true);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_plotButton_clicked()
{
    std::vector<double> num = polyFromRawText(ui->numInput->text().toStdString());//poly(ui->numInput->text());
    std::vector<double> den = polyFromRawText(ui->denInput->text().toStdString());//poly(ui->denInput->text());

    ui->numLabel->setText("NUM:" + QString::fromStdString(getRichTextString(ui->numInput->text().toStdString()))
                          + "=" + QString::fromStdString(getRichTextString(num)));
    ui->denLabel->setText("DEN:" + QString::fromStdString(getRichTextString(ui->denInput->text().toStdString()))
                          + "=" + QString::fromStdString(getRichTextString(den)));

    switch (ui->figureType->currentIndex()) {
    case 0:
        plotRootLocus(num,den);
        break;
    case 1:
        plotUnitStepResponse(num,den);
        break;
    case 2:
        plotUnitRampResponse(num,den);
    default:
        break;
    }

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

void Widget::plotRootLocus(const std::vector<double> num, const std::vector<double> den)
{
    int big;
    if(num.size() > den.size()){
        big = num.size();
    }
    else{
        big = den.size();
    }

    std::vector<std::complex<double>> polyRoot;

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
        }

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

    ui->plot->setChart(chart);
    ui->plot->setRenderHint(QPainter::Antialiasing);

}

void Widget::plotUnitStepResponse(const std::vector<double> num, const std::vector<double> den)
{
    int big;
    if(num.size() > den.size()){
        big = num.size();
    }
    else{
        big = den.size();
    }

    trans tsfunc(num,den);

    QLineSeries **series = new QLineSeries*[1];
    for(int i=0;i<1;++i){
        series[i] = new QLineSeries();
    }

    double time = ui->KmaxInput->text().toDouble();
    auto res = tsfunc.unitStepResponse(time);

    for(int i=0; i<200; ++i){
        series[0]->append(static_cast<double>(i) * time / 200.0,res[i]);
    }

    ui->plot->chart()->removeAllSeries();

    chart->legend()->hide();
    for(int i=0; i<1; ++i){
        series[i]->setUseOpenGL(true);
        chart->addSeries(series[i]);
    }

    chart->createDefaultAxes();


    ui->plot->setChart(chart);
    ui->plot->setRenderHint(QPainter::Antialiasing);
}

void Widget::plotUnitRampResponse(const std::vector<double> num, const std::vector<double> den)
{
    int big;
    if(num.size() > den.size()){
        big = num.size();
    }
    else{
        big = den.size();
    }

    trans tsfunc(num,den);

    QLineSeries **series = new QLineSeries*[1];
    for(int i=0;i<1;++i){
        series[i] = new QLineSeries();
    }

    double time = ui->KmaxInput->text().toDouble();
    auto res = tsfunc.unitRampResponse(time);

    for(int i=0; i<200; ++i){
        series[0]->append(static_cast<double>(i) * time / 200.0,res[i]);
    }

    ui->plot->chart()->removeAllSeries();

    chart->legend()->hide();
    for(int i=0; i<1; ++i){
        series[i]->setUseOpenGL(true);
        chart->addSeries(series[i]);

    }

    chart->createDefaultAxes();

    ui->plot->setChart(chart);
    ui->plot->setRenderHint(QPainter::Antialiasing);
}

void Widget::on_figureType_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->rangeLabel->setText("Kmax");
        break;
    case 1:
        ui->rangeLabel->setText("Tmax");
        break;
    case 2:
        ui->rangeLabel->setText("Tmax");
        break;
    default:
        break;
    }
}
