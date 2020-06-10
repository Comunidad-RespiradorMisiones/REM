//Actializacion 30/05/2020 cambio de motor, nema 24 por nema 17 

//con el sistema de reduccion se cambia la cantidad de pasos para el giro completo del caracol a 3250
//cambia la configuracion de pasos a 200 pasos 1 vuelta

//programa con control de relacion I/E, frecuencia de respiracion fijos: 40 BPM 1:4 Ri/Re
//implementacion de la pantalla LCD

//librerias
#include <Wire.h>  // Lo trae Arduino IDE
#include <LCD.h>
#include <LiquidCrystal_I2C.h> // Incluimos la libreria del LCD


#define dirPin 2 // pin para direccion de giro LOW antihorario/ HIGH horario
#define stepPin 3 //pin para produccion de pasos
#define finaldecarrera 18 // final de carrera en el pin 18, para futuros usos de interrupciones en dicho pin
#define buzer 15
#define boton 14 //boton de inicio
#define vuelta 200 //cantidad de pasos para una vuelta de la viela
#define SDA 20 
#define SCL 21


//declaracion de variables usadas 
bool tope=false;
int bandera =0;
long int i=0;
float frecuencia=0;
float relacioninspiracion=0;
int inicio=0;
float T=0;//periodo de respiracion
float Tin=0;//tiempo de inspiracion en segundos
float Tex=0;//tiempo de espiracion en segundos
float Xin=0;//tiempo de inspiracion para el motor
float Xex=0;//tiempo de espiracion para el motor

//seteo del display
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Seteamos la direccion I2C

//declaracion de funcion para colores del led
void color(int R, int G, int B);


void setup() {

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

//Estado intermedio cuando termina el seteo inicial, el sistema se encuentra en stand by esperando que se oprima el boton 
color(0, 30, 0);//verde para finalizacion del auto set
tone(15,261,1000);//tono de aviso
digitalWrite(dirPin, LOW);//cambio de giro de la leva para comenzar la compresion
delay(3000);
//final de inicializacion bucle para el boton.
do{  
    inicio=digitalRead(boton);
    delay(10); 
}while(inicio==0);

}



void loop() {

inicio=0; 
delay(10);
 do{  
    inicio=digitalRead(boton);
    delay(10); 

  lcd.clear();
  lcd.setCursor(8,0); 
  lcd.print("REM");
  lcd.setCursor(3,1); 
  lcd.print("Ingrese el valor de BPM");


}while(inicio==0);

  
//inicia el bucle general, el led permanecera encendido en color azul durante el funcionamiento

color(0, 0, 30);//azul para el funcionamiento 

//se realiza la carga de la frecuencia    
frecuencia=40;//frecuencia de respiracion BPM; 8 minimo 30 maximo
relacioninspiracion=4;//relacion de inspiracion/espiracion; 1:1 minimo 1:4 maximo

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

        bandera=0;
        digitalWrite(dirPin, LOW);

}
 
}

//funcion de los colores del led
void color(int R, int G, int B)
{
 analogWrite(9 , R);
analogWrite(6 , G);
analogWrite(5 , B);
}
