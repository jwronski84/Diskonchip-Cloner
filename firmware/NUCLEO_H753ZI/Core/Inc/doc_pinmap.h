#pragma once

/*
 * Preliminary MD2202-D16 pin map.
 *
 * Final STM32 port/pin assignments must be generated in STM32CubeMX for
 * NUCLEO-H753ZI after confirming which FMC-capable pins are actually exposed
 * and conflict-free on the Nucleo morpho connectors.
 */

#define DOC_PINOUT_CONFIRMED_FROM_DATASHEET 1

/* MD2202 socket pins */
#define MD2202_PIN_A12 4
#define MD2202_PIN_A7  5
#define MD2202_PIN_A6  6
#define MD2202_PIN_A5  7
#define MD2202_PIN_A4  8
#define MD2202_PIN_A3  9
#define MD2202_PIN_A2  10
#define MD2202_PIN_A1  11
#define MD2202_PIN_A0  12
#define MD2202_PIN_D0  13
#define MD2202_PIN_D1  14
#define MD2202_PIN_D2  15
#define MD2202_PIN_VSS 16
#define MD2202_PIN_D3  17
#define MD2202_PIN_D4  18
#define MD2202_PIN_D5  19
#define MD2202_PIN_D6  20
#define MD2202_PIN_D7  21
#define MD2202_PIN_CE_N 22
#define MD2202_PIN_A8  23
#define MD2202_PIN_OE_N 24
#define MD2202_PIN_A9  25
#define MD2202_PIN_A11 26
#define MD2202_PIN_A10 27
#define MD2202_PIN_WE_N 31
#define MD2202_PIN_VCC 32

/* NC pins: 1, 2, 3, 28, 29, 30. Leave floating. */
