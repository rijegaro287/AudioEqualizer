#include "Filter.h"

/*
    @param a_1: Vector con los coeficientes a_1 de cada bloque
    @param a_2: Vector con los coeficientes a_2 de cada bloque
    @param g: Ganancia de todos los bloques
*/
Filter::Filter(vector<double> a_1, vector<double> a_2, double g)
{
    this->a_1 = a_1;
    this->a_2 = a_2;
    this->g = g;

    vector<double> zeros(a_1.size(), 0);

    this->w_1 = zeros;
    this->w_2 = zeros;

    this->volume = 1;
}

/*
    Cambia la ganancia del filtro de entre -12dB y 12dB.
        @param volume: Nuevo volumen del filtro
*/
void Filter::set_volume(float volume)
{
    this->volume = pow(10, volume / 2000);
    cout << this->volume << endl;
}

/*
    Procesa una muestra haciendo uso de la ecuación de
    diferencias del sistema. Utiliza forma transpuesta.
    @param x_n: Valor de la muestra a procesar
*/
double Filter::filter(float x_n)
{
    // Pasa la muestra por cada sección de orden 2
    for (int k = 0; k < a_1.size(); k++)
    {
        // Calcula la salida utilizando w_1 del resultado anterior
        double y_n = x_n + w_1[k];

        // Sobreescribe el valor de w_1 utilizando la salida actual y w_2 anterior
        w_1[k] = w_2[k] - a_1[k] * y_n;

        // Sobreescribe el valor de w_2 utilizando la salida y entrada actuales
        w_2[k] = -1 * (x_n + a_2[k] * y_n);

        // Multiplica la salida por la ganancia de la sección actual
        // y asigna el resultado a la entrada para su procesamiento en
        // la sección siguiente
        x_n = volume * g * y_n;
    }

    // Devuelve el valor final de la entrada tras finalizar el procesamiento
    return x_n;
}