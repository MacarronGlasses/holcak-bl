#ifndef DRIVER_PIC_H_
#define DRIVER_PIC_H_
#include <stdbool.h>
#include <stdint.h>

#define PIC1_OFFSET            0x20
#define PIC2_OFFSET            (PIC1_OFFSET + 0x08)

#define PIC1_CMD               0x20
#define PIC1_DATA              0x21
#define PIC2_CMD               0xA0
#define PIC2_DATA              0xA1

#define PIC_ICW1_ICW4          0x01
#define PIC_ICW1_EDGE          0x00
#define PIC_ICW1_SINGLE        0x02
#define PIC_ICW1_INTERVAL4     0x04
#define PIC_ICW1_LEVEL         0x08
#define PIC_ICW1_INIT          0x10

#define PIC_ICW4_8088          0x00
#define PIC_ICW4_8086          0x01
#define PIC_ICW4_AUTO_EOI      0x02
#define PIC_ICW4_BUFFER_MASTER 0x04
#define PIC_ICW4_BUFFER_SLAVE  0x00
#define PIC_ICW4_BUFFER        0x08
#define PIC_ICW4_SFNM          0x10

#define PIC_CMD_EOI            0x20
#define PIC_CMD_IRR            0x0A
#define PIC_CMD_ISR            0x0B

void pic_init(void);
void pic_stop(void);
void pic_mask(uint8_t irq, bool mask);
void pic_eoi(uint8_t irq);
uint16_t pic_irr(void);
uint16_t pic_isr(void);

#endif//DRIVER_PIC_H_