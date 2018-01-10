#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <vector>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include "strOp.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void getCoeffs(const std::vector<double> &num, const std::vector<double> &den, double gain);

private slots:
    void on_plotButton_clicked();

    void on_donateButton_clicked();

    void on_figureType_currentIndexChanged(int index);

private:
    Ui::Widget *ui;

    std::vector<double> coeffs;

    void plotRootLocus(const std::vector<double> num, const std::vector<double> den);
    void plotUnitStepResponse(const std::vector<double> num, const std::vector<double> den);
    void plotUnitRampResponse(const std::vector<double> num, const std::vector<double> den);

    QChart* chart;
};



#endif // WIDGET_H
