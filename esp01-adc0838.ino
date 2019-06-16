/*
 * read from ADC0838
 * by Ziggy
 */

#define SPI_CS   10     // Selection Pin 
#define SPI_DODI 11     // MOSI/MISO
#define SPI_CLK  12     // Clock 

void setup()
{ 
        // set pin modes 
        pinMode(SPI_CS, OUTPUT); 
        pinMode(SPI_CLK, OUTPUT); 
        
        // prepare logging
        Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);
} 

int read_adc(int channel)
{
        int adcvalue = 0;

        // 1. CS LOW.
        digitalWrite(SPI_CS, HIGH);     // clear last transmission
        digitalWrite(SPI_CS, LOW);      // bring CS low

        // 2. Start clock
        digitalWrite(SPI_CLK, LOW);     // start clock low

        // 3. Single-ended mode MUX address (5 bits)
        pinMode(SPI_DODI, OUTPUT); 
        byte commandbits = B11000 | channel;
        for (int i = 4; i >= 0; i--)
        {
                digitalWrite(SPI_DODI, commandbits & (1 << i));
                //cycle clock
                digitalWrite(SPI_CLK, HIGH);
                digitalWrite(SPI_CLK, LOW);    
        }

        // 4. read 8 bits
        pinMode(SPI_DODI, INPUT); 
        for (int i = 7; i >= 0; i--)
        {
                //cycle clock
                digitalWrite(SPI_CLK, HIGH);
                digitalWrite(SPI_CLK, LOW);
                adcvalue += digitalRead(SPI_DODI) << i;
        }

        // 5. reset
        digitalWrite(SPI_CS, HIGH);

        return adcvalue;
}

void loop() 
{
        // read from ADC all 8 ports (0-7)
        for (int i=0; i < 8; i++)
        {
                Serial.print(read_adc(i)); 
                Serial.print(" "); 
        }
        Serial.println(""); 

        delay(250); 
} 

