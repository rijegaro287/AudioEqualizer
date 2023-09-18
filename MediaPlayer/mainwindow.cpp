/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2010  Pablo Alvarado
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file   mainwindow.cpp
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado
 * \date   2010.12.12
 *
 * $Id: equalizer.cpp $
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jack.h"
#include <string>

#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif

/**
 * Precision used by trimming
 */
const float MainWindow::Epsilon = 0.001;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          verbose_(false)
{
  ui->setupUi(this);

  /*
    Llama a la función on_volumeSlider_changed cada vez que
    el valor del slider es cambiado
  */
  connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_volume_slider_changed(int)));
  connect(ui->bandSlider_1, SIGNAL(valueChanged(int)), this, SLOT(on_band_1_slider_changed(int)));
  connect(ui->bandSlider_2, SIGNAL(valueChanged(int)), this, SLOT(on_band_2_slider_changed(int)));
  connect(ui->bandSlider_3, SIGNAL(valueChanged(int)), this, SLOT(on_band_3_slider_changed(int)));
  connect(ui->bandSlider_4, SIGNAL(valueChanged(int)), this, SLOT(on_band_4_slider_changed(int)));
  connect(ui->bandSlider_5, SIGNAL(valueChanged(int)), this, SLOT(on_band_5_slider_changed(int)));
  connect(ui->bandSlider_6, SIGNAL(valueChanged(int)), this, SLOT(on_band_6_slider_changed(int)));
  connect(ui->bandSlider_7, SIGNAL(valueChanged(int)), this, SLOT(on_band_7_slider_changed(int)));
  connect(ui->bandSlider_8, SIGNAL(valueChanged(int)), this, SLOT(on_band_8_slider_changed(int)));
  connect(ui->bandSlider_9, SIGNAL(valueChanged(int)), this, SLOT(on_band_9_slider_changed(int)));
  connect(ui->bandSlider_10, SIGNAL(valueChanged(int)), this, SLOT(on_band_10_slider_changed(int)));

  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(on_classical_button_clicked()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_jazz_button_clicked()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_dance_button_clicked()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(on_pop_button_clicked()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(on_rock_button_clicked()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(on_reggae_button_clicked()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(on_techno_button_clicked()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(on_full_bass_button_clicked()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(on_full_treble_button_clicked()));
  connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(on_full_bass_and_treble_button_clicked()));

  connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(on_reset_button_clicked()));

  /*
   * Set up a timer 4 times in a second to check if the user
   * changed the equalizer values, and if so, then create a new
   * filter response
   */
  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
  timer_->start(1);

  dsp_ = new dspSystem;
  jack::init(dsp_);

  // parse some command line arguments
  QStringList argv(QCoreApplication::arguments());

  QStringList::const_iterator it(argv.begin());
  while (it != argv.end())
  {
    if ((*it) == "-v" || (*it) == "--verbose")
    {
      verbose_ = true;
    }
    else if ((*it).indexOf(".wav", 0, Qt::CaseInsensitive) > 0)
    {
      ui->fileEdit->setText(*it);
      std::string tmp(qPrintable(*it));
      jack::playAlso(tmp.c_str());
    }
    ++it;
  }
}

MainWindow::~MainWindow()
{
  jack::close();
  delete timer_;
  delete ui;
  delete dsp_;
}

/*
  Actualiza las posiciones de las barras de
  densidad espectral de energía.
*/
void MainWindow::update()
{
  if (EnergyCalculator::ready) // Espera a que se hayan procesado todas la muestras del buffer
  {
    // Mueve todas las barras de energía
    move_bar(ui->progressBar_1, calculate_bar_position(EnergyCalculator::band_1_energy));
    move_bar(ui->progressBar_2, calculate_bar_position(EnergyCalculator::band_2_energy));
    move_bar(ui->progressBar_3, calculate_bar_position(EnergyCalculator::band_3_energy));
    move_bar(ui->progressBar_4, calculate_bar_position(EnergyCalculator::band_4_energy));
    move_bar(ui->progressBar_5, calculate_bar_position(EnergyCalculator::band_5_energy));
    move_bar(ui->progressBar_6, calculate_bar_position(EnergyCalculator::band_6_energy));
    move_bar(ui->progressBar_7, calculate_bar_position(EnergyCalculator::band_7_energy));
    move_bar(ui->progressBar_8, calculate_bar_position(EnergyCalculator::band_8_energy));
    move_bar(ui->progressBar_9, calculate_bar_position(EnergyCalculator::band_9_energy));
    move_bar(ui->progressBar_10, calculate_bar_position(EnergyCalculator::band_10_energy));

    EnergyCalculator::reset_energies(); // Resetea la energía de todas las bandas
  }
}

/*
  Mueve una barra de densidad espectral de acuerdo a
  la energía de una banda de frecuencia.
  @param bar: Puntero a la barra que se desea mover
  @param value: Energía de la banda de frecuencia
*/
void MainWindow::move_bar(QProgressBar *bar, const float value)
{
  int bar_value = bar->value();     // Valor actual de la barra
  if (abs(bar_value - value) > 150) // Verifica que la diferencia sea de al menos 150
  {
    while (bar_value < value) // Sube la barra de 1 en 1 hasta el valor deseado
    {
      bar_value += 1;
      bar->setValue(bar_value);
    }

    while (bar_value > value) // Baja la barra de 1 en 1 hasta el valor deseado
    {
      bar_value -= 1;
      bar->setValue(bar_value);
    }
  }
}

/*
  Calcula la posicion de la barra de densidad espectral
  de acuerdo a la energia
  @param x: Energía de la banda
  @returns Un número entre 0 y 10000 que representa la posicion de la barra
*/
float MainWindow::calculate_bar_position(float x)
{
  if (x <= 0.01)
  {
    return -100000 * x + 10000; // Función lineal entre los puntos (0, 10000) y (0.01, 9000)
  }
  else if (x <= 0.1)
  {
    return -11111.11 * x + 9111.11; // Función lineal entre los puntos (0.01, 9000) y (0.1, 8000)
  }
  else if (x <= 1)
  {
    return -1111.11 * x + 8111.11; // Función lineal entre los puntos (0.1, 8000) y (1, 7000)
  }
  else if (x <= 5.86)
  {
    return -411.52 * x + 7411.52; // Función lineal entre los puntos (1, 7000) y (5.86, 5000)
  }
  else
  {
    // Función exponencial a partir del punto (5.86, 5000). Asíntota a 0
    return 5000 * exp(-x / 150);
  }
}

void MainWindow::on_fileButton_clicked()
{
  selectedFiles_ =
      QFileDialog::getOpenFileNames(this,
                                    "Select one or more audio files to open",
                                    ui->fileEdit->text(),
                                    "WAV Files (*.wav)");

  if (!selectedFiles_.empty())
  {
    ui->fileEdit->setText(*selectedFiles_.begin());

    jack::stopFiles();
    QStringList::iterator it;
    for (it = selectedFiles_.begin(); it != selectedFiles_.end(); ++it)
    {
      std::string tmp(qPrintable(*it));
      jack::playAlso(tmp.c_str());
    }
  }
}

void MainWindow::on_fileEdit_returnPressed()
{
  jack::stopFiles();
  std::string tmp(qPrintable(ui->fileEdit->text()));
  if (!tmp.empty())
  {
    jack::playAlso(tmp.c_str());
  }
}

/*
  Cambia la ganancia de todos los sliders
  @param values: Array con los valores que tomarán
  los sliders. La posición de cada valor se corresponde
  con el orden en que se colocaron los sliders.
*/
void MainWindow::set_sliders(const float values[10])
{
  ui->bandSlider_1->setValue(values[0]);
  ui->bandSlider_2->setValue(values[1]);
  ui->bandSlider_3->setValue(values[2]);
  ui->bandSlider_4->setValue(values[3]);
  ui->bandSlider_5->setValue(values[4]);
  ui->bandSlider_6->setValue(values[5]);
  ui->bandSlider_7->setValue(values[6]);
  ui->bandSlider_8->setValue(values[7]);
  ui->bandSlider_9->setValue(values[8]);
  ui->bandSlider_10->setValue(values[9]);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música clásica
*/
void MainWindow::on_classical_button_clicked()
{
  const float slider_values[10] = {-168, -168, -168, 0, 0, 157, 442, 442, 442, 442};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música jazz
*/
void MainWindow::on_jazz_button_clicked()
{
  const float slider_values[10] = {157, 305, 442, 442, 305, 157, 157, 0, 0, 0};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música dance
*/
void MainWindow::on_dance_button_clicked()
{
  const float slider_values[10] = {305, 305, 442, 442, 442, 0, 0, 0, 442, 442};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música pop
*/
void MainWindow::on_pop_button_clicked()
{
  const float slider_values[10] = {0, 0, -168, 305, 442, 305, -168, 0, 0, 0};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música rock
*/
void MainWindow::on_rock_button_clicked()
{
  const float slider_values[10] = {305, 442, 305, 0, 305, 157, 305, 0, 0, 0};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música reggae
*/
void MainWindow::on_reggae_button_clicked()
{
  const float slider_values[10] = {305, 442, 442, 157, 0, 305, 0, 0, 0, 0};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para música techno
*/
void MainWindow::on_techno_button_clicked()
{
  const float slider_values[10] = {157, 305, 692, 442, 0, -168, 0, 157, 305, 442};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para el modo full bass
*/
void MainWindow::on_full_bass_button_clicked()
{
  const float slider_values[10] = {571, 692, 806, 571, 157, -168, -168, -168, -168, -168};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para el modo full treble
*/
void MainWindow::on_full_treble_button_clicked()
{
  const float slider_values[10] = {-168, -168, -168, -168, 0, 0, 157, 442, 692, 806};
  set_sliders(slider_values);
}

/*
  Cambia las ganancia de las bandas según se defina
  tras presionar el botón para el modo full bass & treble
*/
void MainWindow::on_full_bass_and_treble_button_clicked()
{
  const float slider_values[10] = {571, 692, 806, 571, -168, 0, 157, 442, 692, 806};
  set_sliders(slider_values);
}

/*
  Coloca las ganancias de todas las bandas en
  el valor inicial (ganancia 1).
*/
void MainWindow::on_reset_button_clicked()
{
  const float slider_values[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  set_sliders(slider_values);
}

/*
  Cambia el volumen global del sistema de procesamiento
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_volume_slider_changed(int value)
{
  FilterBank::set_global_volume(value);
}

/*
  Cambia el volumen de la banda de paso 1
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_1_slider_changed(int value)
{
  FilterBank::band_1->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 2
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_2_slider_changed(int value)
{
  FilterBank::band_2->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 3
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_3_slider_changed(int value)
{
  FilterBank::band_3->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 4
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_4_slider_changed(int value)
{
  FilterBank::band_4->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 5
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_5_slider_changed(int value)
{
  FilterBank::band_5->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 6
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_6_slider_changed(int value)
{
  FilterBank::band_6->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 7
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_7_slider_changed(int value)
{
  FilterBank::band_7->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 8
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_8_slider_changed(int value)
{
  FilterBank::band_8->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 9
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_9_slider_changed(int value)
{
  FilterBank::band_9->set_volume(value);
}

/*
  Cambia el volumen de la banda de paso 10
  cada vez que el slider se mueve

  @param value: Valor actual del slider (entre 0 y 100)
*/
void MainWindow::on_band_10_slider_changed(int value)
{
  FilterBank::band_10->set_volume(value);
}