#include "swspi.h"
#include <util/delay.h>

#define SCLK_CYCLE() _delay_us(0.001)
#define SCLK_HI() SCLK_PORT |= _BV(SCLK_PIN)
#define SCLK_LO() SCLK_PORT &= ~_BV(SCLK_PIN)
#define MOSI_HI() MOSI_PORT |= _BV(MOSI_PIN)
#define MOSI_LO() MOSI_PORT &= ~_BV(MOSI_PIN)
#define GETMISO() MISO_PORT & _BV(MISO_PIN)

uint8_t spi_transfer_lm(uint8_t data) {
    uint8_t nbit, res = 0x00;
    for (nbit = 0; nbit < 8; nbit++) {
        if (data & _BV(nbit)) MOSI_HI();
        else MOSI_LO();
        SCLK_LO();
        SCLK_CYCLE();
        if (GETMISO()) res |= _BV(nbit);
        SCLK_HI();
        SCLK_CYCLE();
    }
    SCLK_LO();
    SCLK_CYCLE();
    return res;
}

uint8_t spi_transfer_ml(uint8_t data) {
    uint8_t nbit, res = 0x00;
    for (nbit = 0; nbit < 8; nbit++) {
        if (data & _BV(7 - nbit)) MOSI_HI();
        else MOSI_LO();
        SCLK_LO();
        SCLK_CYCLE();
        if (GETMISO()) res |= _BV(7 - nbit);
        SCLK_HI();
        SCLK_CYCLE();
    }
    SCLK_LO();
    SCLK_CYCLE();
    return res;
}

uint8_t spi_transfer(uint8_t data) {
    return spi_transfer_ml(data);
}

void spi_ntransfer(uint8_t *dataout, uint8_t *datain, int len) {
    uint8_t nbyte;
    for (nbyte = 0; nbyte < len; nbyte++) {
        datain[nbyte] = spi_transfer(dataout[nbyte]);
    }
}
