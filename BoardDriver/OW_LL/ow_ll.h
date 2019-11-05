/**
 * \file            ow_ll.h
 * \brief           OneWire USART application
 */

/*
 * Copyright (c) 2019 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of OneWire-UART library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         $_version_$
 */
#ifndef OW_HDR_LL_H
#define OW_HDR_LL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stddef.h>
#include "../OW_LL/ow.h"

/**
 * \defgroup        OW_LL Low-Level functions
 * \brief           Low-level device dependant functions
 * \{
 */

#define ONEWIRE_USART                          USART3
#define ONEWIRE_USART_CLK_EN                   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

/* USART TX PIN */
#define ONEWIRE_TX_PORT                         GPIOC
#define ONEWIRE_TX_PORT_CLK_EN                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#define ONEWIRE_TX_PIN                          LL_GPIO_PIN_10
#define ONEWIRE_TX_PIN_AF                       LL_GPIO_AF_7

/* USART RX PIN */
#define ONEWIRE_RX_PORT                         GPIOC
#define ONEWIRE_RX_PORT_CLK_EN                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#define ONEWIRE_RX_PIN                          LL_GPIO_PIN_11
#define ONEWIRE_RX_PIN_AF                       LL_GPIO_AF_7

uint8_t     ow_ll_init(void* arg);
uint8_t     ow_ll_deinit(void* arg);
uint8_t     ow_ll_set_baudrate(uint32_t baudrate, void* arg);
uint8_t     ow_ll_transmit_receive(const uint8_t* tx, uint8_t* rx, size_t len, void* arg);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OW_HDR_LL_H */
