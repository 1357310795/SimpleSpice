#pragma once
#include <string>
#include <vector>

class CircuitNode
{
public:
    std::string nodeName;
    std::string prefix;

    CircuitNode() {};
    CircuitNode(std::string nodeName) : nodeName(nodeName) {};
    CircuitNode(std::string nodeName, std::string prefix) : nodeName(nodeName), prefix(prefix) {};
};