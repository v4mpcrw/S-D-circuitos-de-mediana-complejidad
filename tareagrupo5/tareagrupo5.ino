/*

Tarea 3 - Sistemas Digitales - Proyecto n°2 - Medir luminosidad
Este proyecto mide la luminosidad ambiental y clasifica el nivel de luz en 4 rangos, activando LEDs y un buzzer según la condición detectada

Integrantes:
-> Castillo Benjamín
-> Cegarra Ramón
-> Parra Paola
-> Ramos Edinson
-> Rossel Francisco
-> Sanhueza Angelo

*/

#include <Arduino_MKRIoTCarrier.h> // Libreria para controlar sensores y actuadores de la placa Arduino MKR IoT Carrier

MKRIoTCarrier carrier; // Inicializa el objeto de la placa IoT Carrier

int light = 0; // Valor de luminosidad medido por el sensor
int r,g,b;

unsigned long tiempo = 0; // Tiempo de inicio del Modulo1
unsigned long duracionEnModulo = 0; // Variable que almacena el tiempo de duración en el Modulo1
bool estarEnModulo = false; // Variable bandera para saber si se está en módulo 1
bool sonarBuzzer = false; // Variable bandera para evitar repetir la alarma en módulo 4

// Definición valores de luminosidad (parámetros)
int L1 = 500;
int L2 = 2900;
int L3 = 3800;

void setup() {

  Serial.begin(9600); // Prepara a la placa para el envio y recepción de datos
  CARRIER_CASE = false; // Indica que la placa no está dentro del case
  carrier.begin(); // Inicializa los componentes y sensores del Carrier
}

void loop() {

  while (!carrier.Light.colorAvailable()){ // Espera a que se tengan datos desde el sensor de luz
    delay(5);
  }
  carrier.Light.readColor(r, g, b, light); // Lee valores RGB y nivel de luminosidad
  Clasificar(); // Evalúa en qué rango está la luminosidad y activa el módulo correspondiente
}

void Modulo1(){

  // Módulo 1: Luminosidad baja
  // Si light ≤ 𝐿1, se mantienen apagados los leds

  carrier.leds.setPixelColor(0, 0, 0, 0); // primer pixel, apagado
  carrier.leds.setPixelColor(1, 0, 0, 0); // segundo pixel, apagado
  carrier.leds.setPixelColor(2, 0, 0, 0); // tercer pixel, apagado 
  carrier.leds.setPixelColor(3, 0, 0, 0); // cuarto pixel, apagado 
  carrier.leds.setPixelColor(4, 0, 0, 0); // quinto pixel, apagado 
  carrier.leds.show(); //actualiza el nuevo estado de los leds
}

void Modulo2(){

  // Módulo 2: Luminosidad media-baja
  // Si 𝐿1 < light ≤ 𝐿2, se enciende 1 led RGB

  carrier.leds.setPixelColor(0, 255, 0, 0); // primer pixel, rojo
  carrier.leds.setPixelColor(1, 0, 0, 0); // segundo pixel, apagado
  carrier.leds.setPixelColor(2, 0, 0, 0); // tercer pixel, apagado
  carrier.leds.setPixelColor(3, 0, 0, 0); // cuarto pixel, apagado
  carrier.leds.setPixelColor(4, 0, 0, 0); // quinto pixel, apagado 
  carrier.leds.show(); //actualiza el nuevo estado de los leds
}

void Modulo3(){

  // Módulo 3: Luminosidad media-alta
  // Si 𝐿2 < light ≤ 𝐿3, se encienden 3 leds RGB

  carrier.leds.setPixelColor(0, 0, 0, 255); // primer pixel, azul
  carrier.leds.setPixelColor(1, 0, 0, 0); // segundo pixel, apagado
  carrier.leds.setPixelColor(2, 0, 0, 255); // tercer pixel, azul
  carrier.leds.setPixelColor(3, 0, 0, 0); // cuarto pixel, apagado
  carrier.leds.setPixelColor(4, 0, 0, 255); // quinto pixel, azul
  carrier.leds.show(); //actualiza el nuevo estado de los leds
}

void Modulo4(){

  // Módulo 4: Luminosidad crítica
  // Si light > 𝐿3, se encienden los 5 leds RGB y se activa una alarma usando la bocina

  carrier.leds.setPixelColor(0, 255, 255, 255); // primer pixel, blanco
  carrier.leds.setPixelColor(1, 255, 255, 255); // segundo pixel, blanco
  carrier.leds.setPixelColor(2, 255, 255, 255); // tercer pixel, blanco
  carrier.leds.setPixelColor(3, 255, 255, 255); // cuarto pixel, blanco
  carrier.leds.setPixelColor(4, 255, 255, 255); // quinto pixel, blanco
  carrier.leds.show(); //actualiza el nuevo estado de los leds

  // Activa la alarma una sola vez mientras se encuentre en el estado light > 𝐿3

  if(!sonarBuzzer){
    carrier.Buzzer.sound(500); // Frequencia establecida en 500 Hz
    delay(3000); // Detiene la ejecución del programa (alarma) durante 3 segundos
    carrier.Buzzer.noSound(); // La alarma se desactiva
    sonarBuzzer = true; // Actualiza el estado
  }
}

// Función para determinar el módulo que se ejecutará

void Clasificar(){

  if(light <= L1){
    if(!estarEnModulo){
      tiempo = millis(); // Marca el tiempo de entrada a Modulo1 en milisegundos
      estarEnModulo = true;
    }
    duracionEnModulo = millis() - tiempo; // Calcula duración en Modulo1

    printLightBlue(); // Configuración de la pantalla para el Modulo1

    carrier.display.setTextColor(ST77XX_WHITE); // Establecer texto en color blanco
    carrier.display.setTextSize(2); // Tamaño medio del texto
    carrier.display.setCursor(20, 160); // Ubicación del texto (x, y)

    // Mostrar en pantalla el tiempo que el módulo ha experimentado el estado light ≤ 𝐿1

    carrier.display.print("Tiempo: ");
    carrier.display.print(duracionEnModulo/1000.0); // Conversión de milisegundos a segundos

    carrier.display.println(" s");

    Modulo1();
    delay(5000); // Detiene la ejecución del programa durante 5 segundos para visualizar Tiempo en Modulo1
  }

  else if(light > L1 && light <= L2){

    estarEnModulo = false; // Reinicia variable bandera
    sonarBuzzer = false; // Reinicia variable bandera

    Modulo2();
    printLightGreen();
  }
  else if(light > L2 && light <= L3){

    estarEnModulo = false; // Reinicia variable bandera
    sonarBuzzer = false; // Reinicia variable bandera

    Modulo3();
    printLightMagenta();
  }
  else if(light > L3){

    estarEnModulo = false; // Reinicia variable bandera
    Modulo4();
    printLightRed();
  }
}

// Función para establecer configuración de la pantalla para Modulo1

void printLightBlue(){
	
  carrier.display.fillScreen(ST77XX_BLUE); // Establece fondo azul
  carrier.display.setTextColor(ST77XX_WHITE); // Establece texto en color blanco
  carrier.display.setTextSize(2); // Tamaño medio del texto
  carrier.display.setCursor(60, 40);
  carrier.display.println("Modulo 1");
  carrier.display.setCursor(60, 60);
  carrier.display.println("Luminosidad");
  carrier.display.setCursor(60, 80);
  carrier.display.println("Baja");

  // Impresión del valor de luminosidad en LUX

  carrier.display.setCursor(20, 120); /// Ubicación del texto (x, y)
  carrier.display.print("Light: ");
  carrier.display.print(light);
  carrier.display.println(" LUX");
}

// Función para establecer configuración de la pantalla para Modulo2
void printLightGreen(){
	
  // Ajuste del color de fondo, color del texto, tamaño y posición del texto

  carrier.display.fillScreen(ST77XX_GREEN); // Establece fondo verde
  carrier.display.setTextColor(ST77XX_WHITE); // Establece texto en color blanco
  carrier.display.setTextSize(2); // Tamaño medio del texto
  carrier.display.setCursor(60, 40);
  carrier.display.println("Modulo 2");
  carrier.display.setCursor(60, 60);
  carrier.display.println("Luminosidad");
  carrier.display.setCursor(60, 80);
  carrier.display.println("Media-Baja");


  // Impresión del valor de luminosidad en LUX

  carrier.display.setCursor(20, 120); // Ubicación del texto (x, y)
  carrier.display.print("Light: ");
  carrier.display.print(light);
  carrier.display.println(" LUX");
}

// Función para establecer configuración de la pantalla para Modulo3

void printLightMagenta(){
	
  // Ajuste del color de fondo, color del texto, tamaño y posición del texto

  carrier.display.fillScreen(ST77XX_MAGENTA); // Establece fondo magenta
  carrier.display.setTextColor(ST77XX_WHITE); // Establece texto en color blanco
  carrier.display.setTextSize(2); // Tamaño medio del texto
  carrier.display.setCursor(60, 40);
  carrier.display.println("Modulo 3");
  carrier.display.setCursor(60, 60);
  carrier.display.println("Luminosidad");
  carrier.display.setCursor(60, 80);
  carrier.display.println("Media-Alta");

  // Impresión del valor de luminosidad en LUX

  carrier.display.setCursor(20, 120); // Ubicación del texto (x, y)
  carrier.display.print("Light: ");
  carrier.display.print(light);
  carrier.display.println(" LUX");
}

// Función para establecer configuración de la pantalla para Modulo4

void printLightRed(){

  // Ajuste del color de fondo, color del texto, tamaño y posición del texto
	
  carrier.display.fillScreen(ST77XX_RED); // Establece fondo rojo
  carrier.display.setTextColor(ST77XX_WHITE); // Establece texto en color blanco
  carrier.display.setTextSize(2);
  carrier.display.setCursor(60, 40);
  carrier.display.println("Modulo 4");
  carrier.display.setCursor(60, 60);
  carrier.display.println("Luminosidad");
  carrier.display.setCursor(60, 80);
  carrier.display.println("Critica");

  // Impresión del valor de luminosidad en LUX

  carrier.display.setCursor(20, 120); // Ubicación del texto (x, y)
  carrier.display.print("Light: ");
  carrier.display.print(light);
  carrier.display.println(" LUX");
}