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

  if(Serial.available() ){
      int entrada = Serial.read();
      
      if(entrada == 2){
          manual();
      }
    
      else if(entrada == 4){
          Serial.println("ERROR");
      }
  }
  
  automatico();
}


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
}


void enviarMediciones(){
    //String sensorOutput = String(analogRead(A0));
    //Serial.print(sensorOutput + ",");
    
    int sensorOutput = analogRead(A0);
    sensorOutput = map(sensorOutput, 0, 1023, 100, 10000);
    
    //int ppm = sensorOutput.toInt();
    String ppm = String(sensorOutput);
    Serial.print(ppm + ",");
}
