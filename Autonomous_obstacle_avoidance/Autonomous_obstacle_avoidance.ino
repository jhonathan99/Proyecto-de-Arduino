//Evitación de obstáculos Autonomouse (ARDUINO)
//    L = Izquierda
//    R = Derecha
//    F = Adelante
//    B = Atras

#include <Servo.h> 
int pinLB=A3;     // Direccion de Izquierda hacia atras
int pinLF=A2;      // Direccion de Izquierda hacia delante 

int pinRB=A1;    // Direccion de Derecha hacia atras
int pinRF=A0;    // Direccion de Derecha hacia delante

int inputPin = 9;  // Posición de recepción de la señal ultrasónica 
int outputPin =8;  // Definir la posición del transmisor de señal ultrasónica

int Fspeedd = 0;      // Velocidad de avance
int Rspeedd = 0;      // Velocidad hacia la derecha
int Lspeedd = 0;      // Velocidad hacia la Izquierda 
int directionn = 0;   // F=8 B=2 L=4 R=6 
Servo myservo;        // Configurar mi servo
int delay_time = 250; // servo motor turn back delay time

int Fgo = 8;         // forward
int Rgo = 6;         // turn right
int Lgo = 4;         // turn left
int Bgo = 2;         // turn back

void setup()
 {
  Serial.begin(9600);     // Define motor output pin 
  pinMode(pinLB,OUTPUT); 
  pinMode(pinLF,OUTPUT); 
  pinMode(pinRB,OUTPUT);  
  pinMode(pinRF,OUTPUT); 
  
  pinMode(inputPin, INPUT);    // Ultrasonic input pin 
  pinMode(outputPin, OUTPUT);  // Ultrasonic output pin    

  myservo.attach(7);    // Define servo motor output 7 pin position 
 }
void advance(int a)     // forward
    {
     digitalWrite(pinRB,LOW);  // Make motor (right rear) action 
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  // Make motor (left rear) action 
     digitalWrite(pinLF,HIGH);
     delay(a * 100);     
    }

void right(int b)        //turn right
    {
     digitalWrite(pinRB,LOW);   //Make motor (right rear) action
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,HIGH);
     delay(b * 100);
    }
void left(int c)         //turn left
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);   //Make motor (left rear) action
     digitalWrite(pinLF,HIGH);
     delay(c * 100);
    }
void turnR(int d)        //turn right
    {
     digitalWrite(pinRB,LOW);  //Make motor (right rear) action
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,LOW);  //Make motor (left rear) action
     delay(d * 100);
    }
void turnL(int e)        //turn left
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,LOW);   //Make motor (right front) action 
     digitalWrite(pinLB,LOW);   //Make motor (left rear) action
     digitalWrite(pinLF,HIGH);
     delay(e * 100);
    }    
void stopp(int f)         //stop
{
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,HIGH);
     delay(f * 100);
    }
void back(int g)          //turn back
    {

     digitalWrite(pinRB,HIGH);  //Make motor (right rear) action
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);  //Make motor (left rear) action
     digitalWrite(pinLF,LOW);
     delay(g * 100);     
    }
    
void detection()        //Measuring 3 angles (0.90.179)
    {      
      int delay_time = 250;   // servo motor turn back delay time
      ask_pin_F();            // Read ahead distance
      
     if(Fspeedd < 10)         // If the distance is less than 10 cm in front 
      {
      stopp(1);               // Clear output data 
      back(2);                // Back 0.2 seconds
      }
           
      if(Fspeedd < 25)         // If the distance is less than 25 cm in front
      {
        stopp(1);               // Clear output data  
        ask_pin_L();            // Read from the left
        delay(delay_time);      // Wait for servo motor stability 
        ask_pin_R();            // Read the right distance  
        delay(delay_time);      // Wait for servo motor stability   
        
        if(Lspeedd > Rspeedd)   //If the left distance is greater than the right distance 
        {
         directionn = Rgo;      //Go to the right
        }
        
        if(Lspeedd <= Rspeedd)   //If the left distance is less than or equal to the right distance 
        {
         directionn = Lgo;      //Go to the left 
        } 
        
        if (Lspeedd < 10 && Rspeedd < 10)   //If the distance between the left and the right is less than 10 cm. 

        {
         directionn = Bgo;      //Go back         
        }          
      }
      else                      //If the front is not smaller than (greater than) 25 cm      
      {
        directionn = Fgo;        //Go forward    
      }
     
    }    
void ask_pin_F()   // Distance ahead  
    {
      myservo.write(90);
      digitalWrite(outputPin, LOW);   // Let ultrasonic launch low voltage 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // Let the ultrasonic wave high voltage 10μs，here is at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // Maintain low voltage transmission of ultrasonic
      float Fdistance = pulseIn(inputPin, HIGH);  // Difference time difference 
      Fdistance= Fdistance/5.8/10;       // Transfer time to distance (unit: cm) 
      Serial.print("F distance:");      //Output distance (unit: cm)
      Serial.println(Fdistance);         //Display distance 
      Fspeedd = Fdistance;              // Read the distance to Fspeedd (the front speed)
    }  
 void ask_pin_L()   // Distance to the left  
    {
      myservo.write(5);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   // Let the ultrasonic launch low voltage 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // Let the ultrasonic wave transmit high voltage10μs，Here at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // Maintain low voltage transmission of ultrasonic
      float Ldistance = pulseIn(inputPin, HIGH);  // Difference time difference
      Ldistance= Ldistance/5.8/10;       // Transfer time to distance (unit: cm) 
      Serial.print("L distance:");       //Output distance (unit: cm) 
      Serial.println(Ldistance);         //Display distance 
      Lspeedd = Ldistance;              // Read distance into Lspeedd (left speed
    }  
void ask_pin_R()   // Measure out the right distance 
    {
      myservo.write(177);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   // Let the ultrasonic launch low voltage 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // Let the ultrasonic wave transmit high voltage10μs，Here at least 10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // Maintain low voltage transmission of ultrasonic
      float Rdistance = pulseIn(inputPin, HIGH);  // Difference time difference
      Rdistance= Rdistance/5.8/10;       // Transfer time to distance (unit: cm)
      Serial.print("R distance:");       //Output distance (unit: cm)
      Serial.println(Rdistance);         //Display distance 
      Rspeedd = Rdistance;              // Read distance into Rspeedd (right speed)
    }  
    
void loop()
 {
    myservo.write(90);  //Allow the servo motor to return to the ready position for the next measurement. 
    detection();        //Measure the angle and determine which direction to move
      
   if(directionn == 2)  //If directionn (direction) = 2 (reverse)              
   {
     back(8);                    //  Regression (car) 
     turnL(2);                   //Slightly to the left (to prevent mobile card in dead end Lane)
     Serial.print(" Reverse ");   //Display direction (backward) 
   }
   if(directionn == 6)           //If directionn (direction) = 6 (right turn)     
   {
     back(1); 
     turnR(6);                   // turn right 
     Serial.print(" Right ");    //Display direction (left turn) 
   }
   if(directionn == 4)          //If directionn (direction) = 4 (left turn)     
   {  
     back(1);      
     turnL(6);                  // turn left 
     Serial.print(" Left ");     //Display direction (right turn)    
   }  
   if(directionn == 8)          //If directionn (direction) = 8 (forward)      
   { 
    advance(1);                 // Normal advance   
    Serial.print(" Advance ");   //Display direction (forward) 
    Serial.print("   ");    
   }
 }


