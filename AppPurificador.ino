//CleanAir Código fuente
//Mayo 2019
//
//Universidad de Guadalajara
//Escuela Politécnica de Guadalajara
//
//Autores:   Díaz Avila Christian Jhareth
//           Ramos Estrada Luis Felipe
//           González Ruvalcaba Angel
//           Hernández Rosales Cristian
//           Olmedo González Vidal Alejandro
//
//Curso:  Tecnólogo Profesional en Sistemas Informáticos
//        Poli-Innova
//        8°B  T/Matutino 
//
//Versión: 1.0
//
//**Este código debe cargarse en una placa Arduino UNO o similar
//  
//  El presente código permite manejar las funciones de un dispositivo purificador de aire 
//  mediante el control de:
//   Un disipador de 115v con un relevador (relé).
//   Un sensor de humo y gas MQ9.
//   Un módulo bluetooth HC-05 para comunicación y control a 
//    distancia con la aplicación CleanAir.
///////////////////////////////////////////////////////////////////////////////

//Prototipos de las funciones 
//↓↓↓↓
void automatico();
void manual();
void enviarMediciones();


int relay = 8;  //Variable que identifica el numero de pin digital al que se conecta 
                //el relevador para abrir o cerrar el circuito de alimenta al disipador
                
int estadoVent = 0; //Variable que identifica el estado del disipador (Encendido 1 / 
                    //Apagado 0) en el modo Manual


//Funcion encargada de recoger la configuración de los pines y de la comunicación serial 
void setup() {
  
  Serial.begin(9600); //Abre el puerto serie y fija la velocidad en baudios para la 
                      //transmisión de datos en serie (comunicación bluetooth con la aplicación)
  pinMode(relay, OUTPUT);   //Configuración de modo de trabajo del pin del relevador, 
                            //en este caso se usa como salida para activar/desactivar el disipador
  digitalWrite(relay, HIGH);  //Al ejecutarse el programa en el Arduino, activa el relevador de 
                              //manera predeterminada apagando el disipador (HIGH = apagado, 
                              //LOW = encendido)
}


//Función que contiene el programa que se ejecutará cíclicamente
void loop() {

  if(Serial.available() ){  //Si hay datos entrantes en el puerto serial 
                            //(enviados desde la aplicación):
      int entrada = Serial.read();  //Lee los datos
      
      if(entrada == 2){ //Si el dato recibido es igual a dos (indica que se presionó el botón 
                        //para cambiar de modo (Manual)):
          manual(); //Llama a la función para cambiar a modo Manual
      }
                              //La aplicaicón envía un 4 como advertencia cuando ocurren casos 
                              //específicos como pérdida de datos en la transmisión (bERROR) o 
                              //intentos de acceder a un elemento inexistente de la lista de
                              //mediciones recibida (listERROR)
      else if(entrada == 4){  //Si el dato recibido es igual a cuatro:
          Serial.println("ERROR");  //Imprime una advertencia, no es necesario hacer nada
      }
  }
  
  automatico(); //Si no hay datos entrantes, se llama a la función que ejecuta el modo de 
                //funcionamiento Automático (modo predeterminado)
}


//Función que ejecuta el modo de funcionamiento Automático
//En este modo, se envian las mediciones en ppm (partes por millón - medida de concentración) 
//del sensor a la aplicación y si estas rebasan el límite de 1000 ppm (concentración nociva), 
//se enciende el disipador y cuando el sensor mide menos de 1000 ppm, se apaga el disipador
void automatico(){
  int voltaje = analogRead(A0); //Se leen las mediciones del sensor conectado a la entrada 
                                //analógica A0
  voltaje = voltaje * 5.5;      //Como la lectura del sensor es un valor entre 0 y 1023 (0v y 5v) 
                                //se convierte tal valor a ppm dentro del rango de medición del 
                                //sensor (100 a 10000 ppm)
  
  if(voltaje > 1000){ //Si la medición del sensor rebasa los 1000 ppm:
     digitalWrite(relay, LOW);   //Se enciende el disipador
  }
  
  if(voltaje < 1000){ //Si la medición del sensor vuelve por debajo de los 1000 ppm:
     digitalWrite(relay, HIGH);  //Se apaga el disipador
  }

  enviarMediciones(); //Llama a la función que envía las mediciones del sensor por el puerto serial 
                      //a la aplicación
}


//Función que ejecuta el modo de funcionamiento Manual
//En este modo, se envían las mediciones en ppm del sensor pero el disipador no se enciende ni se 
//apaga de manera automática aunque se rebase el límite de 1000 ppm, en cambio, el usuario cuenta
//con un botón en la aplicación para hacerlo de forma manual, esto en caso de que el sensor tenga
//un mal funcionamiento o no pueda detectar la concentración de contaminantes
void manual(){
  digitalWrite(relay, HIGH);  //Al entrar en este modo, se desactiva el disipador de manera 
                              //predeterminada
  
  while(true){  //Se ejecuta un bucle infinito hasta que se cambie a modo Automático:
      if(Serial.available() ){  //Si hay datos entrantes en el puerto serial (desde la aplicación):
          int entrada = Serial.read();  //Lee los datos
  
          if(entrada == 1){ //Si el dato recibido es igual a uno (indica que el botón ON/OFF se 
                            //presionó):
              if(estadoVent == 0){  //Si el disipador está apagado (0 - estado predeterminado):
                 digitalWrite(relay, LOW);   //Se enciende el disipador
                 estadoVent = 1;  //Se cambia el estado del disipador a Encendido (1)
              }
              
              else if(estadoVent == 1){ //Si el disipador está encendido (1):
                 digitalWrite(relay, HIGH);  //Se apaga el disipador
                 estadoVent = 0;  //Se cambia el estado del disipador a Apagado (0)
              }
          }
          
          else if(entrada == 2){  //Si el dato recibido es igual a dos (indica que se presionó el 
                                  //botón para cambiar de modo (Automático)):
             break; //Se cancela el bucle infinito (while(true)) y se vuelve al modo Automático 
          }

          else{ //El dato es igual a cuatro, es decir, una advertencia
             Serial.println("ERROR"); //Imprime una advertencia, no es necesario hacer nada
          }
      }

      enviarMediciones(); //Llama a la función que envía las mediciones del sensor por el puerto 
                          //serial a la aplicación 
  }
}


//Función que envía las mediciones del sensor en ppm por el puerto serial a la aplicación
void enviarMediciones(){

    int sensorOutput = analogRead(A0); //Se leen las mediciones del sensor conectado a la entrada 
                                       //analógica A0
    sensorOutput = sensorOutput * 5.5; //Se convierte el valor recibido del sensor (0 - 1023) a ppm 
                                       //dentro de su rango de medición (100 a 10000 ppm)
    
    String ppm = String(sensorOutput);  //Se convierte el valor en ppm a una cadena de texto para 
                                        //para que pueda ser recibida como tal en la aplicación
    Serial.print(ppm + ",");  //Se agrega una coma al final de cada valor (para que sean 
                              //distinguibles por la aplicación) y se imprime en el puerto serial 
                              //enviándolo a la aplicación por medio del módulo Bluetooth HC-05 
                              //conectado al mismo
}
