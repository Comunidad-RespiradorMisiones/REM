//declaracion de variables usadas
float presion, p, j, presionh20;
int i;

void setup()
{
  presion=0;



  pinMode(A0,INPUT);
  analogReference(EXTERNAL);//configurado en 1.67V

  Serial.begin(9600);

}


void loop()
{
//saca el promedio de 10 muesstras porque es muy variable la salida
  presion=0;
  for(i=0;i<10;i++)
  {
  presion=presion+analogRead(A0);
  delay(1);
  }
  presion=presion/10;

  //procesamiento algebraico para convertir voltage en presion
  j=(1.67*presion)/1024;//1ro calculo de valor de tension recibido en A0, VOLT=(Vref*analogico)/1024
  j=j/1.29;//2do dividimos por el factor de relacion entre valor medido y calculado de tension
  p=((j/10)+0.200);// 3ro extraemos el valor de tension del sensor despejando de la formula de la etapa de adaptacion (Vsensor-Vregulado)*GANANACIA= Vadaptado. con Vregulado=200, ganancia 10
  presionh20=((p/5)-0.04)/0.0012858;// 4to se transforma tension en presion con la funcion de transferencia del sensor
  presionh20=presionh20*10.1974; //5to se traspasa de Kpa CMH2O
  Serial.println(presionh20); //se imprime el resultado
  delay(50);

}
