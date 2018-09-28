#include <LiquidCrystal_I2C.h>

/*
	Helper library class for LCD interaction.
	The LiquidCrystal_I2C implementation used in this class was obtained from

		https://github.com/marcoschwartz/LiquidCrystal_I2C

	but any compatible implementation should work.
*/
class LCD
{
public:
	LCD();
	void print(const char *msg, uint8_t col, uint8_t row);
	void clear();
	void scroll();
private:
	LiquidCrystal_I2C m_lcd;	
};