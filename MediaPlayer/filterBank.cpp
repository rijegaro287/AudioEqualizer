#include "filterBank.h"

namespace FilterBank
{
    // Filtro con banda de paso de 22Hz a 44Hz
    Filter *band_1 = new Filter({-1.997987552553654},
                                {0.998007324693865},
                                0.000996337653067);

    // Filtro con banda de paso de 44Hz a 88Hz
    Filter *band_2 = new Filter({-1.996222904179254},
                                {0.996302471991951},
                                0.001848764004024);

    // Filtro con banda de paso de 88Hz a 176Hz
    Filter *band_3 = new Filter({-1.992311160086715},
                                {0.992618565238867},
                                0.003690717380566);

    // Filtro con banda de paso de 176Hz a 353Hz
    Filter *band_4 = new Filter({-1.98416968339499},
                                {0.985431620826654},
                                0.007284189586672);

    // Filtro con banda de paso de 353Hz a 707Hz
    Filter *band_5 = new Filter({-1.960830937224514, -1.974890676225681},
                                {0.968105063798403, 0.978251989040713},
                                0.018976752856496);

    // Filtro con banda de paso de 707Hz a 1414Hz
    Filter *band_6 = new Filter({-1.909053678507300, -1.943707962760329},
                                {0.937681693392037, 0.957091772473230},
                                0.037251993297590);

    // Filtro con banda de paso de 1414Hz a 2828Hz
    Filter *band_7 = new Filter({-1.766103793075504, -1.862165912835572},
                                {0.877848663746051, 0.914717730798640},
                                0.073584163788485);

    // Filtro con banda de paso de 2828Hz a 5657Hz
    Filter *band_8 = new Filter({-1.361158408938612, -1.633907702328189},
                                {0.773679639397642, 0.834150395410529},
                                0.139910234954161);

    // Filtro con banda de paso de 5657Hz a 11314Hz
    Filter *band_9 = new Filter({-0.967558562062742, -0.243745801634429},
                                {0.670730913734913, 0.614238968000610},
                                0.260216038897773);

    // Filtro con banda de paso de 11314Hz a 22627Hz
    Filter *band_10 = new Filter({1.989925979650414, 0.362181960784019},
                                 {0.989976962657561, 0.197128528999125},
                                 0.455711518952991);

    float global_volume = 0; // Volumen de todo el sistema

    /*
        Cambia el atributo global_volume para
        alterar la ganancia del sistema.
        @param volume: Nuevo volumen del sistema
    */
    void set_global_volume(const float volume)
    {
        global_volume = volume / 100;
        std::cout << global_volume << std::endl;
    }

    /*
        Pasa una muestra por todos los filtros
        y suma las salidas.
        @param x_n: Valor de la muestra a procesar
        @returns La suma de la salida de todos los filtros
    */
    double filter_all_bands(float x_n)
    {
        // Pasa la muestra por todos los filtros y guarda los resultados
        double filtered_values[10] = {band_1->filter(x_n),
                                      band_2->filter(x_n),
                                      band_3->filter(x_n),
                                      band_4->filter(x_n),
                                      band_5->filter(x_n),
                                      band_6->filter(x_n),
                                      band_7->filter(x_n),
                                      band_8->filter(x_n),
                                      band_9->filter(x_n),
                                      band_10->filter(x_n)};

        EnergyCalculator::add_energies(filtered_values); // Calcula la energia de todas las salidas

        double result = 0;
        for (int i = 0; i < 10; i++) // Suma todos los resultados de los filtros
        {
            // Multiplica la salida de cada filtro por el volumen global antes de sumarlo al resultato
            result += global_volume * filtered_values[i];
        }

        return result;
    }
}
