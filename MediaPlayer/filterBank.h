#pragma once

#include "Filter.h"
#include "energyCalculator.h"

namespace FilterBank
{
    extern Filter *band_1;
    extern Filter *band_2;
    extern Filter *band_3;
    extern Filter *band_4;
    extern Filter *band_5;
    extern Filter *band_6;
    extern Filter *band_7;
    extern Filter *band_8;
    extern Filter *band_9;
    extern Filter *band_10;

    extern float global_volume;

    void set_global_volume(const float volume);

    double filter_all_bands(float x_n);
}