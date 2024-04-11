#include "circuit/circuit.h"

Circuit::~Circuit() {
    for(BaseDevice* d : this->devices)
        delete d;
    this->devices.clear();
}
