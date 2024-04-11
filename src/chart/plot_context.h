#pragma once
#include <QtCharts>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class PlotContext {
public:
    std::vector<QAbstractSeries*> seriesGroups;
    std::string title;
    std::string axisXTitle;
    std::string axisYTitle;
    std::string axisZTitle;
    bool useLogAxis;
    bool useExtraAxis;
};
