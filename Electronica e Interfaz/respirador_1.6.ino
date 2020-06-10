//Actializacion 01/06/2020 
//cambio de motor, nema 24 por nema 17 
//con el sistema de reduccion se cambia la cantidad de pasos para el giro completo del caracol a 3250
//cambia la configuracion de pasos a 200 pasos 1 vuelta
//programa con control de relacion I/E, frecuencia de respiracion seteables por teclado en tiempo real
//implementacion de la pantalla LCD
//implementacion de interrupciones para boton de parada de emergencia

//librerias
#include <Wire.h>  // Lo trae Arduino IDE
#include <LCD.h>
#include <LiquidCrystal_I2C.h> // Incluimos la libreria del LCD

//definicion de pines
#define dirPin 2 // pin para direccion de giro LOW antihorario/ HIGH horario
#define stepPin 3 //pin para produccion de pasos
#define finaldecarrera 18 // final de carrera en el pin 18, para futuros usos de interrupciones en dicho pin
#define buzer 15
#define botona 14 //boton de enter
#define botonb 33 //boton arriba
#define botonc 31 //boton abajo
#define botond 19 //boton de inicio y parada
#define vuelta 200 //cantidad de pasos para una vuelta de la viela
#define SDA 20 
#define SCL 21

//definicion de parametros para interrupciones
const int intPin = 19;//PIN 19 para la interrupcion 4
volatile int i = 0;//variable para control de pasos en aspiracion
volatile int seteo=0;//variable de configuracion de parametros
volatile int bandera2=0;//variable de configuracion de parametros
volatile int interrump = 0;//variable para control de interrupcion

//declaracion de variables usadas 
//finales de carreras
bool tope=false;

//banderas
int bandera=0;
int incremental=0;
int listo=0;

//variables
float frecuencia=0;
float relacioninspiracion=0;
int inicio=0;
float T=0;//periodo de respiracion
float Tin=0;//tiempo de inspiracion en segundos
float Tex=0;//tiempo de espiracion en segundos
float Xin=0;//tiempo de inspiracion para el motor
volatile float Xex=0;//tiempo de espiracion para el motor

//seteo del display
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Seteamos la direccion I2C

//declaracion de funcion para colores del led
void color(int R, int G, int B);


void setup() {

//declaracion de las interrupciones
pinMode(intPin, INPUT);
attachInterrupt(digitalPinToInterrupt(intPin), paradadeemergencia, RISING);//declaracion de la interrupcion con flanco ascendente

  lcd.begin(20,4);         // Inicializamos el lcd para 20 caracteres 4 lineas, Encendemos el backlight
  lcd.setCursor(8,0); //Empiezo en la posicion 6 (caracter) sobre la linea 0
  lcd.print("REM");
//inicializacion de los pines y configuracion como salidas
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  
//configuracion del pin de final de carrera como entrada
  pinMode(finaldecarrera, INPUT);
  
// Setea el giro para acomodar el caracol, giro horario:
  digitalWrite(dirPin, HIGH);
  
//Seteo inicial: se procede a acomodar la leva para la posicion inicial. La leva girará en sentido horario hasta que termine el recorrido con el final de carrera
do{
      
      if(digitalRead(finaldecarrera)){tope=false;}
      else{tope=true;}
        
        color(30, 0, 0);//rojo mientras esta corrigiendo la posicion inicial
        lcd.setCursor(5,2); //Empiezo en la posicion 6 (caracter) sobre la linea 0
        lcd.print("Calibrando");
        digitalWrite(stepPin, HIGH);
        delay(17);
        digitalWrite(stepPin, LOW);
        delay(17);
        
        
  }while(tope==false);//cuando el final de carrera se acciona, la variable tope se vuelve true y termina el bucle

lcd.clear();
lcd.setCursor(8,0); 
lcd.print("REM");
lcd.setCursor(5,2); //Empiezo en la posicion 6 (caracter) sobre la linea 0
lcd.print("Calibrado");
lcd.setCursor(3,4); 
lcd.print("Presione ENTER");

//Estado intermedio cuando termina el seteo inicial, el sistema se encuentra en stand by esperando que se oprima el boton 
color(0, 30, 0);//verde para finalizacion del auto set
tone(15,261,1000);//tono de aviso
digitalWrite(dirPin, LOW);//cambio de giro de la leva para comenzar la compresion
delay(3000);
//final de inicializacion bucle para el boton.
do{  
    inicio=digitalRead(botona);
    delay(10); 
}while(inicio==0);
lcd.clear();

inicio=0;
delay(10);
}



void loop() {


//carga de los parametros

if(seteo==0){ 
  lcd.setCursor(8,0); 
  lcd.print("REM");
  lcd.setCursor(0,1); 
  lcd.print("Ingrese los valores");
  delay(200);

//carga de la velocidad BPM
while(bandera2==0){
  lcd.setCursor(0,2); 
  lcd.print(">");
  lcd.setCursor(1,2); 
  lcd.print("BPM: ");
  lcd.setCursor(6,2); 
  lcd.print(frecuencia);
  if(digitalRead(botonb)){
    if(incremental==0){
    frecuencia++;
    incremental=1;
    }
    else{}
    }
  else if(digitalRead(botonc)){
    if(incremental==0){
    frecuencia--;
    incremental=1;
    }
    else{}
    }
  else{if(digitalRead(botona)){bandera2=1;}
        else{incremental=0;}
        }
};

lcd.setCursor(0,2); 
lcd.print("!");
delay(200);

//carga de la relacion inspiracion espiracion
while(bandera2==1){
  lcd.setCursor(0,3); 
  lcd.print(">");
  lcd.setCursor(1,3); 
  lcd.print("Relacion I/E: ");
  lcd.setCursor(15,3); 
  lcd.print(relacioninspiracion);
  if(digitalRead(botonb)){
     if(incremental==0){
    relacioninspiracion++;;
    incremental=1;
    }
    else{}
    }
  else if(digitalRead(botonc)){
    if(incremental==0){
    relacioninspiracion--;
    incremental=1;
    }
    else{}
    }
  else{if(digitalRead(botona)){bandera2=2;}
        else{incremental=0;}
        }
};
  
lcd.setCursor(0,3); 
lcd.print("!");
delay(2000);

//limpieza de la pantalla
lcd.clear();
seteo=1;
}
else{if(digitalRead(botona)){seteo=0;}

if(listo==0){
  lcd.setCursor(8,0); 
  lcd.print("REM");
  lcd.setCursor(1,1); 
  lcd.print("Listo para empezar");
  lcd.setCursor(3,2); 
  lcd.print("Presione ENTER");

do{  
    inicio=digitalRead(botona);
    delay(10);
}while(inicio==0);

listo=1;}
  
//inicia el bucle general, el led permanecera encendido en color azul durante el funcionamiento
color(0, 0, 30);//azul para el funcionamiento 

//calculo de los tiempos
T=60/frecuencia;//calculo del periodo de respiracion elegido en segundos
Tin=T/(1+relacioninspiracion);//tiempo de inspiracion
Xin=(Tin/(vuelta/2))*1000;//milisegundos para el motor paso a paso. 400 es la cantidad de veces que se ejecuta la transicion de pasos
Tex=T-Tin;//tiempo de expiracion
Xex=(Tex/(vuelta/2))*1000;

  lcd.clear();
  lcd.setCursor(0,0); //Empiezo en la posicion 6 (caracter) sobre la linea 0
  lcd.print("BPM:");
  lcd.setCursor(5,0);
  lcd.print(frecuencia);
  lcd.setCursor(0,1);
  lcd.print("Relacion I/E:");
  lcd.setCursor(15,1);
  lcd.print(relacioninspiracion);


//inspiracion
if(bandera==0){
 do{

        digitalWrite(stepPin, HIGH);
        delay(Xin);
        digitalWrite(stepPin, LOW);
        delay(Xin);
        i++;
  
}while(i<vuelta);

i=0;
bandera=1;
digitalWrite(dirPin, HIGH);
delay(250);//este es el tiempo TH para mantener la compresion, establecido arbitrariamente a 1/2 de segundo.

}

//expiracion
else{
do{

        digitalWrite(stepPin, HIGH);
        delay(Xex);
        digitalWrite(stepPin, LOW);
        delay(Xex);
  
}while(digitalRead(finaldecarrera)==1);

        if(interrump==1){
        interrump=0;
        seteo=0;
        bandera2=0;
        listo=0;} 
        
        bandera=0;
        digitalWrite(dirPin, LOW);

}
} 
}

//funcion de interrupcion
void paradadeemergencia() {

i=vuelta;
interrump=1;
Xex=10;  
}


//funcion de los colores del led
void color(int R, int G, int B)
{
 analogWrite(9 , R);
analogWrite(6 , G);
analogWrite(5 , B);
}
