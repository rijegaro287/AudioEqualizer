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
 * \file   mainwindow.h
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado
 * \date   2010.12.12
 *
 * $Id: mainwindow.h $
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QProgressBar>
#include <unistd.h>

#include "dspsystem.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  /**
   *Tolerance value
   */
  static const float Epsilon;

  /**
   * Verbose flag
   */
  bool verbose_;

  /**
   * Timer used to recompute the filter once the user changes the
   * values
   */
  QTimer *timer_;

  /**
   * List of selected files so far
   */
  QStringList selectedFiles_;

  /**
   * Pointer to an inherited class of processor, which does
   * all the real work.
   */
  dspSystem *dsp_;

private slots:
  void on_fileEdit_returnPressed();
  void on_fileButton_clicked();

  void on_volume_slider_changed(int value);
  void on_band_1_slider_changed(int value);
  void on_band_2_slider_changed(int value);
  void on_band_3_slider_changed(int value);
  void on_band_4_slider_changed(int value);
  void on_band_5_slider_changed(int value);
  void on_band_6_slider_changed(int value);
  void on_band_7_slider_changed(int value);
  void on_band_8_slider_changed(int value);
  void on_band_9_slider_changed(int value);
  void on_band_10_slider_changed(int value);

  void set_sliders(const float values[10]);

  void on_classical_button_clicked();
  void on_jazz_button_clicked();
  void on_dance_button_clicked();
  void on_pop_button_clicked();
  void on_rock_button_clicked();
  void on_reggae_button_clicked();
  void on_techno_button_clicked();
  void on_full_bass_button_clicked();
  void on_full_treble_button_clicked();
  void on_full_bass_and_treble_button_clicked();

  void on_reset_button_clicked();

  void update();
  void move_bar(QProgressBar *bar, const float value);
  float calculate_bar_position(float x);
};

#endif // MAINWINDOW_H
