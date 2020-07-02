#include <SoftwareSerial.h>
 
String BT_data; 
String BT_data_Out;
int Vector[5];
int Cant; 
 
SoftwareSerial Comm(2,3);
 
void setup() {
  //Serial.begin(9600);
  Comm.begin(9600);
  pinMode(13, OUTPUT);
  BT_data = (String)("");

}
 
void loop() {
  if (Comm.available()) {
    BT_data = (Comm.readString());
    Cant = Str_A_Vector (BT_data); 
  }
    //if (Cant > 0) {
      digitalWrite(13, HIGH);   // set the LED on
      delay(500);              // wait for a second
      digitalWrite(13, LOW);    // set the LED off

    //}
    
    //Cant = Str_A_Vector (BT_data);
    BT_data_Out = (String) random(1,100) + ";" + (String) random(1,1000) + ";" + (String) random(1,10);
    Comm.println(BT_data_Out);
    delay(500);              
    
}

int Str_A_Vector(String Cadena) {
  int Elem = 0;
  char Uno;
  String Cad = "";
  for (int i = 0; (Cadena.length()-1); i++) {
    Uno = Cadena.charAt(i+1);
    if (Uno == ";") {
      Vector[Elem]= Cad.toInt() + 1;
      Elem += 1;
      Cad = "";
    }
    else
    Cad += Uno; 
  }
  return (Elem + 1);
}

String Vector_A_Str (int Elem) {
  String Cad = "";
  for (int i = 0; (Elem - 1); i++) {
    Cad += (char)Vector[i] + ";";
    //  digitalWrite(13, HIGH);   // set the LED on
    //  delay(500);              // wait for a second
    //  digitalWrite(13, LOW);    // set the LED off
  }
  Cad.remove(Cad.length()-1,1);
  return (Cad);
}
