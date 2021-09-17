/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#ifndef EEMBED_ARDUINO_H
#define EEMBED_ARDUINO_H 1

#include "eembed.h"

extern unsigned long eembed_die_delay;

void eembed_arduino_serial_log_init(void);

#endif /* EEMBED_ARDUINO_H */
