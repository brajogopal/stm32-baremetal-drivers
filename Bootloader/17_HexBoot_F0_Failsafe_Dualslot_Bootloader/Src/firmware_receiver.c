/*
 * firmware_receiver.c
 *
 * Receives firmware using DMA.
 * Parses the firmware header.
 * Controls the firmware receive state machine.
 */


/* Bootloader */
#include "firmware_receiver.h"
#include "firmware_pingpong.h"


/* Drivers */
#include "uart.h"
#include "dma.h"


#define FW_HEADER 0xAA


/* Firmware Header */
static FW_Header_t fw_header;


/* Firmware Information */
uint16_t payload_length_A;
uint16_t expected_crc_A;
uint16_t payload_length_B;
uint16_t expected_crc_B;

static uint16_t payload_length;
static uint16_t expected_crc;


/* Reception Status */
volatile uint32_t header_received;
volatile uint32_t bytes_received;
volatile uint8_t program_chunk_size;


/* DMA */
static uint16_t current_chunk_size;
static volatile uint8_t rx_data_ready;


/* State */
static volatile fw_rx_state_t rx_state = PARSE_HEADER;


/* Private Functions Prototypes */
static uint16_t firmware_get_next_chunk_size(void);


/******************************************************************************
 * Public Functions
 ******************************************************************************/


/* Configure DMA to receive the firmware header. */
void firmware_receiver_init(void)
{
    dma_receive(&fw_header, sizeof(FW_Header_t));
}


void firmware_receiver_dma_callback(uint8_t value)
{
    rx_data_ready = value;
}


void firmware_rx_process(void)
{
    switch (rx_state)
    {
        case PARSE_HEADER:
        	/* Validate header and extract firmware information */
        	if (fw_header.header == FW_HEADER)
        	{
        		payload_length_A = fw_header.payload_length_A;
        		payload_length_B = fw_header.payload_length_B;

        		expected_crc_A = fw_header.crc_A;
        		expected_crc_B = fw_header.crc_B;

        		header_received = 1;
        		rx_state = START_PAYLOAD_DMA;
        	}
        	else
        	{
        		rx_state = FW_ERROR;
        	}
            break;

        case START_PAYLOAD_DMA:
        	/* Start DMA reception */
        	current_chunk_size = firmware_get_next_chunk_size();
        	dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
        	rx_state =  RECEIVE_PAYLOAD;
        	break;

        case RECEIVE_PAYLOAD:
        	/* Process received DMA chunk */
        	program_chunk_size = current_chunk_size;
        	swap_buffers(&fw_pingpong);
        	bytes_received += current_chunk_size;
        	fw_pingpong.prog_ready = 1;
        	current_chunk_size = firmware_get_next_chunk_size();

            if (current_chunk_size > 0)
            {
                dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
            }
            else
            {
                rx_state = FW_COMPLETE;
                println("firmware receive complete");
            }
        	break;

        case FW_COMPLETE:
        	/* Reception complete */
        	println("fw complete");
            break;

        case FW_ERROR:
        	/* Invalid firmware */
        	println("fw error");
            break;
    }
}


fw_rx_state_t firmware_rx_get_state(void)
{
    return rx_state;
}


uint8_t get_rx_update(void)
{
	return rx_data_ready;
}


void firmware_set_payload_info(uint16_t length, uint16_t crc)
{
    payload_length = length;
    expected_crc   = crc;
}




/******************************************************************************
 * Private Functions
 ******************************************************************************/
static uint16_t firmware_get_next_chunk_size(void)
{
    uint32_t remaining;

    remaining = payload_length - bytes_received;

    if (remaining > FW_BUFFER_SIZE)
    {
        return FW_BUFFER_SIZE;
    }

    return remaining;
}
