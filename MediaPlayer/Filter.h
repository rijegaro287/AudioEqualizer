#pragma once

#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

/*
    Crea un filtro de cualquier orden en base
    a filtros de orden 2.
    Implementación específica para filtros con coeficientes
    b_0 = 1
    b_1 = 0
    b_2 = -1
    Las ganancias de todos los bloques de orden 2 deben ser iguales.
*/
class Filter
{
private:
    float volume;

    vector<double> a_1;
    vector<double> a_2;
    double g;

    vector<double> w_1;
    vector<double> w_2;

public:
    Filter(vector<double> a_1, vector<double> a_2, double g);

    void set_volume(float volume);

    double filter(float x_n);
};