/*
 * EGR314 Team 302 — Laksh Subsystem PIC Version
 * Target: PIC18F57Q83 (PCB)
 * 
 * PIN ASSIGNMENTS:
 *   I2C1 SCL  = RC3 (pin 18)  — OLED + BNO055 shared bus
 *   I2C1 SDA  = RC4 (pin 23)  — OLED + BNO055 shared bus
 *   UART1 TX  = RC6 (pin 25)  — Shared UART bus
 *   UART1 RX  = RC7 (pin 26)  — Shared UART bus
 *   BTN_PAGE  = RA0 (pin 2)   — Cycle OLED pages (active LOW, pull-up)
 *   BTN_CAL   = RA1 (pin 3)   — Recalibrate IMU zero (active LOW, pull-up)
 *   MCLR      = RE3 (pin 1)   — 10kΩ pull-up to 3.3V required
 *   PGD       = RB7 (pin 40)  — SNAP programmer
 *   PGC       = RB6 (pin 39)  — SNAP programmer
 */

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/i2c_host/i2c1.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// ══════════════════════════════════════════
//  ADDRESSES
// ══════════════════════════════════════════
#define OLED_ADDR   0x3C
#define BNO_ADDR    0x28

// ══════════════════════════════════════════
//  PROTOCOL IDs  (Raunak's AZ..YB format)
// ══════════════════════════════════════════
#define MY_ID       'L'
#define MIHIR_ID    'M'
#define RAUNAK_ID   'R'
#define BROADCAST   'X'

#define MAX_PKT     64

// ══════════════════════════════════════════
//  I2C HELPERS
// ══════════════════════════════════════════
static void i2c_write(uint8_t addr, uint8_t *buf, uint8_t len){
    while(I2C1_IsBusy());
    I2C1_Write(addr, buf, len);
    __delay_ms(5);
    while(I2C1_IsBusy());
}

static void i2c_write_read(uint8_t addr, uint8_t *wbuf, uint8_t wlen,
                            uint8_t *rbuf, uint8_t rlen){
    while(I2C1_IsBusy());
    I2C1_WriteRead(addr, wbuf, wlen, rbuf, rlen);
    __delay_ms(5);
    while(I2C1_IsBusy());
}

static uint8_t bno_read_reg(uint8_t reg){
    uint8_t val = 0;
    i2c_write_read(BNO_ADDR, &reg, 1, &val, 1);
    return val;
}

static void bno_write_reg(uint8_t reg, uint8_t val){
    uint8_t buf[2] = {reg, val};
    i2c_write(BNO_ADDR, buf, 2);
    __delay_ms(10);
}

// ══════════════════════════════════════════
//  OLED SSD1306
// ══════════════════════════════════════════
static void oled_cmd(uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd};
    i2c_write(OLED_ADDR, buf, 2);
}

static void oled_data(uint8_t data){
    uint8_t buf[2] = {0x40, data};
    while(I2C1_IsBusy());
    I2C1_Write(OLED_ADDR, buf, 2);
    __delay_us(200);
    while(I2C1_IsBusy());
}

static void oled_init(void){
    __delay_ms(500);
    oled_cmd(0xAE);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0x81); oled_cmd(0xCF);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);
}

static void oled_set_cursor(uint8_t col, uint8_t page){
    oled_cmd(0x21); oled_cmd(col); oled_cmd(127);
    oled_cmd(0x22); oled_cmd(page); oled_cmd(7);
}

static void oled_clear(void){
    oled_set_cursor(0, 0);
    for(uint16_t i=0; i<1024; i++) oled_data(0x00);
}

static void oled_fill(void){
    oled_set_cursor(0, 0);
    for(uint16_t i=0; i<1024; i++) oled_data(0xFF);
}

static const uint8_t font[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x3F,0x40,0x38,0x40,0x3F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x00,0x5F,0x00,0x00}, // !
    {0x14,0x08,0x14,0x00,0x00}, // >
};

static void oled_char(char c){
    uint8_t idx;
    if(c >= 'A' && c <= 'Z')      idx = c - 'A' + 1;
    else if(c >= '0' && c <= '9') idx = c - '0' + 27;
    else if(c == ':')              idx = 37;
    else if(c == '-')              idx = 38;
    else if(c == '!')              idx = 39;
    else if(c == '>')              idx = 40;
    else                           idx = 0;
    for(uint8_t i=0; i<5; i++) oled_data(font[idx][i]);
    oled_data(0x00);
}

static void oled_str(const char *s, uint8_t col, uint8_t page){
    oled_set_cursor(col, page);
    while(*s) oled_char(*s++);
}

static void oled_int(int16_t num, uint8_t col, uint8_t page){
    char buf[8];
    uint8_t i = 0;
    bool neg = false;
    oled_set_cursor(col, page);
    if(num < 0){ neg = true; num = -num; }
    if(num == 0){ oled_char('0'); return; }
    while(num > 0){ buf[i++] = '0' + (num % 10); num /= 10; }
    if(neg) buf[i++] = '-';
    for(uint8_t j=0; j<i/2; j++){
        char tmp=buf[j]; buf[j]=buf[i-1-j]; buf[i-1-j]=tmp;
    }
    for(uint8_t j=0; j<i; j++) oled_char(buf[j]);
}

// ══════════════════════════════════════════
//  BNO055 IMU
// ══════════════════════════════════════════
static float h_zero = 0, r_zero = 0, p_zero = 0;

static bool bno_init(void){
    __delay_ms(700);
    uint8_t id = bno_read_reg(0x00);
    if(id != 0xA0) return false;
    bno_write_reg(0x3F, 0x20); __delay_ms(700);
    bno_write_reg(0x3E, 0x00); __delay_ms(10);
    bno_write_reg(0x3D, 0x0C); __delay_ms(600);
    return true;
}

static void bno_read_euler(float *h, float *r, float *p){
    uint8_t data[6];
    uint8_t reg = 0x1A;
    i2c_write_read(BNO_ADDR, &reg, 1, data, 6);
    int16_t hr = (int16_t)((data[1]<<8)|data[0]);
    int16_t rr = (int16_t)((data[3]<<8)|data[2]);
    int16_t pr = (int16_t)((data[5]<<8)|data[4]);
    *h = hr / 16.0f;
    *r = rr / 16.0f;
    *p = pr / 16.0f;
}

static uint8_t bno_read_temp(void){
    return bno_read_reg(0x34);
}

static void imu_calibrate(void){
    oled_clear();
    oled_str("CALIBRATING", 0, 0);
    oled_str("PLACE FLAT", 0, 2);
    oled_str("WAIT 5 SEC", 0, 4);
    __delay_ms(5000);
    float hs=0, rs=0, ps=0, h, r, p;
    for(uint8_t i=0; i<10; i++){
        bno_read_euler(&h, &r, &p);
        hs+=h; rs+=r; ps+=p;
        __delay_ms(100);
    }
    h_zero = hs/10; r_zero = rs/10; p_zero = ps/10;
    oled_clear();
    oled_str("ZERO SET", 0, 3);
    __delay_ms(1000);
}

// ══════════════════════════════════════════
//  UART PACKET HELPERS
// ══════════════════════════════════════════
static uint8_t rx_buf[MAX_PKT];
static uint8_t rx_idx = 0;
static uint8_t tx_buf[MAX_PKT];

static void uart_putc(char c){
    while(!U1ERRIRbits.TXMTIF);
    U1TXB = c;
}

static void uart_puts(const char *s){
    while(*s) uart_putc(*s++);
}

static void send_packet(char dest, const char *payload){
    uart_putc('A');
    uart_putc('Z');
    uart_putc(MY_ID);
    uart_putc(dest);
    uart_puts(payload);
    uart_putc('Y');
    uart_putc('B');
}

static void send_fwd(void){ send_packet(RAUNAK_ID, "FWD"); }
static void send_res(void){ send_packet(RAUNAK_ID, "RES"); }
static void send_stp(void){ send_packet(RAUNAK_ID, "STP"); }

static void send_sensor(float h, float r, float p, uint8_t temp){
    // Send to Mihir: H:x.xR:x.xP:x.xT:xx
    char buf[32];
    int16_t hi = (int16_t)(h * 10);
    int16_t ri = (int16_t)(r * 10);
    int16_t pi = (int16_t)(p * 10);
    sprintf(buf, "H:%dR:%dP:%dT:%d", hi, ri, pi, temp);
    send_packet(MIHIR_ID, buf);
}

static bool packet_valid(uint8_t *buf, uint8_t len){
    if(len < 6) return false;
    if(buf[0] != 'A' || buf[1] != 'Z') return false;
    if(buf[len-2] != 'Y' || buf[len-1] != 'B') return false;
    return true;
}

static void process_packet(uint8_t *buf, uint8_t len){
    if(!packet_valid(buf, len)) return;
    char src  = buf[2];
    char dest = buf[3];
    if(src == MY_ID) return;
    if(dest != MY_ID && dest != BROADCAST) return;

    // Extract payload
    uint8_t plen = len - 6;
    char payload[58] = {0};
    for(uint8_t i=0; i<plen && i<57; i++) payload[i] = buf[4+i];

    // Forward motor commands to Raunak
    if(src == MIHIR_ID){
        if(strncmp(payload, "FWD", 3)==0) send_fwd();
        else if(strncmp(payload, "RES", 3)==0) send_res();
        else if(strncmp(payload, "STP", 3)==0) send_stp();
        else if(strncmp(payload, "DATA", 4)==0){
            // Mihir requesting sensor data — will be sent in main loop
        }
    }
}

static void receive_loop(void){
    while(U1ERRIRbits.RXMTIF == 0){ // data available
        uint8_t b = U1RXB;
        if(rx_idx < MAX_PKT) rx_buf[rx_idx++] = b;
        // Check for footer
        if(rx_idx >= 6 &&
           rx_buf[rx_idx-2] == 'Y' &&
           rx_buf[rx_idx-1] == 'B'){
            process_packet(rx_buf, rx_idx);
            rx_idx = 0;
        }
        // Prevent buffer overflow
        if(rx_idx >= MAX_PKT) rx_idx = 0;
    }
}

// ══════════════════════════════════════════
//  OLED PAGE DRAW
// ══════════════════════════════════════════
static uint8_t page = 0;
static float cur_h=0, cur_r=0, cur_p=0;
static uint8_t cur_temp = 0;

static void draw_page(void){
    oled_clear();
    if(page == 0){
        oled_str("H-R-P", 0, 0);
        oled_str("H:", 0, 2);
        oled_int((int16_t)cur_h, 14, 2);
        oled_str("R:", 0, 4);
        oled_int((int16_t)cur_r, 14, 4);
        oled_str("P:", 0, 6);
        oled_int((int16_t)cur_p, 14, 6);
    } else if(page == 1){
        oled_str("TEMPERATURE", 0, 0);
        oled_int(cur_temp, 0, 3);
        oled_str("C", 12, 3);
        oled_str("BNO055", 0, 6);
    } else if(page == 2){
        oled_str("ALL IMU DATA", 0, 0);
        oled_str("H:", 0, 1);
        oled_int((int16_t)cur_h, 14, 1);
        oled_str("R:", 0, 2);
        oled_int((int16_t)cur_r, 14, 2);
        oled_str("P:", 0, 3);
        oled_int((int16_t)cur_p, 14, 3);
        oled_str("T:", 0, 4);
        oled_int(cur_temp, 14, 4);
    } else if(page == 3){
        oled_str("SYSTEM STATUS", 0, 0);
        oled_str("ME-ONLINE", 0, 2);
        oled_str("ID-LAKSH-L", 0, 4);
        oled_str("EGR314 T302", 0, 6);
    }
}

// ══════════════════════════════════════════
//  MAIN
// ══════════════════════════════════════════
int main(void){
    SYSTEM_Initialize();

    // Force I2C pin config after SYSTEM_Initialize
    RC3I2C = 0x51;
    RC4I2C = 0x51;
    ANSELCbits.ANSELC3 = 0;
    ANSELCbits.ANSELC4 = 0;
    ODCONCbits.ODCC3 = 1;
    ODCONCbits.ODCC4 = 1;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    // Enable global interrupts for hardware I2C
    INTCON0bits.GIE = 1;

    // Button pins input with pull-up
    ANSELAbits.ANSELA0 = 0;
    ANSELAbits.ANSELA1 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    WPUAbits.WPUA0 = 1;
    WPUAbits.WPUA1 = 1;

    __delay_ms(1000);

    printf("EGR314 Team 302 — Lakshanand\r\n");
    printf("PIC18F57Q83 Subsystem\r\n");

    // Init OLED
    oled_init();
    oled_fill();
    __delay_ms(500);
    oled_clear();
    oled_str("EGR314 T302", 0, 0);
    oled_str("LAKSH SUBSYS", 0, 2);
    oled_str("STARTING", 0, 4);
    __delay_ms(1000);

    // Init IMU
    if(!bno_init()){
        printf("BNO055 not found!\r\n");
        oled_clear();
        oled_str("IMU ERROR", 0, 2);
        oled_str("CHECK WIRING", 0, 4);
        while(1) __delay_ms(1000);
    }
    printf("BNO055 ready\r\n");

    // Calibrate zero
    imu_calibrate();

    oled_clear();
    oled_str("READY", 0, 0);
    oled_str("A0-PAGE BTN", 0, 2);
    oled_str("A1-RECAL BTN", 0, 4);
    __delay_ms(1000);

    printf("Running...\r\n");

    uint8_t  btn_page_prev = 1;
    uint8_t  btn_cal_prev  = 1;
    uint16_t imu_tick      = 0;
    uint16_t send_tick     = 0;

    while(1){
        receive_loop();

        // Read buttons
        uint8_t bp = PORTAbits.RA0;
        uint8_t bc = PORTAbits.RA1;

        if(btn_page_prev==1 && bp==0){
            page = (page + 1) % 4;
            printf("Page -> %d\r\n", page);
            char pbuf[8];
            sprintf(pbuf, "PAGE%d", page);
            send_packet(MIHIR_ID, pbuf);
            draw_page();
            __delay_ms(200);
        }
        btn_page_prev = bp;

        if(btn_cal_prev==1 && bc==0){
            printf("Recalibrating...\r\n");
            imu_calibrate();
            draw_page();
            __delay_ms(200);
        }
        btn_cal_prev = bc;

        // Read IMU every ~200ms (rough count)
        imu_tick++;
        if(imu_tick >= 200){
            imu_tick = 0;
            float h, r, p;
            bno_read_euler(&h, &r, &p);
            h -= h_zero; r -= r_zero; p -= p_zero;
            if(h > 180)  h -= 360;
            if(h < -180) h += 360;
            cur_h = h; cur_r = r; cur_p = p;
            cur_temp = bno_read_temp();
            printf("H:%.1f R:%.1f P:%.1f T:%d\r\n", h, r, p, cur_temp);
            draw_page();
        }

        // Send sensor to Mihir every ~5s
        send_tick++;
        if(send_tick >= 5000){
            send_tick = 0;
            send_sensor(cur_h, cur_r, cur_p, cur_temp);
        }

        __delay_ms(1);
    }
}
