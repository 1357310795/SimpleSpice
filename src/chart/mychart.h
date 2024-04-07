#pragma once

#include <QWidget>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class ChartViewWindow; }
QT_END_NAMESPACE

class ChartViewWindow : public QWidget
{
    Q_OBJECT

public:
    ChartViewWindow(QWidget *parent = nullptr);
    ~ChartViewWindow();

public:
    void initChart(std::vector<QAbstractSeries*> &seriesGroups, bool useLogAxis);

private:
    Ui::ChartViewWindow *ui;
};

