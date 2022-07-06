#include "drv_uart.h"

static uart_t *_owner = NULL;
static const uint32_t uart_dma_timeout = 250; // 100 ms

/* DMA write/read buffers need to be located in sysram */
#define DMA_BUF_SIZE 128
__attribute__((section(".sysram"))) uint8_t tx_dma_buf[DMA_BUF_SIZE];
#ifdef USE_RX_DMA
__attribute__((section(".sysram"))) uint8_t rx_dma_buf[DMA_BUF_SIZE];
#endif 

static err_t _acquire( uart_t *obj, bool obj_open_state )
{
    err_t status = ACQUIRE_SUCCESS;

    if ( obj_open_state == true && _owner == obj )
    {
        return ACQUIRE_FAIL;
    }

    if ( _owner != obj )
    {
        int ret = mtk_os_hal_uart_ctlr_init(obj->config.rx_pin);
         
        if( ret != 0){
            return ACQUIRE_FAIL;
        }

        if ( status != ACQUIRE_FAIL )
            _owner = obj;
    }

    return status;
}

void uart_configure_default( uart_config_t *config )
{
    config->tx_pin = 0xFFFFFFFF;
    config->rx_pin = 0xFFFFFFFF;

    config->baud = 115200;
    config->data_bits = UART_DATA_BITS_DEFAULT;
    config->parity = UART_PARITY_DEFAULT;
    config->stop_bits = UART_STOP_BITS_DEFAULT; // UART_PARITY_DEFAULT;

    // Allocate a dma buffer for uart tx data
    if(tx_dma_buf != NULL){
        memset(tx_dma_buf, 0x00, DMA_BUF_SIZE);
    }        

#ifdef USE_RX_DMA
    // Allocate a dma buffer for uart rx data
    if(rx_dma_buf != NULL){
        memset(rx_dma_buf, 0x00, DMA_BUF_SIZE);
    } 
#endif 

// TODO Implement Ring Buffer
//    memset( &config->tx_buf, 0x00, sizeof( ring_buf8_t ) );
//    memset( &config->rx_buf, 0x00, sizeof( ring_buf8_t ) );

//    config->tx_ring_size = 0;
//    config->rx_ring_size = 0;
}

err_t uart_open( uart_t *obj, uart_config_t *config )
{
    uart_config_t *p_config = &obj->config;
    memcpy( p_config, config, sizeof( uart_config_t ) );

    return _acquire( obj, true );
}

err_t uart_set_baud( uart_t *obj, uint32_t baud )
{
    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.baud = baud;
        mtk_os_hal_uart_set_baudrate(obj->config.rx_pin, obj->config.baud);
        return UART_SUCCESS;
    } else {
        return UART_ERROR;
    }
}

err_t uart_set_parity( uart_t *obj, uart_parity_t parity )
{

    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.parity = parity;
        mtk_os_hal_uart_set_format(obj->config.rx_pin, obj->config.data_bits, obj->config.parity, obj->config.stop_bits);
        return UART_SUCCESS;
    } else {
        return UART_ERROR;
    }
}

err_t uart_set_stop_bits( uart_t *obj, uart_stop_bits_t stop )
{
    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.stop_bits = stop;
        mtk_os_hal_uart_set_format(obj->config.rx_pin, obj->config.data_bits, obj->config.parity, obj->config.stop_bits);
        return UART_SUCCESS;
    } else {
        return UART_ERROR;
    }
}

err_t uart_set_data_bits( uart_t *obj, uart_data_bits_t bits )
{
    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        obj->config.data_bits = bits;
        mtk_os_hal_uart_set_format(obj->config.rx_pin, obj->config.data_bits, obj->config.parity, obj->config.stop_bits);
        return UART_SUCCESS;
    } else {
        return UART_ERROR;
    }
}

void uart_set_blocking( uart_t *obj, bool blocking )
{
    obj->is_blocking = blocking;
}

err_t uart_write( uart_t *obj, char *buffer, size_t size )
{
    size_t data_written = 0;

    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {
        
        if((size <= DMA_BUF_SIZE) && (buffer != NULL) && (tx_dma_buf != NULL)){
    
            memcpy(tx_dma_buf, buffer, size);
            data_written = mtk_os_hal_uart_dma_send_data(obj->config.rx_pin, (u8 *)tx_dma_buf, size, 0, uart_dma_timeout);
            if (data_written < 0){
                printf("UART Tx(DMA) Error! (%ld)\n", data_written);
            }
            return data_written;
        }
        else{
            return UART_ERROR;
        }
    } else {
        return UART_ERROR;
    }
}

err_t uart_read( uart_t *obj, char *buffer, size_t size )
{
    size_t data_read = 0;

    if ( _acquire( obj, false ) != ACQUIRE_FAIL )
    {

#ifdef USE_RX_DMA

#error "This code is currently not working"

        if(DMA_BUF_SIZE < size){
            return UART_ERROR;
        }

        data_read = mtk_os_hal_uart_dma_get_data(obj->config.rx_pin, (u8 *)rx_dma_buf, size, 1, uart_dma_timeout);
        printf("Data Read: %d\n", data_read);


        if(data_read < 0){
            memcpy(buffer, rx_dma_buf, data_read);
        }

#else // Not DMA

        for(int i = 0; i < size; i++){
            if(obj->is_blocking){
                buffer[i] = mtk_os_hal_uart_get_char(obj->config.rx_pin);
            }
            else{
                buffer[i] = mtk_os_hal_uart_get_char_nowait(obj->config.rx_pin);
            }
            data_read++;
        }
#endif 

        return data_read;
    } else {
        return UART_ERROR;
    }
}

err_t uart_print( uart_t *obj, char *text )
{

    uart_write(obj, text, strnlen(text, DMA_BUF_SIZE));
    return UART_SUCCESS;
}

err_t uart_println( uart_t *obj, char *text )
{
    uart_write(obj, text, strnlen(text, DMA_BUF_SIZE));
    uart_write(obj, "\n", 1);
    return UART_SUCCESS;
}

size_t uart_bytes_available( uart_t *obj )
{

    //#warning "uart_bytes_available() not implemented"
    // TODO Implement Ring buffers
    printf("uart_bytes_available() is not implemented\n");
    return UART_ERROR;
}

void uart_clear( uart_t *obj )
{

    //#warning "uart_bytes_available() not implemented"
    // TODO Implement Ring buffers
    printf("uart_clear() is not implemented\n");
}

void uart_close( uart_t *obj )
{
    if(0 == mtk_os_hal_uart_ctlr_deinit(obj->config.rx_pin))
    {
        obj->handle = -1;
        _owner = NULL;
    }
}

err_t uart_set_flow_control(uart_t *obj, uart_flow_control_t fc){

    // Disable all flow control    
    mtk_os_hal_uart_set_hw_fc(obj->config.rx_pin, UART_EFR_HW_FC_DISABLE);
    mtk_os_hal_uart_disable_sw_fc(obj->config.rx_pin);

    switch(fc){
        case UART_FLOW_CONTROL_NONE:
            break;
        case UART_FLOW_CONTROL_HW:
            mtk_os_hal_uart_set_hw_fc(obj->config.rx_pin, UART_EFR_HW_FC_RTS_CTS);
            break;
        case UART_FLOW_CONTROL_SW:
            mtk_os_hal_uart_set_sw_fc(obj->config.rx_pin, 0x11, 0x13, 0x12, 0x14, 0x1B);
            break;
        default:
            break;
    }

    return UART_SUCCESS;
}
