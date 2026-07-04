/*
 * firmware_receiver.c
 *
 * Project : HexBoot_F0_DMA_PingPong_IAP
 *
 * Description:
 * Implements the firmware reception state machine for the bootloader.
 *
 * Unlike previous projects that received firmware one byte at a time
 * using UART interrupts, this implementation receives both the firmware
 * header and payload using DMA.
 *
 * The receiver operates as an event-driven state machine. Each invocation
 * performs a single state transition, while DMA transfer completion events
 * drive the reception process forward.
 *
 * Responsibilities:
 * - Receive the firmware header.
 * - Validate and parse header information.
 * - Configure DMA transfers.
 * - Manage Ping-Pong buffer reception.
 * - Notify the Flash programmer when a buffer is ready.
 *
 * Notes:
 * DMA is responsible only for moving bytes between UART and memory.
 * This module interprets those bytes and controls the firmware update flow.
 */



/* Bootloader */
#include "firmware_receiver.h"
#include "firmware_pingpong.h"

/* Drivers */
#include "uart.h"
#include "dma.h"


#define FW_HEADER 0xAA

static volatile fw_rx_state_t rx_state = RECEIVE_HEADER;


/******************************************************************************
 * Global Variables
 ******************************************************************************/
uint16_t payload_length = 0;
uint16_t expected_crc = 0;
volatile uint8_t program_chunk_size = 0;
volatile uint32_t header_received = 0;
volatile uint32_t bytes_received = 0;


/******************************************************************************
 * Static Variables
 ******************************************************************************/
uint16_t current_chunk_size = 0;
static FW_Header_t fw_header;



/******************************************************************************
 * Public Functions
 ******************************************************************************/
void firmware_rx_process(void)
{
    switch(rx_state)
    {

    	/*
    	 * RECEIVE_HEADER
    	 *
    	 * Configure DMA to receive the fixed-size firmware header.
    	 *
    	 * The firmware header contains the information required to receive
    	 * the remaining firmware image, including firmware size and CRC.
    	 *
    	 * Once DMA reception completes, the state advances to PARSE_HEADER.
    	 */
        case RECEIVE_HEADER:
        	dma_receive(&fw_header, sizeof(FW_Header_t));
        	rx_state = PARSE_HEADER;
            break;



            /*
             * PARSE_HEADER
             *
             * Validate the received firmware header and extract:
             *
             * - Firmware Size
             * - Expected CRC
             *
             * If the header is valid, the Flash erase process can begin and the
             * receiver prepares for payload reception.
             */
        case PARSE_HEADER:
        	if(fw_header.header == FW_HEADER){
        		payload_length = fw_header.payload_length;
        		expected_crc = fw_header.crc;
        		header_received = 1;
        		rx_state = START_PAYLOAD_DMA;
        	}	else
        	{
        		rx_state = FW_ERROR;
        	}
            break;



            /*
             * START_RECEIVE_PAYLOAD
             *
             * Configure DMA to receive the first firmware chunk.
             *
             * The transfer size is calculated dynamically based on the remaining
             * firmware bytes.
             *
             * After DMA is configured, the receiver waits for a DMA completion
             * interrupt before processing the received data.
             */
        case START_PAYLOAD_DMA:
        	current_chunk_size = firmware_get_next_chunk_size();
        	dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
        	rx_state =  RECEIVE_PAYLOAD;
        	break;



        	/*
        	 * RECEIVE_PAYLOAD
        	 *
        	 * Process the firmware chunk received by DMA.
        	 *
        	 * Operations performed:
        	 * - Swap Ping-Pong buffers.
        	 * - Update received byte count.
        	 * - Notify Flash programmer.
        	 * - Configure DMA for the next chunk.
        	 *
        	 * Reception continues until all firmware bytes have been received.
        	 */
        case RECEIVE_PAYLOAD:
        	program_chunk_size = current_chunk_size;
        	swap_buffers(&fw_pingpong);
        	bytes_received += current_chunk_size;
        	fw_pingpong.prog_ready = 1;
        	current_chunk_size = firmware_get_next_chunk_size();


            if(current_chunk_size > 0)
            {
                dma_receive(fw_pingpong.rx_buffer, current_chunk_size);
            }
            else
            {
                rx_state = FW_COMPLETE;
                println("firmware receive complete");
            }

        	break;



        	/*
        	 * FW_COMPLETE
        	 *
        	 * Firmware reception has completed successfully.
        	 *
        	 * The application can now:
        	 *
        	 * - Verify CRC.
        	 * - Update metadata.
        	 * - Boot the application.
        	 */
        case FW_COMPLETE:
        	/* Reserved for future use */

            break;



        case FW_ERROR:
        	/* Reserved for future error handling */

            break;
    }
}


fw_rx_state_t firmware_rx_get_state(void)
{
    return rx_state;
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/
uint16_t firmware_get_next_chunk_size(void)
{
    uint32_t remaining;

    remaining = payload_length - bytes_received;

    if(remaining > FW_BUFFER_SIZE)
    {
        return FW_BUFFER_SIZE;
    }

    return remaining;
}




