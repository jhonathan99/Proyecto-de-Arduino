//IR Seguimiento de linea, codigo de arduino：
int MotorRight1=A3; //1er. Motor derecho conectado en A3
int MotorRight2=A2; //2do. Motor derecho conectado en A2
int MotorLeft1=A1; //1er. Motor izquierdo conectado en A1
int MotorLeft2=A0; //2do. Motor izquierdo cococtado en A0

const int SensorLeft = 6;      //Pin de entrada de datos del sensor Izquierdo (Pin 6)
const int SensorMiddle= 4;    //Pin de entrada de datos del sensor Central (Pin 4) 
const int SensorRight = 2;     //Pin de entrada de datos del sensor Derecho (Pin 2) 
int SL;    //Estado del sensor Izquierdo
int SM;    //Estado del sensor Central
int SR;    //Estado del sensor Derecho

void setup()
{  
  Serial.begin(9600);
  pinMode(MotorRight1, OUTPUT); //salida de voltaje, 1er. Motor Derecho 
  pinMode(MotorRight2, OUTPUT); //salida de voltaje, 2do. Motor Derecho
  pinMode(MotorLeft1,  OUTPUT); //salida de voltaje, 1er. Motor Izquierdo 
  pinMode(MotorLeft2,  OUTPUT); //salida de voltaje, 2do. Motor Izquierdo
  pinMode(SensorLeft, INPUT); //Definir el sensor Izquierdo
  pinMode(SensorMiddle, INPUT);//Definir el sensor Central
  pinMode(SensorRight, INPUT); //Definir el sensor Derecho
}

void loop() 
 {
 SL = digitalRead(SensorLeft); // 
 SM = digitalRead(SensorMiddle);
 SR = digitalRead(SensorRight);
                   
       if (SM == HIGH)//Sensor central en el área negra
       { 
          if (SL == LOW & SR == HIGH) // Izquierda negra y derecha blanca, gire a la izquierda
          {  
             digitalWrite(MotorRight1,LOW);
             digitalWrite(MotorRight2,HIGH);
             analogWrite(MotorLeft1,0);
             analogWrite(MotorLeft2,80);
          } 
          else if (SR == LOW & SL == HIGH) //Izquierda blanca derecha negra, gire a la derecha
          {  
             analogWrite(MotorRight1,0);//turn right 
             analogWrite(MotorRight2,80);
             digitalWrite(MotorLeft1,LOW);
             digitalWrite(MotorLeft2,HIGH);
          }
         else  // Ambos lados son blancos, avance
          { 
             digitalWrite(MotorRight1,LOW);
             digitalWrite(MotorRight2,HIGH);
             digitalWrite(MotorLeft1,LOW);
             digitalWrite(MotorLeft2,HIGH);
             analogWrite(MotorLeft1,200);
             analogWrite(MotorLeft2,200);
             analogWrite(MotorRight1,200);
             analogWrite(MotorRight2,200);
         }      
       } 
       else // Sensor central en área blanca
      {  
         if (SL == LOW & SR == HIGH)// Izquierda negra derecha blanca, rápida gire a la izquierda  
        {  
            digitalWrite(MotorRight1,LOW);
            digitalWrite(MotorRight2,HIGH);
            digitalWrite(MotorLeft1,LOW);
            digitalWrite(MotorLeft2,LOW);
        }
         else if (SR == LOW & SL == HIGH) // Izquierda blanca derecha negra, giro rápido a la derecha
        {  
           digitalWrite(MotorRight1,LOW);
           digitalWrite(MotorRight2,LOW);
           digitalWrite(MotorLeft1,LOW);
           digitalWrite(MotorLeft2,HIGH);
        }
         else //Todo blanco, parada
        {    
        digitalWrite(MotorRight1,HIGH);
        digitalWrite(MotorRight2,LOW);
        digitalWrite(MotorLeft1,HIGH);
        digitalWrite(MotorLeft2,LOW);;
        }}}

