#include <Wire.h>
#include <math.h>

// Variaveis do acelerometro
long acelerometro_X, acelerometro_Y, acelerometro_Z;
float grav_X, grav_Y, grav_Z;
float soma_X = 0, media_X = 0, ganho_X = 0, offset_X = 0, theta_X = 0, soma_Y = 0,  media_Y = 0, ganho_Y = 0, offset_Y = 0, theta_Y = 0, soma_Z = 0,  media_Z = 0, ganho_Z = 0, offset_Z = 0;
float medias[2] = {0.0, 0.0};

// Variaveis do giroscopio
long giroscopio_X, giroscopio_Y, giroscopio_Z;
float rotacao_X, rotacao_Y, rotacao_Z;

char eixo;
int i = 0, j = 0;
float theta = 0;

void setup() {
  Serial.begin(9600);   // Baud rate p/ ESP32 - Microcontrolador utilizado nesse projeto
  Wire.begin();           // Inicia a comunicacao I2C
  configurarMPU();
}

void loop(){
  acelerometro();
}

void configurarMPU(){
  //Inicializa a MPU6050
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);               // Acessa o registro 6B p/ gerenciamento de energia - Power Management 1 PWR_MGMT_1
  Wire.write(0);                  // Configura o registro sleep como 0
  Wire.endTransmission(true);     // Encerra a transmissao desses dados
}

int16_t dados(int endereco, byte regEndereco) {
  // Le 2 bytes de dados do registrador especificado
  Wire.beginTransmission(endereco);
  Wire.write(regEndereco);
  Wire.endTransmission(false);

  Wire.requestFrom(endereco, 2, true);
  byte byte_maior = Wire.read();
  byte byte_menor = Wire.read();

  int16_t valor = (byte_maior << 8) | byte_menor;
  return valor;
}


// Datasheet fornece que a sensibilidade é 16384 LSB/g -> conversao p/ forca gravitacional ou aceleracao
float ConversaoAcelerometro(long valor){
  float grav = valor / 16384.0; 
  return grav;
}

// Calibracao para o eixo X
void eixoX(){
  Serial.print("--------------- Eixo X ---------------");
  for(i = 1; i <= 100; i++){

    acelerometro_X = dados(0x68, 0x3B);     // Le os valores do acelerometro
    grav_X = ConversaoAcelerometro(acelerometro_X);       // Converter para forca gravitacional   
    theta_X = (atan( (grav_X/(sqrt ((grav_Y*grav_Y)) + ((grav_Z) *(grav_Z))) )))*180/M_PI; // Calcular o angulo

    Serial.print("acelerometro (g): X = ");
    Serial.println(grav_X);
    Serial.print("acelerometro theta: X = ");
    Serial.println(theta_X);

    delay(100);  // Intervalo de leitura dos dados

    soma_X = soma_X + grav_X;

    if (i == 100) {
      media_X = soma_X / 100;
      medias[j] = media_X;
      Serial.print("Media do eixo X: ");
      Serial.println(media_X);
      Serial.print("MediaX1 = ");
      Serial.println(medias[0]);
      Serial.print("MediaX2 = ");
      Serial.println(medias[1]);
      media_X = 0;
      soma_X = 0;
      j++;

      if (j == 2) {
        ganho_X = (medias[0] - medias[1]) / 2;
        offset_X = (medias[0] + medias[1]) / 2;
        Serial.print("Ganho = ");
        Serial.println(ganho_X);
        Serial.print("Offset = ");
        Serial.println(offset_X);
      }
    }
  }
}

// Calibracao para o eixo Y
void eixoY(){
  Serial.println("--------------- Eixo Y ---------------\n");
  for(i = 1; i <= 100; i++){

    Serial.print("acelerometro (g): Y = ");
    Serial.println(grav_Y);
    Serial.print("acelerometro theta: Y = ");
    Serial.println(theta_Y);

    delay(100);  // Intervalo de leitura dos dados

    soma_Y = soma_Y + grav_Y;

    if (i == 100) {
      media_Y = soma_Y / 100;
      medias[j] = media_Y;
      Serial.print("Media do eixo Y: ");
      Serial.println(media_Y);
      Serial.print("MediaY1 = ");
      Serial.println(medias[0]);
      Serial.print("MediaY2 = ");
      Serial.println(medias[1]);
      media_Y = 0;
      soma_Y = 0;
      j++;

      if (j == 2) {
        ganho_Y = (medias[0] - medias[1]) / 2;
        offset_Y = (medias[0] + medias[1]) / 2;
        Serial.print("Ganho = ");
        Serial.println(ganho_Y);
        Serial.print("Offset = ");
        Serial.println(offset_Y);
      }
    }
  }
}

// Calibracao para o eixo Z
void eixoZ(){
  Serial.println("--------------- Eixo Z ---------------\n");
  for(i = 1; i <= 100; i++){

    acelerometro_Z = dados(0x68, 0x3F);     // Le os valores do acelerometro
    grav_Z = ConversaoAcelerometro(acelerometro_Z);       // Converter para forca gravitacional  

    Serial.print("acelerometro (g): Z = ");
    Serial.println(grav_Z);

    delay(100);  // Intervalo de leitura dos dados

    soma_Z = soma_Z + grav_Z;

    if (i == 100) {
      media_Z = soma_Z / 100;
      medias[j] = media_Z;
      Serial.print("Media do eixo Z: ");
      Serial.println(media_Z);
      Serial.print("MediaZ1 = ");
      Serial.println(medias[0]);
      Serial.print("MediaZ2 = ");
      Serial.println(medias[1]);
      media_Z = 0;
      soma_Z = 0;
      j++;

      if (j == 2) {
        ganho_Z = (medias[0] - medias[1]) / 2;
        offset_Z = (medias[0] + medias[1]) / 2;
        Serial.print("Ganho = ");
        Serial.println(ganho_Z);
        Serial.print("Offset = ");
        Serial.println(offset_Z); 
      }
    }
  }
}

// Escolher eixos do acelerometro e chamar as calibracoes desses eixos
void acelerometro(){
  char eixo;
  if(Serial.available() > 0){
    Serial.print("Insira o eixo: ");
    eixo = Serial.read();
    Serial.println(eixo);
    if(eixo == 'x'){
      eixoX();
    } else if(eixo == 'y'){
       eixoY();
    } else if(eixo == 'z'){
      eixoZ();
    }
  }
}

/* // Funcao p/ configuracao e leitura do giroscopio
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
} */
