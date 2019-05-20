int relay = 8;
int estadoVent = 0;
//int modo = 0;

void automatico();
void manual();
void enviarMediciones();
void error();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  /*if(Serial.available() ){
    modo = Serial.read();
    if(modo == 3){
        manual();
    }

    if(modo == 4){
        error();
    }
  }*/
  if(Serial.available() ){
      int entrada = Serial.read();
      
      if(entrada == 2){
          manual();
      }
    
      else if(entrada == 4){
          Serial.println("ERROR");
          //automatico();
      }
  }
  
  automatico();
}

/*void serialEvent(){
  while(Serial.available() ){
    
      if(Serial.read() == 3){
          manual();
      }
    
      else if(Serial.read() == 4){
          automatico();
      }

      else{
          break;
      }
  }
}*/

void automatico(){
  int voltaje = analogRead(A0);

  //Serial.println("SI SE EJECUTA ESTE BLOQUE");
  
  if(voltaje > 200){
     digitalWrite(relay, LOW);   //LOW ACTIVA EL RELAY
  }
  
  if(voltaje < 200){
     digitalWrite(relay, HIGH);  //HIGH DESACTIVA EL RELAY
  }

  enviarMediciones();

  //modo = 0;
}

void manual(){
  digitalWrite(relay, HIGH);  //HIGH DESACTIVA EL RELAY
  
  while(true){
      if(Serial.available() ){
          int entrada = Serial.read();
  
          if(entrada == 1){
              if(estadoVent == 0){
                 digitalWrite(relay, LOW);   //LOW ACTIVA EL RELAY
                 estadoVent = 1;
              }
              
              else if(estadoVent == 1){
                 digitalWrite(relay, HIGH);  //HIGH DESACTIVA EL RELAY
                 estadoVent = 0;
              }
          }
          
          else if(entrada == 2){
             break;
          }

          else{ //entrada == 4
             Serial.println("ERROR");
          }
      }

      enviarMediciones();
  }

  //modo = 0;
}

/*void error(){
    Serial.println("olakase");
    Serial.println("olakase");
    Serial.println("olakase");
    Serial.println("olakase");
    Serial.println("olakase");
    Serial.println("olakase");
}*/

void enviarMediciones(){
    String sensorOutput = String(analogRead(A0));
    Serial.print(sensorOutput + ",");
    /*Serial.print(",");

    if(Serial.available() && Serial.read() == 4){
        Serial.println("olakase");
    }
  
    int ppm = map(sensorOutput, 0, 1023, 20, 10000);
    Serial.println(ppm);
    delay(1000);*/
}
