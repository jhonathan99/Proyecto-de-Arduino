#include <Servo.h>    
#include <DistanceSRF04.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


DistanceSRF04 Dist;
LiquidCrystal_I2C lcd(0x27, 16, 2);//Statement LCD, used to display text. 
int distance;
int EN1 = A3;//Define I4 interface 
int EN2 = A2;//Define I3 interface
int EN3 = A1;//Define I2 interface 
int EN4 = A0;//Define I1 interface 

int LED1 = 2;
int LED2 = 4;
int EA = 3;
int EB = 5;

int LaBa = 12;


//Level state of four motors 
uint8_t EN1Status = LOW;
uint8_t EN2Status = LOW;
uint8_t EN3Status = LOW;
uint8_t EN4Status = LOW;

Servo servoX;
Servo servoY;


byte serialIn = 0;
byte commandAvailable = false;
String strReceived = "";
//Whether to turn on the radar, my bad. The radar part has been commented out. 
byte radar = false;
//Whether to turn on the lights, did not intend to do.. lazy, you can increase the LED1, LED2 port is planned to use the interface 
byte light = false;
//The center angle of the two sevo gear.
byte servoXCenterPoint = 94;
byte servoYCenterPoint = 88 ;
//The maximum angle of the two sevo gear
byte servoXmax = 170;
byte servoYmax = 130;
//The minimum angle of the two sevos gear
byte servoXmini = 10;
byte servoYmini = 10;
//The current point of view, the two servos for return, don't read the calculation, saving resources
byte servoXPoint = 0;
byte servoYPoint = 0;
//The current speed value of the motor at the left and right 
byte leftspeed = 0;
byte rightspeed = 0;
//he angle of the sevo gear rotates at a time.
byte servoStep = 4;

///The declaration of the end of the part, attention, a lot of use of byte, if more than 255 or with int



void setup()
{
	lcd.init();
	lcd.init();
	lcd.backlight();

	//LCD test 
	lcd.setCursor(0, 0);
	lcd.print("System starting!");


	Dist.begin(9,8);//Radar signal port 

	servoX.attach(10);//X sevo signal port
	servoY.attach(11);//Y sevo signal port

	pinMode(EN1, OUTPUT);
	pinMode(EN2, OUTPUT);
	pinMode(EN3, OUTPUT);
	pinMode(EN4, OUTPUT);

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LaBa, OUTPUT); 

	lcd.setCursor(0, 1);
	lcd.print("test engine......");
	servo_test();//sevo test

	Serial.begin(9600);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Welcome!");
	lcd.setCursor(0, 1);
	lcd.print("smart car system");
}


void loop()
{
	if (radar == true)
	{
		distance = Dist.getDistanceCentimeter();
		if (distance <= 5 & distance > 1){
			lcd.setCursor(0, 1);
			lcd.println("Warning back");
			hou();
			delay(100);
			ting();
			distance = 0;
		}
	}
	getSerialLine();
	if (commandAvailable) {
		processCommand(strReceived);
		strReceived = "";
		commandAvailable = false;
	}

	
}


void getSerialLine()
{
	//Using the \R character as the two command interval, the character is received. 
	while (serialIn != '\r')
	{
		if (!(Serial.available() > 0))
		{
			return;
		}

		serialIn = Serial.read();
		if (serialIn != '\r') {
			//For some languages may not be able to separate the \R ignore the subsequent \n character
			if (serialIn != '\n'){
				char a = char(serialIn);
				strReceived += a;
			}

		}
	}
	//Read to the partition, re start stitching 
	if (serialIn == '\r') {
		commandAvailable = true;
		serialIn = 0;
	}

}
///
//Command processing 
//It is recommended that all command recognition be done here. Each action  is independent of the method, and don't write the operation in it, so that it looks more clear. 
///
void processCommand(String input)
{
	String command = getValue(input, ' ', 0);
	byte iscommand = true;
	int val;
	if (command == "MD_Qian")
	{
		qian();
	}
	else if (command == "MD_Hou")
	{
		hou();
	}
	else if (command == "MD_Zuo")
	{
		zuo();
	}
	else if (command == "MD_You")
	{
		you();
	}
	else if (command == "MD_Ting")
	{
		ting();
	}
	else if (command == "MD_SD")
	{
		val = getValue(input, ' ', 1).toInt();
		leftspeed = val;
		val = getValue(input, ' ', 2).toInt();
		rightspeed = val;
	}
	else if (command == "DJ_CS")
	{
		servo_test();
	}
	else if (command == "DJ_Shang")
	{
		servo_up();
	}
	else if (command == "DJ_Xia")
	{
		servo_down();
	}
	else if (command == "DJ_Zuo")
	{
		servo_left();
	}
	else if (command == "DJ_You")
	{
		servo_right();
	}
	else if (command == "DJ_Zhong")
	{
		servo_center();
	}
	else if (command == "DJ_CZ_JD")//VerticalRotation
	{
		val = getValue(input, ' ', 1).toInt();
	}
	else if (command == "DJ_SP_JD")//Horizontal rotation
	{
		val = getValue(input, ' ', 1).toInt();
	}
	else if (command == "LED_Status")
	{
		val = getValue(input, ' ', 1).toInt();
		light = val == 0 ? false : true;
		SetLight(light);
	}
	else if (command == "LED_Status_Swich")
	{
		light = light ? false : true;
		SetLight(light);
	}
	else if (command == "Radar_Status")
	{
		val = getValue(input, ' ', 1).toInt();
		radar = val ? true : false;
	}
	else if (command == "Radar_Status_Swich")
	{
		radar = radar ? false : true;
	}
	else if (command == "LaBa_Start"){
		SetLaBa(true);
	}
	else if (command == "LaBa_Stop"){
		SetLaBa(false);
	}
	else
	{
		iscommand = false;
	}
	//Whether received is already defined order, if not then do not return, lest waste of bandwidth
	if (iscommand){
		SendMessage("cmd:" + input);
		SendStatus();
	}
	
}
//Command parameters acquiring methods, support a command multiple parameters using space character as symbol segmentation, pay attention to is the half angle space character, not tabulation character is not a full width space, or self definition, and modify, lazy changed. 
String getValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = {
		0, -1 };
	int maxIndex = data.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++){
		if (data.charAt(i) == separator || i == maxIndex){
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}

	return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//Sevo action part
void servo_test(void) {
	int nowcornerY = servoY.read();
	int nowcornerX = servoX.read();
	servo_Vertical(servoYmini);
	delay(500);
	servo_Vertical(servoYmax);
	delay(500);
	servo_Vertical(servoYCenterPoint);
	delay(500);
	servo_Horizontal(servoXmini);
	delay(500);
	servo_Horizontal(servoXmax);
	delay(500);
	servo_Horizontal(servoXCenterPoint);
	delay(500);
	servo_center();
}
void servo_right(void)
{
	int servotemp = servoX.read();
	servotemp -= servoStep;
	servo_Horizontal(servotemp);
}
void servo_left(void)
{
	int servotemp = servoX.read();
	servotemp += servoStep;
	servo_Horizontal(servotemp);
}
void servo_down(void)
{
	int servotemp = servoY.read();
	servotemp += servoStep;
	servo_Vertical(servotemp);
}
void servo_up(void)
{
	int servotemp = servoY.read();
	servotemp -= servoStep;
	servo_Vertical(servotemp);
}
void servo_center(void)
{
	servo_Vertical(servoYCenterPoint);
	servo_Horizontal(servoXCenterPoint);
}
void servo_Vertical(int corner)
{
	int cornerY = servoY.read();
	if (cornerY > corner) {
		for (int i = cornerY; i > corner; i = i - servoStep) {
			servoY.write(i);
			servoYPoint = i;
			delay(50);
		}
	}
	else {
		for (int i = cornerY; i < corner; i = i + servoStep) {
			servoY.write(i);
			servoYPoint = i;
			delay(50);
		}
	}
	servoY.write(corner);
	servoYPoint = corner;
}
void servo_Horizontal(int corner)
{
	int i = 0;
	byte cornerX = servoX.read();
	if (cornerX > corner) {
		for (i = cornerX; i > corner; i = i - servoStep) {
			servoX.write(i);
			servoXPoint = i;
			delay(50);
		}
	}
	else {
		for (i = cornerX; i < corner; i = i + servoStep) {
			servoX.write(i);
			servoXPoint = i;
			delay(50);
		}
	}
	servoX.write(corner);
	servoXPoint = corner;
}
//Motor direction action part 
void qian(void)
{
	EN1Status = LOW;
	EN2Status = HIGH;
	EN3Status = LOW;
	EN4Status = HIGH;
	SetEN();
}
void hou(void)
{
	EN1Status = HIGH;
	EN2Status = LOW;
	EN3Status = HIGH;
	EN4Status = LOW;
	SetEN();
}
void you(void)
{
	EN1Status = LOW;
	EN2Status = HIGH;
	EN3Status = HIGH;
	EN4Status = LOW;
	SetEN();
}
void zuo(void)
{
	EN1Status = HIGH;
	EN2Status = LOW;
	EN3Status = LOW;
	EN4Status = HIGH;
	SetEN();
}
void ting(void)
{
	leftspeed = 0;
	rightspeed = 0;
	EN1Status = LOW;
	EN2Status = LOW;
	EN3Status = LOW;
	EN4Status = LOW;
	SetEN();
}
//Set speed on both sides
void SetEN(){
	analogWrite(EA, leftspeed);
	analogWrite(EB, rightspeed);
	digitalWrite(EN1, EN1Status);
	digitalWrite(EN2, EN2Status);
	digitalWrite(EN3, EN3Status);
	digitalWrite(EN4, EN4Status);
}

//Light switch 
void SetLight(bool status){
	digitalWrite(LED1, status);
	digitalWrite(LED2, status);
}
void SetLaBa(bool status){
	if (status)
		digitalWrite(LaBa, HIGH);//Hair sound 
	else
		digitalWrite(LaBa, LOW);
}


//Splicing and state 
void SendStatus(){

	String out = "";
	out += EN1Status;
	out += ",";
	out += EN2Status;
	out += ",";
	out += EN3Status;
	out += ",";
	out += EN4Status;
	out += ",";
	out += leftspeed;
	out += ",";
	out += rightspeed;
	out += ",";
	out += servoXPoint;
	out += ",";
	out += servoYPoint;
	out += ",";
	out += radar;
	out += ",";
	out += light;
	SendMessage(out);
}
//Serial message sending 
void SendMessage(String data){
	Serial.println(data);
}

