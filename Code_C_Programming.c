#include<lpc214x.h> // To define all Special Function Registers (SFRs) - Address Mapping
#define bit(x) (1<<x) // Define a macro (Logic Shift Left)

// Set Default Password:
unsigned char pass[4] = "1234"; 

// Delay Function:
void delay() 
{
    unsigned int temp, ct;
    for(ct=0; ct<30; ct++) 
    {
        for(temp=0; temp < 65000; temp++);
    }
}

unsigned int range = 0, i; // Variables used for iteration

// DC Motor Function Definitions:
void forward(void);
void reverse(void);
void stop(void);

// LCD Function Definitions:
void lcd_init(void);
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay(void);

// Keypad Function Definitions:
#define c1 (IOPIN1&1<<20)
#define c2 (IOPIN1&1<<21)
#define c3 (IOPIN1&1<<22)

// Allocating Row and Column of 4x3 Numeric Keypad:
unsigned char r_loc,c_loc; 
unsigned char key[4][3]={"123","456","789","*0#"};
unsigned char keypad(void);

// Main Function:
int main()
{    
    unsigned char rx_arr[4];
    int count;
    
    VPBDIV=0x01; // PCLK = 60 MHz
    IO1DIR |= 0x0f<<16;	// P1.16 to P1.19 is assigned for the numeric 4*3 phone keypad.
    IO0DIR |= 0xf00fc;	// P0.2 to P0.7 is assigned for the 16*2 alphanumeric LCD.
    
    lcd_init();	// Call the LCD Function

    // Infinite Loop (An embedded program does not stop):
    while(1) 
    {
        cmd(0x80); // Set DDRAM address or cursor position on display to first line
        show("#Enter Password#");
        
        cmd(0xc5); // Loading DDRAM to the Second Line of LCD
        for(count=0; count <4; count++) 
        {
            rx_arr[count] = keypad(); // Call the Keypad Function
            dat('*'); // Password Masking
        }
        
        if ( (pass[0] == rx_arr[0]) && (pass[1] == rx_arr[1]) && (pass[2] == rx_arr[2]) && (pass[3] == rx_arr[3]) ) 
        {
            cmd(0xc0); // Set DDRAM address or cursor position on display to second line 
            show("   Thank You!   ");
            forward(); // Motor rotates in forward direction.
            delay(); // Call Delay Function
            stop(); // Motor stops to rotate
            
            cmd(0xc0); // Set DDRAM address or cursor position on display to second line 
            show("  Come Again!!  ");
            delay(); // Call Delay Function
            reverse(); // Motor rotates in reverse direction.
            delay(); // Call Delay Function
            stop(); // Motor stops to rotate
        } 
        
        else 
        {
            cmd(0xc0); // Set DDRAM address or cursor position on display to second line 
            show("~Wrong Password~");
            delay(); // Call Delay Function
        }

        cmd(0x01); // Clear LCD display; Also, clear DDRAM content      
    }
}

// Keypad Functions:
unsigned char keypad()
{
    IO1PIN &= ~(0xff<<16); // P1.16 to P1.23 is initialized to 0.
    IO1PIN |= 0xf0<<16; // P1.16 to P1.19 is configured as output.

    while(c1 && c2 && c3);
    while(!c1 || !c2 || !c3) 
    {
        if(!c1 && c2 && c3)     c_loc=0;
        else if(c1 && !c2 && c3)    c_loc=1;
        else if(c1 && c2 && !c3)    c_loc=2;

	    // ROW 1:
        IO1CLR = 1<<16; // Make row1 LOW
        IO1SET = 0x0e<<16; // Rest of the rows as '1'
        if(!c1 || !c2 || !c3) 
        {
            // Scan for Keypress:
            r_loc=0;
            break;
        }

	    // ROW 2:
        IO1CLR = 1<<17; // Make row2 LOW
        IO1SET = 0x0d<<16; // Rest of the rows as '1'
        if(!c1 || !c2 || !c3) 
        {
            // Scan for Keypress:
            r_loc=1;
            break;
        }
        
	    // ROW 3:
        IO1CLR = 1<<18; // Make row3 LOW
        IO1SET = 0x0b<<16; // Rest of the rows as '1'
        if(!c1 || !c2 || !c3) 
        { 
            // Scan for Keypress:
            r_loc=2;
            break;
        }

	    // ROW 4:
        IO1CLR = 1<<19; // Make row4 LOW
        IO1SET = 0x07<<16; // Rest of the rows as '1'
        if(!c1 || !c2 || !c3) 
        { 
            // Scan for Keypress:
            r_loc=3;
            break;
        }
    }

    while(!c1 || !c2 || !c3);
    return (key[r_loc][c_loc]); // Return Value to Display
}

// LCD Functions:

// Function to Configure LCD:
void lcd_init()
{
    cmd(0x02); // Initialize cursor to home position
    cmd(0x28); // Set the Function Set (4-bit interface, two line, 5X8 dots)
    cmd(0x0c); // Display ON, Cursor OFF, & Blink OFF
    cmd(0x06); // Set the Entry Mode (Cursor Increment, Display Shift OFF)
    cmd(0x80); // Set DDRAM or cursor position on display
}

// Display Character Using LCD Command Write Function:
void cmd(unsigned char a) 
{
    IO0PIN &= 0xffffff03; // Set P0.2 - P0.7 as input
    IO0PIN |= (a & 0xf0) << 0; // Pass the MSB, Most Significant Bit (last 4 bits of our input 8-bit data into input pins)
    IO0CLR |= bit(2); // Command Input Method Instruction, Set Register Select to 0 (RS = 0)
    IO0CLR |= bit(1); // Write to LCD, Set Read/Write to 0 (RW = 0)
    IO0SET |= bit(3); // LCD will be enabled ON until delay is performed (EN = 1)
    lcd_delay(); // Call LCD Delay Function
    IO0CLR |= bit(3); // LCD will be turned OFF, EN = 0 (Ground)
    
    // Same procedure followed again, for LSB Bit:
	IO0PIN &= 0xffffff03; // Set P0.2 - P0.7 as input
    IO0PIN |= ((a << 4)  & 0xf0) << 0; // Pass the LSB, Least Significant Bit (last 4 bits of our input 8-bit data into input pins)
    IO0CLR |= bit(2); // Command Input Method Instruction, Set Register Select to 0 (RS = 0)
    IO0CLR |= bit(1); // Write to LCD, Set Read/Write to 0 (RW = 0)
    IO0SET |= bit(3); // LCD will be enabled ON until delay is performed (EN = 1)
    lcd_delay(); // Call LCD Delay Function
    IO0CLR |= bit(3); // LCD will be turned OFF, EN = 0 (Ground)
}

// Display Character Using Data Write Function: 
void dat(unsigned char b)
{
    IO0PIN &= 0xffffff03; // Set P0.2 - P0.7 as input
    IO0PIN |= (b & 0xf0) << 0; // Pass the MSB, Most Significant Bit (last 4 bits of our input 8-bit data into input pins)
    IO0SET |= bit(2); // Data Input Method Instruction, Set Register Select to 1 (RS = 1)
    IO0CLR |= bit(1); // Write to LCD, Set Read/Write to 0 (RW = 0)
    IO0SET |= bit(3); // LCD will be enabled ON until delay is performed (EN = 1)
    lcd_delay(); // Call LCD Delay Function
    IO0CLR |= bit(3); // LCD will be turned OFF, EN = 0 (Ground)
    
    // Same procedure followed again, for LSB Bit:
	IO0PIN &= 0xffffff03; // Set P0.2 - P0.7 as input
    IO0PIN |= ((b << 4)  & 0xf0) << 0; // Pass the LSB, Least Significant Bit (last 4 bits of our input 8-bit data into input pins)
    IO0SET |= bit(2); // Data Input Method Instruction, Set Register Select to 1 (RS = 1)
    IO0CLR |= bit(1); // Write to LCD, Set Read/Write to 0 (RW = 0)
    IO0SET |= bit(3); // LCD will be enabled ON until delay is performed (EN = 1)
    lcd_delay(); // Call LCD Delay Function
    IO0CLR |= bit(3); // LCD will be turned OFF, EN = 0 (Ground)
}
 
// Function for Password Masking: 
void show(unsigned char *s)
{
    while(*s) // It involves a text field that accepts any character, however, doesn't show the inputted character to the user. 
    {
        dat(*s++); // Instead, it shows an asterisk. 
    }
}

// LCD Delay Function Loop: 
void lcd_delay() 
{
    unsigned int i;
    for(i=0;i<=1000;i++);
}

// DC Motor Functions: 
// Here, we have the following three pins - Input 1 Pin configuring P0.16, Input 2 Pin configuring P0.17 and Enable Pin configuring P0.18.
void forward()
{    
    IO0SET = bit(16) | bit(18); // Configure P0.16 and P0.18 as LOW
    IO0CLR = bit(17); // Motor rotates in forward direction.
}

void reverse()
{    
    IO0SET = bit(17) | bit(18); // Configure P0.17 & P0.18 as LOW
    IO0CLR = bit(16); // Motor rotates in reverse direction.
}

void stop()
{
    IO0CLR = bit(18); // When P0.18 is cleared, motor rotation stops.
}
