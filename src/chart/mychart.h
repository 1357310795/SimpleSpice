#pragma once

#include <QWidget>
#include <QtCharts>
#include "chart/plot_context.h"

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
    void initChart(PlotContext& context);

private:
    Ui::ChartViewWindow *ui;
};

