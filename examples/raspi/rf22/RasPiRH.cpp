/*
 * Snootlab 
 *-----------------------------------------------------------------------------------------
 * Example of a communication between two Chistera-Pi or between a Chistera-Pi and a sensor
 * with a RFM95 like Snootlab's TeensyWiNo.
 * This is the receiver code, the Chistera-Pi receives a message with an address and a 
 * counter and it prints the informations on the prompt.
 *
 */

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define BOARD_SI4432  //Here we define the board type
#define RF_FREQUENCY  433.00
// Now we include RasPi_Boards.h so this will expose defined 
// constants with CS/IRQ/RESET/on board LED pins definition
#include "../RasPiBoards.h"

#include <RH_RF22.h>

RH_RF22 rf22(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24);;

int run = 1;

/* Signal the end of the software */
void sigint_handler(int signal)
{
    run = 0;
}

void setup()
{ 
    wiringPiSetupGpio();

    if (!rf22.init()) 
    {
        fprintf(stderr, "Init failed\n");
        exit(1);
    }

    /* Tx Power is from +5 to +20 dbm */
    rf22.setTxPower(10);
    /* There are different configurations
     * you can find in lib/radiohead/RH_RF95.h 
     * at line 437 
     */
    rf22.setModemConfig(RH_RF22::OOK_Rb1_2Bw75);
    rf22.setFrequency(RF_FREQUENCY); /* MHz */
}

void loop()
{

    /* If we receive one message we show on the prompt
     * the address of the sender and the Rx power.
     */
    if( rf22.available() ) 
    {
        uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf22.recv(buf, &len)) 
        {
            printf("from %d, number %d\n", buf[0], buf[1]);
        }
    }
}

int main(int argc, char **argv)
{ 
    signal(SIGINT, sigint_handler);

    setup();

    while( run )
    {
        loop();
        usleep(1);
    }

    return EXIT_SUCCESS;
}
