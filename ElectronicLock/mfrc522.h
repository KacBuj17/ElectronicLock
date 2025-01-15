#ifndef MFRC522_H
#define MFRC522_H

#include <stdint.h>
#include "spi.h"

// Pin definitions
#define MFRC522_CS_PIN      8
#define MFRC522_RESET_PIN   0

// Definicje rejestrów MFRC522
#define MFRC522_REG_COMMAND     0x01
#define MFRC522_REG_FIFO_DATA   0x09
#define MFRC522_REG_FIFO_LEVEL  0x0A
#define MFRC522_REG_CONTROL     0x0C
#define MFRC522_REG_BIT_FRAMING 0x0D
#define MFRC522_REG_STATUS2     0x08
#define MFRC522_REG_MODE        0x11
#define MFRC522_REG_TX_CONTROL  0x14
#define MFRC522_REG_VERSION     0x37

// Komendy
#define MFRC522_CMD_IDLE        0x00
#define MFRC522_CMD_MEM         0x01
#define MFRC522_CMD_GEN_RANDOM  0x02
#define MFRC522_CMD_TRANSCEIVE  0x0C

// Status operacji
typedef enum {
    MFRC522_OK,
    MFRC522_ERR
} mfrc522_status_t;


mfrc522_status_t mfrc522_init(void);
uint8_t mfrc522_read(uint8_t reg);
void mfrc522_write(uint8_t reg, uint8_t value);
uint8_t mfrc522_get_version(void);
void mfrc522_select(void);
void mfrc522_deselect(void);
void mfrc522_reset(void);
#endif
