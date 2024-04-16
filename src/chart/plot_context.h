#pragma once
#include <QtCharts>

#include <vector>
#include <string>
#include <climits>
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
    double min;
    double max;

    PlotContext() {
        min = INT_MAX;
        max = INT_MIN;
    }
};
