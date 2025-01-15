#include "mfrc522.h"
#include "spi.h"
#include "MKL05Z4.h"
#include <stdint.h>

#define MFRC522_SPI_SPEED 1000000 // 1 MHz

// Inicjalizacja MFRC522
mfrc522_status_t mfrc522_init(void) {
    if (spi_init(SPI_MODE_0, MFRC522_SPI_SPEED, SPI_SIDE_MASTER) != SPI_OK) {
        return MFRC522_ERR;
    }

    // Konfiguracja pinu Chip Select (CS) na PTA4
    PORTA->PCR[MFRC522_CS_PIN] = PORT_PCR_MUX(1);  // Ustaw PTA4 jako GPIO
    PTA->PDDR |= (1 << MFRC522_CS_PIN);             // Ustaw PTA4 jako wyjœcie
    PTA->PSOR = (1 << MFRC522_CS_PIN);              // Ustaw CS na wysoki (nieaktywny)

    // Konfiguracja pinu Reset (Reset) na PTA1
    PORTA->PCR[MFRC522_RESET_PIN] = PORT_PCR_MUX(1);  // Ustaw PTA1 jako GPIO
    PTA->PDDR |= (1 << MFRC522_RESET_PIN);             // Ustaw PTA1 jako wyjœcie
    PTA->PSOR = (1 << MFRC522_RESET_PIN);              // Ustaw Reset na wysoki (modu³ nie jest zresetowany)

    // Reset modu³u MFRC522
    mfrc522_reset();

    // Ustawienia domyœlne
    mfrc522_write(MFRC522_REG_COMMAND, MFRC522_CMD_IDLE);  // Set to idle mode
    mfrc522_write(MFRC522_REG_MODE, 0x3D);                 // Typical Mode
    mfrc522_write(MFRC522_REG_TX_CONTROL, 0x83);           // Antenna ON

    return MFRC522_OK;
}

// Funkcja do odczytu z rejestru MFRC522
uint8_t mfrc522_read(uint8_t reg) {
    uint8_t address = ((reg << 1) & 0x7E) | 0x80;
    mfrc522_select();  // Wybieramy urz¹dzenie (CS = LOW)
    spi_master_write(address);
    uint8_t data = spi_master_write(0x00);  // Dummy write to read data
    mfrc522_deselect();  // Zwalniamy urz¹dzenie (CS = HIGH)
    return data;
}

// Funkcja do zapisu do rejestru MFRC522
void mfrc522_write(uint8_t reg, uint8_t value) {
    uint8_t address = (reg << 1) & 0x7E;
    mfrc522_select();  // Wybieramy urz¹dzenie (CS = LOW)
    spi_master_write(address);
    spi_master_write(value);
    mfrc522_deselect();  // Zwalniamy urz¹dzenie (CS = HIGH)
}

// Funkcja do sprawdzenia wersji uk³adu MFRC522
uint8_t mfrc522_get_version(void) {
    return mfrc522_read(MFRC522_REG_VERSION);
}

// Funkcja do resetowania MFRC522
void mfrc522_reset(void) {
    PTA->PCOR = (1 << MFRC522_RESET_PIN);  // Reset (stan niski)
    for (volatile int i = 0; i < 10000; i++) {  // OpóŸnienie
        __NOP();
    }
    PTA->PSOR = (1 << MFRC522_RESET_PIN);  // Modu³ przestaje byæ zresetowany (stan wysoki)
}

// Funkcja do aktywacji CS
void mfrc522_select(void) {
    PTA->PCOR = (1 << MFRC522_CS_PIN); // Aktywacja CS (stan niski)
}

// Funkcja do dezaktywacji CS
void mfrc522_deselect(void) {
    PTA->PSOR = (1 << MFRC522_CS_PIN); // Dezaktywacja CS (stan wysoki)
}
