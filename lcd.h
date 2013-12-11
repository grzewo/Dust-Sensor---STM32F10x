
#define black	0x0000
#define red	0xF800
#define green	0x07E0
#define blue	0x001F
#define white	0xFFFF


#define rd GPIO_Pin_1
#define wr GPIO_Pin_2
#define rs GPIO_Pin_10
#define rst GPIO_Pin_11
#define cs GPIO_Pin_12

#define PORT GPIOA
#define STER GPIOB

void HI(uint16_t);
void LOW(uint16_t);
void cog_wr0(uint16_t);
void cog_wr1(uint16_t, uint16_t);
void cog_write(uint16_t ,uint16_t);
void cog_wr4(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void cog_write_letter(uint8_t , uint8_t, uint8_t);
void cog_write_number(uint8_t , uint8_t , uint8_t );
void cog_write_string(char *, uint8_t, uint8_t);
int dl(char *s);
//void put_char_b(unsigned int kod, unsigned int xPosition, unsigned int yPosition);

