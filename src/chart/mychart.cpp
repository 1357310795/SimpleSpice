#include <QDebug>
#include <QtCharts>
#include <QString>
#include "chart/mychart.h"
#include "chart/plot_context.h"

QT_CHARTS_USE_NAMESPACE
#include "ui_mychart.h"

ChartViewWindow::ChartViewWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartViewWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("SimpleSpice - 仿真结果");
    //initChart();
}

ChartViewWindow::~ChartViewWindow()
{
    delete ui;
}

void ChartViewWindow::initChart(PlotContext& context)
{
    QChart* chart = ui->chartView->chart();    // 获取一个chart用于管理不同类型的series和其他图表相关对象
    //chart->legend()->hide();                   // 隐藏图例
    chart->setTitle(QString::fromStdString(context.title));
    QAbstractAxis* axisX;
    QAbstractAxis* axisY;
    QAbstractAxis* axisZ;
    for (auto &series : context.seriesGroups)
        chart->addSeries(series); 
    if (!context.useLogAxis)
    {
        axisX = new QValueAxis();
        ((QValueAxis*)axisX)->setLabelFormat("%.2e");
        ((QValueAxis*)axisX)->setTitleText(QString::fromStdString(context.axisXTitle));
        chart->addAxis(axisX, Qt::AlignBottom);
        axisY = new QValueAxis();
        ((QValueAxis*)axisY)->setLabelFormat("%.2e");
        ((QValueAxis*)axisY)->setTitleText(QString::fromStdString(context.axisYTitle));
        chart->addAxis(axisY, Qt::AlignLeft);
    }
    else
    {
        axisX = new QLogValueAxis();
        ((QLogValueAxis*)axisX)->setLabelFormat("%.2e");
        ((QLogValueAxis*)axisX)->setTitleText(QString::fromStdString(context.axisXTitle));
        ((QLogValueAxis*)axisX)->setBase(10.0);
        ((QLogValueAxis*)axisX)->setMinorTickCount(-1);
        chart->addAxis(axisX, Qt::AlignBottom);
        axisY = new QValueAxis();
        ((QValueAxis*)axisY)->setLabelFormat("%.2e");
        ((QValueAxis*)axisY)->setTitleText(QString::fromStdString(context.axisYTitle));
        chart->addAxis(axisY, Qt::AlignLeft);
    }
    if (context.useExtraAxis)
    {
        axisZ = new QValueAxis();
        ((QValueAxis*)axisZ)->setLabelFormat("%.2f");
        ((QValueAxis*)axisZ)->setTitleText(QString::fromStdString(context.axisZTitle));
        chart->addAxis(axisZ, Qt::AlignRight);
    }

    if (context.useExtraAxis)
    {
        for (int i = 0; i < context.seriesGroups.size(); i += 2)
        {
            context.seriesGroups[i]->attachAxis(axisX);
            context.seriesGroups[i]->attachAxis(axisY);
            context.seriesGroups[i+1]->attachAxis(axisX);
            context.seriesGroups[i+1]->attachAxis(axisZ);
        }
    }
    else
    {
        for (auto &series : context.seriesGroups)
        {
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        }
    }
    
    if (!context.useLogAxis && !context.useExtraAxis)
    {
        chart->axes(Qt::Vertical).first()->setRange(context.min, context.max);  // 设置Y轴的范围
    } 
    
    ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
}



