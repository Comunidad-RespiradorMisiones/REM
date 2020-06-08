//Actializacion 1/05/2020 con el sistema de reduccion se cambia la cantidad de pasos para el giro completo del caracol a 3250
//cambia la configuracion de pasos a 200 pasos 1 vuelta
//programa con control de relacion I/E, frecuencia de respiracion fijos: 10 BPM 1:1 Ri/Re

#define dirPin 2 // pin para direccion de giro LOW antihorario/ HIGH horario
#define stepPin 3 //pin para produccion de pasos
#define finaldecarrera 18 // final de carrera en el pin 18, para futuros usos de interrupciones en dicho pin
#define buzer 15
#define boton 14 //boton de inicio


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


//declaracion de funcion para colores del led
void color(int R, int G, int B);


void setup() {

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

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
  }while(tope==false);//cuando el final de carrera se acciona, la variable tope se vuelve true y termina el bucle

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
  
//inicia el bucle general, el led permanecera encendido en color azul durante el funcionamiento

color(0, 0, 30);//azul para el funcionamiento 

//se realiza la lectura de los potenciometros    
//frecuencia=map(analogRead(A1),0,1024,8,30);//frecuencia de respiracion BPM; 8 minimo 30 maximo
//relacioninspiracion=map(analogRead(A2),0,1024,1,4);//relacion de inspiracion/espiracion; 1:1 minimo 1:4 maximo
frecuencia=10;
relacioninspiracion=1;

T=60/frecuencia;//calculo del periodo de respiracion elegido
Tin=T/(1+relacioninspiracion);//tiempo de inspiracion
Xin=(Tin/7000)*1000000;//microsegundos para el motor paso a paso. 12000 es la cantidad de veces que se ejecuta la transicion de pasos
Tex=T-Tin;//tiempo de expiracion
Xex=(Tex/7000)*1000000;

//inspiracion
if(bandera==0){
 do{

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(Xin);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(Xin);
        i++;
  
}while(i<3250);

i=0;
bandera=1;
digitalWrite(dirPin, HIGH);
delay(250);//este es el tiempo TH para mantener la compresion, establecido arbitrariamente a 1/2 de segundo.

}

//expiracion
else{
do{

        digitalWrite(stepPin, HIGH);
        delayMicroseconds(Xex);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(Xex);
  
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
