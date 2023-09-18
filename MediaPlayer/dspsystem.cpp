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
 * \file   freqFilter.h
 *         Implements filtering in the frequency domain
 * \author Pablo Alvarado
 * \date   2010.12.12
 *
 * $Id: equalizer.cpp $
 */

#include "dspsystem.h"
#include <cstring>

#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif

dspSystem::dspSystem()
    : sampleRate_(0), bufferSize_(0)
{
}

dspSystem::~dspSystem()
{
}

/**
 * Initialization function for the current filter plan
 */
bool dspSystem::init(const int sampleRate, const int bufferSize)
{
  _debug("dspSystem::init()" << std::endl);

  sampleRate_ = sampleRate;
  bufferSize_ = bufferSize;

  return true;
}

/**
 * Processing function
 */
bool dspSystem::process(float *in, float *out)
{
  for (int i = 0; i < bufferSize_; i++) // Recorre todas las muestras del bloque actual
  {
    in[i] = FilterBank::filter_all_bands(in[i]); // Filtra las muestras una a una
  }

  // Indica que se han procesado todas las muestras del buffer para actualizar el gráfico
  EnergyCalculator::ready = true;

  // Copia los valores de la entrada procesada en la salida
  memcpy(out, in, bufferSize_ * sizeof(float));

  return true;
}

/**
 * Shutdown the processor
 */
bool dspSystem::shutdown()
{
  return true;
}

/**
 * Set buffer size (call-back)
 */
int dspSystem::setBufferSize(const int bufferSize)
{
  bufferSize_ = bufferSize;
  return 1;
}

/**
 * Set sample rate (call-back)
 */
int dspSystem::setSampleRate(const int sampleRate)
{
  sampleRate_ = sampleRate;
  return 1;
}