#include "energyCalculator.h"
#include <iostream>

namespace EnergyCalculator
{
    float band_1_energy = 0;  // Energía de la banda de paso de 22Hz a 44Hz
    float band_2_energy = 0;  // Energía de la banda de paso de 44Hz a 88Hz
    float band_3_energy = 0;  // Energía de la banda de paso de 88Hz a 176Hz
    float band_4_energy = 0;  // Energía de la banda de paso de 176Hz a 353Hz
    float band_5_energy = 0;  // Energía de la banda de paso de 353Hz a 707Hz
    float band_6_energy = 0;  // Energía de la banda de paso de 707Hz a 1414Hz
    float band_7_energy = 0;  // Energía de la banda de paso de 1414Hz a 2828Hz
    float band_8_energy = 0;  // Energía de la banda de paso de 2828Hz a 5657Hz
    float band_9_energy = 0;  // Energía de la banda de paso de 5657Hz a 11314Hz
    float band_10_energy = 0; // Energía de la banda de paso de 11314Hz a 22627Hz

    bool ready = false; // Indica si se han procesado al menos 1024 muestras

    /*
        Suma las energías de las bandas utilizando la fórmula: E(n) = |x(n)|^2
        @param filtered_values: vector de las muestras filtradas de cada banda
    */
    void add_energies(double *filtered_values)
    {
        band_1_energy += pow(filtered_values[0], 2);
        band_2_energy += pow(filtered_values[1], 2);
        band_3_energy += pow(filtered_values[2], 2);
        band_4_energy += pow(filtered_values[3], 2);
        band_5_energy += pow(filtered_values[4], 2);
        band_6_energy += pow(filtered_values[5], 2);
        band_7_energy += pow(filtered_values[6], 2);
        band_8_energy += pow(filtered_values[7], 2);
        band_9_energy += pow(filtered_values[8], 2);
        band_10_energy += pow(filtered_values[9], 2);
    }

    /*
        Resetea las energías de las bandas y el indicador de que se han procesado
    */
    void reset_energies()
    {
        band_1_energy = 0;
        band_2_energy = 0;
        band_3_energy = 0;
        band_4_energy = 0;
        band_5_energy = 0;
        band_6_energy = 0;
        band_7_energy = 0;
        band_8_energy = 0;
        band_9_energy = 0;
        band_10_energy = 0;

        ready = false;
    }

}
