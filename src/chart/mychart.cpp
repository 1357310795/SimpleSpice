#include "chart/mychart.h"
#include <QDebug>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE
#include "ui_mychart.h"

ChartViewWindow::ChartViewWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartViewWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("QtCharts绘图");
    //initChart();
}

ChartViewWindow::~ChartViewWindow()
{
    delete ui;
}

void ChartViewWindow::initChart(std::vector<QAbstractSeries*> &seriesGroups, bool useLogAxis)
{
    // QSplineSeries* series = new QSplineSeries();   // 创建一个样条曲线对象
    // series->setName("曲线");

    // #if 1
    //     // 添加数据方式1
    //     series->append(0, 10);
    //     series->append(1, 1);
    //     series->append(2, 4);
    //     series->append(3, 7);
    //     series->append(4, 13);
    //     // 添加数据方式2
    //     *series << QPointF(5, 3)<< QPointF(6, 6)<< QPointF(7, 13)<< QPointF(8, 5);
    // #else  // 添加数据方式3，一次性更新所有数据
    //     QList<QPointF> points;
    //     for(int i = 0; i < 20; i++)
    //     {
    //         points.append(QPointF(i, i %7));
    //     }
    //     series->replace(points);
    // #endif

        QChart* chart = ui->chartView->chart();    // 获取一个chart用于管理不同类型的series和其他图表相关对象
        //chart->legend()->hide();                   // 隐藏图例
        chart->setTitle("曲线图图表标题");           // 设置标题

        QAbstractAxis* axisX;
        QAbstractAxis* axisY;

        for (auto &series : seriesGroups)
            chart->addSeries(series); 

        if (!useLogAxis)
        {
            axisX = new QValueAxis();
            chart->addAxis(axisX, Qt::AlignBottom);
            axisY = new QValueAxis();
            chart->addAxis(axisY, Qt::AlignLeft);
        }
        else
        {
            axisX = new QLogValueAxis();
            //axisY.setLabelFormat('%g')
            //axisY.setTitleText('值')
            ((QLogValueAxis*)axisX)->setBase(10.0);
            ((QLogValueAxis*)axisX)->setMinorTickCount(-1);
            chart->addAxis(axisX, Qt::AlignBottom);

            axisY = new QValueAxis();
            chart->addAxis(axisY, Qt::AlignLeft);
            //axisX.setLabelFormat('%i');
            //axisX.setTitleText('数据点');
            //axisX.setTickCount(len(lineSeries));
            //lineSeries.attachAxis(axisX);
        }

        for (auto &series : seriesGroups)
        {
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        }
        // chart->axes(Qt::Vertical).first()->setRange(0, 20);  // 设置Y轴的范围

        ui->chartView->setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿
}



