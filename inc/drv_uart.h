
#pragma once

#include "drv.h"
#include "os_hal_uart.h"
#include <string.h>

// Define to use DMA for the UART Receive data, currently not working 7/6/22
//#define USE_RX_DMA

typedef enum
{
    UART_SUCCESS = 0,
    UART_ERROR = (-1)
} uart_err_t;

typedef enum
{
    UART_DATA_BITS_7 = UART_DATA_7_BITS,
    UART_DATA_BITS_8 = UART_DATA_8_BITS,
    // 9 bits not supported by MTK drivers

    UART_DATA_BITS_DEFAULT = UART_DATA_BITS_8
} uart_data_bits_t;

typedef enum
{
    UART_PARITY_NONE = 0x00,
    UART_PARITY_EVEN = 0x18,
    UART_PARITY_ODD = 0x08,
    // Mark parity not supported by MTK drivers
    // Space parity not supported by MTK drivers

    UART_PARITY_DEFAULT = UART_PARITY_NONE
} uart_parity_t;

typedef enum
{
    //UART_STOP_BITS_HALF = 0, Not supported by MKT drivers
    UART_STOP_BITS_ONE = UART_STOP_1_BIT,
    //UART_STOP_BITS_ONE_AND_A_HALF, Not supported by MKT drivers
    UART_STOP_BITS_TWO = UART_STOP_2_BIT,

    UART_STOP_BITS_DEFAULT = UART_STOP_BITS_ONE
} uart_stop_bits_t;

typedef enum
{
    UART_FLOW_CONTROL_NONE = 0x00,
    UART_FLOW_CONTROL_HW,
    UART_FLOW_CONTROL_SW,

    UART_FLOW_CONTROL_DEFAULT = UART_FLOW_CONTROL_SW
} uart_flow_control_t;

typedef struct
{
    pin_name_t tx_pin;
    pin_name_t rx_pin;

    uint32_t baud;
    uart_data_bits_t data_bits;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;

// TODO Implement Ring Buffer
    ring_buf8_t tx_buf;
    ring_buf8_t rx_buf;

    size_t tx_ring_size;
    size_t rx_ring_size;
} uart_config_t;

typedef struct
{
    handle_t handle;

    uart_config_t config;

    uint8_t *tx_ring_buffer;
    uint8_t *rx_ring_buffer;

    bool is_tx_irq_enabled;
    bool is_rx_irq_enabled;

    bool is_blocking;
} uart_t;

void uart_configure_default( uart_config_t *config );

err_t uart_open( uart_t *obj, uart_config_t *config );

err_t uart_set_baud( uart_t *obj, uint32_t baud );

err_t uart_set_parity( uart_t *obj, uart_parity_t parity );

err_t uart_set_stop_bits( uart_t *obj, uart_stop_bits_t stop );

err_t uart_set_data_bits( uart_t *obj, uart_data_bits_t bits );

void uart_set_blocking( uart_t *obj, bool blocking );

err_t uart_write( uart_t *obj, char *buffer, size_t size );

err_t uart_print( uart_t *obj, char *text );

err_t uart_println( uart_t *obj, char *text );

err_t uart_read( uart_t *obj, char *buffer, size_t size );

size_t uart_bytes_available( uart_t *obj );

void uart_clear( uart_t *obj );

void uart_close( uart_t *obj );

err_t uart_set_flow_control(uart_t *obj, uart_flow_control_t fc);

