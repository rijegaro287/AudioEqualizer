#pragma once

#include <math.h>
#include <vector>

using namespace std;
namespace EnergyCalculator
{
    extern float band_1_energy;
    extern float band_2_energy;
    extern float band_3_energy;
    extern float band_4_energy;
    extern float band_5_energy;
    extern float band_6_energy;
    extern float band_7_energy;
    extern float band_8_energy;
    extern float band_9_energy;
    extern float band_10_energy;

    extern bool ready;

    void add_energies(double *filtered_values);

    void reset_energies();
}