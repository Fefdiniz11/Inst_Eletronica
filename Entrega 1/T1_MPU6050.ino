#include <Wire.h>

// Variaveis p/ armazenar os dados do acelerametro
long acelerometro_X, acelerometro_Y, acelerometro_Z;
float grav_X, grav_Y, grav_Z;

// Variaveis p/ armazenar os dados do giroscopio
long giroscopio_X, giroscopio_Y, giroscopio_Z;
float rotacao_X, rotacao_Y, rotacao_Z;

void setup() {
  Serial.begin(115200);   // Baud rate p/ ESP32 - Microcontrolador utilizado nesse projeto
  Wire.begin();           // Inicia a comunicacao I2C
  configurarMPU();
}

void loop() {
  Serial.println("rotacao_X, rotacao_Y, rotacao_Z, grav_X, grav_Y, grav_Z");  // Legenda para o Serial Plotter
  acelerometro();
  giroscopio();
  registros();            
  delay(100);         // Tempo em ms entre as leituras
}

/* Enderecos de memoria encontrados no datasheet register map */

// Funcao p/ configurar o MPU p/ as leituras
void configurarMPU(){
  Wire.beginTransmission(0x68);  // Endereco I2C do MPU-6050 p/ comunicacao
  Wire.write(0x6B);              // Acessa o registro 6B p/ gerenciamento de energia - Power Management 1 PWR_MGMT_1
  Wire.write(0);                 // Configura o registro sleep como 0
  Wire.endTransmission();        // Encerra a transmissao desses dados
  Wire.beginTransmission(0x68);  
  Wire.write(0x1B);              // Acessa o registro 1B p/ configuracao do giroscopio - Gyroscope Configuration GYRO_CONFIG
  Wire.write(0x00);              // Configura o giroscopio p/ a escala de +/- 250 deg/s -> FS_SEL=0
  Wire.endTransmission(); 
  Wire.beginTransmission(0x68);  
  Wire.write(0x1C);              // Acessa o registro 1C p/ configuracao do acelerometro - Accelerometer Configuration ACCEL_CONFIG
  Wire.write(0x00);              // Configura o acelerometro p/ a escala de +/- 2g -> AFS_SEL = 0
  Wire.endTransmission(); 
}

// Funcao p/ configuracao e leitura do acelerometro
void acelerometro() {
  Wire.beginTransmission(0x68);  
  Wire.write(0x3B);              // Endereco p/ leitura dos dados do acelerometro - Accelerometer Measurements
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);     // Solicita a leitura dos registros do acelerometro (3B - 40)
  while(Wire.available() < 6);   // Loop que aguarda 6 bytes p/ leitura
  acelerometro_X = Wire.read() << 8 | Wire.read();  // Armazena os dois primeiros bytes em acelerometro_X
  acelerometro_Y = Wire.read() << 8 | Wire.read();  // Armazena os dois bytes do meio em acelerometro_Y
  acelerometro_Z = Wire.read() << 8 | Wire.read();  // Armazena os dois ultimos bytes em acelerometro_Z
  ConversaoAcelerometro();                          // Converter p/ g
}

// Datasheet fornece que a sensibilidade é 16384 LSB/g -> conversao p/ forca gravitacional ou aceleracao
void ConversaoAcelerometro(){
  grav_X = acelerometro_X / 16384.0; 
  grav_Y = acelerometro_Y / 16384.0; 
  grav_Z = acelerometro_Z / 16384.0;
}

// Funcao p/ configuracao e leitura do giroscopio
void giroscopio() {
  Wire.beginTransmission(0x68);  
  Wire.write(0x43);              // Endereco p/ leitura dos dados do giroscopio - Gyroscope Measurements 
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);     // Solicita a leitura dos registros do giroscopio (43 - 48)
  while(Wire.available() < 6);
  giroscopio_X = Wire.read() << 8 | Wire.read();  // Armazena os dois primeiros bytes em giroscopio_X
  giroscopio_Y = Wire.read() << 8 | Wire.read();  // Armazena os dois bytes do meio em giroscopio_Y
  giroscopio_Z = Wire.read() << 8 | Wire.read();  // Armazena os dois últimos bytes em giroscopio_Z
  ConversaoGiroscopio();                          // Converter p/ graus/s
}

// Datasheet fornece que a sensibilidade é  131 LSB/(deg/s) -> conversao p/ graus/s
void ConversaoGiroscopio() {
  rotacao_X = giroscopio_X / 131.0;
  rotacao_Y = giroscopio_Y / 131.0; 
  rotacao_Z = giroscopio_Z / 131.0;
}

// Funcao p/ ler os valores numericos melhor visualmente no Serial Monitor
/*void registros() {
  Serial.print("giroscopio (graus/s)");
  Serial.print(" X=");
  Serial.print(rotacao_X);
  Serial.print(" Y=");
  Serial.print(rotacao_Y);
  Serial.print(" Z=");
  Serial.print(rotacao_Z);
  Serial.print(" acelerometro (g)");
  Serial.print(" X=");
  Serial.print(grav_X);
  Serial.print(" Y=");
  Serial.print(grav_Y);
  Serial.print(" Z=");
  Serial.println(grav_Z);
}*/

// Funcao p/ plotar as curvas melhor visualmente no Serial Plotter
void registros() {
  Serial.print(rotacao_X); 
  Serial.print(", ");
  Serial.print(rotacao_Y);
  Serial.print(", ");
  Serial.print(rotacao_Z);
  Serial.print(", ");
  Serial.print(grav_X);
  Serial.print(", ");
  Serial.print(grav_Y);
  Serial.print(", ");
  Serial.println(grav_Z);
}
