#include <EEPROM.h>
#include <phi_interfaces.h>
#include <Servo.h> 

#define buttons_per_column 3
#define buttons_per_row 4
#define SOUNDOUT_PIN 9
#define GREEN_LED 10
#define RED_LED 11
#define LIGHTS 12
#define SWITCH 
#define MODE_OPEN 1
#define MODE_OPEN_CHANGECODE 2
#define MODE_CLOSED 0


char mapping[]={'1','2','3','4','5','6','7','8','9','*','0','#'}; // This is a matrix keypad.
byte pins[]={5, 4, 3, 2, 8, 7, 6}; // The first four pins are rows, the next 4 are columns. If you have 4*3 pad, then the first 4 are rows and the next 3 are columns.
phi_matrix_keypads panel_keypad(mapping, pins, buttons_per_row, buttons_per_column);


#define MAX_CODE_INDEX 255

String code = "1234\0";
String recovery_code = "1234567890\0";
String ecode = "\0";
String ecode1 = "\0";
char temp = NO_KEY;
int ecode_index = 0;
boolean code_ok = false;
int closed_switch = 0;
int mode = MODE_OPEN;

Servo myservo1;

void setup()
{
	Serial.begin(9600);
	Serial.println("Welcome to Tresor V1.0 (c) by Alexander Hofmann");
	panel_keypad.set_repeat(100); // You don't have to set this since it has a default value of 200. This sets how often a held key is repeated, 0.1s. So the held key is repeated 10 times a second.
	panel_keypad.set_hold(2000); // You don't have to set this since it has a default value of 2000. This sets how long you need to hold a key before it is repeated, 2s. So the held key starts repeating after being held for 2 seconds.
	pinMode(SOUNDOUT_PIN,OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(RED_LED, OUTPUT);
	pinMode(LIGHTS, OUTPUT);
	pinMode(A0, INPUT);
	//write_code();
	read_code();  
	Serial.print("Code = ");
	Serial.println(code);

	green();
	myservo1.attach(13);
	servos_open();
	lights_on();
}

void read_code() {
	int i=0;
	char c = 0;
	code = "";
	do {
		c = EEPROM.read(i);
		code = code + c;
		i++;
	} while(c != '\0');

	recovery_code = "";
	i=255;
	do {
		c = EEPROM.read(i);
		recovery_code = recovery_code + c;
		i++;
	} while(recovery_code != '\0');
}

void write_code() {
	int i=0;
	do {
		EEPROM.write(i, code[i]);
		i++;
	} while(code[i] != '\0');
	i = 255;
	do {
		EEPROM.write(i, recovery_code[i]);
		i++;
	} while(recovery_code[i] != '\0');  
}

void green() {
	digitalWrite(RED_LED, LOW);
	digitalWrite(GREEN_LED, HIGH);  
}

void red() {
	digitalWrite(RED_LED, HIGH);
	digitalWrite(GREEN_LED, LOW);        
}

void orange() {
	digitalWrite(RED_LED, HIGH);
	digitalWrite(GREEN_LED, HIGH);        
}
void orange_blink() {
	for (int i=0; i<5; i++) {
		off();
		delay(200);
		beep4();
		orange();
		delay(100);
	}
	orange();
}
void off() {
	digitalWrite(RED_LED, LOW);
	digitalWrite(GREEN_LED, LOW);        
}

void red_blink() {
	for (int i=0; i<5; i++) {
		off();
		delay(200);
		beep4();
		red();
		delay(100);
	}
	red();
}

void lights_on() {
	digitalWrite(LIGHTS, HIGH);
}

void lights_off() {
	digitalWrite(LIGHTS, LOW);
}

void beep1() {
	for (int i=0; i<100; i++) {  
	digitalWrite(SOUNDOUT_PIN,HIGH);
	delayMicroseconds(1000);
	digitalWrite(SOUNDOUT_PIN,LOW);
	delayMicroseconds(1000);
	}
}

void beep2() {
	for (int i=0; i<100; i++) {  
	digitalWrite(SOUNDOUT_PIN,HIGH);
	delayMicroseconds(500);
	digitalWrite(SOUNDOUT_PIN,LOW);
	delayMicroseconds(500);
	}
}

void beep3() {
	for (int i=0; i<100; i++) {  
	digitalWrite(SOUNDOUT_PIN,HIGH);
	delayMicroseconds(250);
	digitalWrite(SOUNDOUT_PIN,LOW);
	delayMicroseconds(250);
	}
}

void beep4() {
	beep3();
	beep1();
}

void beep5() {
	for (int i=0; i<50; i++) {  
	digitalWrite(SOUNDOUT_PIN,HIGH);
	delayMicroseconds(3000);
	digitalWrite(SOUNDOUT_PIN,LOW);
	delayMicroseconds(3000);
	}
}  

void reset_ecode() {
	ecode_index = 0;	
	ecode[0] = '\0';
	code_ok = false;
}

boolean recovery_code_ecode_compare() {
	for (int i=0; i<255; i++) {
	if (recovery_code[i] == '\0') {
		if (ecode[i] == '\0') {
			return true;
		} else {
			return false;
		}
	}
	if (ecode[i] == '\0') {
		return false;
	}
	if (recovery_code[i] != ecode[i]) {
		return false;
	}
	}
	return true;  
}
void ecode_code_copy() {
	int i = 0;
	do {
		code[i] = ecode[i];
	} while ((ecode[i++] != '\0') && (i < 255))	;		
}
boolean code_ecode_compare() {
	for (int i=0; i<255; i++) {
		if (code[i] == '\0') {
			if (ecode[i] == '\0') {
				return true;
			} else {
				return false;
			}
		}
		if (ecode[i] == '\0') {
			return false;
		}
		if (code[i] != ecode[i]) {
			return false;
		}
	}
	return true;
}
boolean ecode_ecode1_compare() {
	for (int i=0; i<255; i++) 
	{
		if (ecode1[i] == '\0') 
		{
			if (ecode[i] == '\0') 
			{
				return true;
			} else {
				return false;
			}
		}
		if (ecode[i] == '\0') {
			return false;
		}
		if (ecode1[i] != ecode[i]) {
			return false;
		}
	}
	return true;
}

void servos_open() {
	myservo1.write(90);
}

void servos_close() {
	myservo1.write(10);
}

void print_mode() {
	if (mode == 0) Serial.println("MODE CLOSED");
	if (mode == 1) Serial.println("MODE OPEN");
	if (mode == 2) Serial.println("MODE OPEN CHANGECODE");
}

void loop()
{
	// read the status of the door closed switch
	closed_switch = digitalRead(A0);

	if (closed_switch) {
		// if the door is being closed
		if (mode >= MODE_OPEN) {
			mode = MODE_CLOSED;
			delay(500);
			red();
			lights_off();
			servos_close();
			reset_ecode();
			closed_switch = HIGH;
			ecode1[0] = '\0';
		}
	}

	temp = NO_KEY;
	temp = panel_keypad.getKey(); // Use phi_keypads object to access the keypad
	
	if (temp != NO_KEY) {    
		if ((temp != '*') && (temp != '#') && (temp != NO_KEY)) {
			beep3();
			ecode[ecode_index] = temp;
			ecode_index++;
			ecode[ecode_index] = '\0';
			if (ecode_index >= 255) {
				reset_ecode();
			}
		}
	}
	if (temp == '*') {
		if (mode == MODE_OPEN_CHANGECODE) {
			// cancel MODE OPEN
			beep1();
			green();
			lights_on();
			reset_ecode();
			ecode1[0] = '\0';
			mode = MODE_OPEN;
			return;
		}
		if (mode == MODE_CLOSED) {
			beep1();
			reset_ecode();      
		}
	}
	if (temp == '#') {
		print_mode();
		Serial.print("Entered Code = ");
		Serial.println(ecode);
		if (mode == MODE_CLOSED) {
			// tresor is closed and user wants to open it now
			beep4();
			if ((code_ecode_compare()) || (recovery_code_ecode_compare())) {
				beep1();beep2();beep3();
				mode = MODE_OPEN;
				servos_open();
				green();
				lights_on();
				reset_ecode();
				delay(2000);
				
				return;
			} else {
				red_blink();  
				reset_ecode();
			}
		}
		if (mode >= MODE_OPEN) {
			// user entered new code already once
			if (mode == MODE_OPEN_CHANGECODE) {
				if (ecode_ecode1_compare()) {
					beep1();beep2();beep3();
					ecode_code_copy();
					Serial.print("new code = ");
					Serial.println(code);
					write_code();
					mode = MODE_OPEN;
					green();
					lights_on();
					reset_ecode();
					ecode[0] = '\0';
					return;
				} else {
					Serial.println("wrong code reentered");
					orange_blink();
					orange();
					reset_ecode();
					return;
				}
			}
			if (mode == MODE_OPEN) {
				// copy ecode to ecode1
				Serial.print("ecode = ");
				Serial.println(ecode);
				int i=0;
				do {
					ecode1[i] = ecode[i];
				} while ((ecode[i++] != '\0') && (i <= 254));
				Serial.print("ecode1 = ");
				Serial.println(ecode1);
				reset_ecode();
				mode = MODE_OPEN_CHANGECODE;
				orange();
			}

		}
		reset_ecode();
	}
}
  

