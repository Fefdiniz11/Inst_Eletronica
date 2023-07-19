#include <Wire.h>
#include <math.h>

// Variaveis 
long acelerometro_X, acelerometro_Y, acelerometro_Z, GyrX, GyrY, GyrZ;  
float grav_X, grav_Y, grav_Z, velocidadeAngularX, velocidadeAngularY, velocidadeAngularZ;;
float soma_X = 0, media_X = 0, ganho_X = 0, offset_X = 0, soma_Y = 0,  media_Y = 0, ganho_Y = 0, offset_Y = 0, soma_Z = 0,  media_Z = 0, ganho_Z = 0, offset_Z = 0;
float mediaS_X[2] = {0.0,0.0}, mediaS_Y[2] = {0.0, 0.0}, mediaS_Z[2] = {0.0, 0.0};
float dt = 0;
float angulo_X = 0, angulo = 0, theta = 0, theta_X = 0,angulo_Ref = 0; 

float temp_prev=0;
char eixo;
int i = 0, j = 0;

void setup() {
  Serial.begin(115200);   // Baud rate p/ ESP32 - Microcontrolador utilizado nesse projeto
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

// Le os dados do acelerometro e do giroscopio
int16_t dados(int endereco, byte regEndereco) {
  // Le 2 bytes de dados do registrador especificado
  Wire.beginTransmission(endereco);                 // Endereco I2C, no caso da MPU-6050 eh 0x68
  Wire.write(regEndereco);                          // Endereco do registrador que contem os dados
  Wire.endTransmission(false);                      // Encerra a transmissão I2C, mas mantém a conexão com o dispositivo aberta 

  Wire.requestFrom(endereco, 2, true);              // Solicita 2 bytes do endereco e encerra apos a leitura
  byte byte_maior = Wire.read();                    // Le os dois bytes de dados solicitados do dispositivo
  byte byte_menor = Wire.read();                    

  int16_t valor = (byte_maior << 8) | byte_menor;   // combina os dois bytes lidos em um valor inteiro de 16 bits 
  return valor;
}

// Datasheet fornece que a sensibilidade é 16384 LSB/g -> conversao p/ forca gravitacional ou aceleracao
float ConversaoAcelerometro(long valor){
  float grav = valor / 16384.0; 
  return grav;
}

// Datasheet fornece sensibilidade de 131 LSB/(graus/s) -> conversao p/ graus/s
float ConversaoGiroscopio(long valor){
  float gyro = valor / 131.0; 
  return gyro;
}


// Calibracao para os eixos
void eixos(){

  Serial.print("\n--------------- Eixos ---------------\n");

  for(i = 1; i <= 100; i++){

    acelerometro_X = dados(0x68, 0x3B);                   // Le os valores do acelerometro
    grav_X = ConversaoAcelerometro(acelerometro_X);       // Converter para forca gravitacional
    acelerometro_Y = dados(0x68, 0x3D);                   // Le os valores do acelerometro
    grav_Y = ConversaoAcelerometro(acelerometro_Y);       // Converter para forca gravitacional
    acelerometro_Z = dados(0x68, 0x3F);                   // Le os valores do acelerometro
    grav_Z = ConversaoAcelerometro(acelerometro_Z);       // Converter para forca gravitacional       
    velocidadeAngularX = dados(0x68, 0x43);               //Le os valores do giroscopio
    GyrX = ConversaoGiroscopio(velocidadeAngularX);       //Converter para graus por segundo
    // velocidadeAngularY = dados(0x68, 0x45);               //Le os valores do giroscopio
    // GyrY = ConversaoGiroscopio(velocidadeAngularY);       //Converter para graus por segundo
    // velocidadeAngularZ = dados(0x68, 0x47);               //Le os valores do giroscopio
    // GyrZ = ConversaoGiroscopio(velocidadeAngularZ);       //Converter para graus por segundo
    
    // Angulo de inclinacao por trigonometria para o acelerometro - Formula do angulo de inclinacao
    theta = (atan( (grav_X/(sqrt ((grav_Y*grav_Y)) + ((grav_Z) *(grav_Z))) )))*180/M_PI; // Calcular o angulo

    // Para ajustar os valores dentro da circunferencia de 0 a 360
    if(theta < 0){
      theta_X = theta + 360;
    }
    else{
      theta_X = theta;
    }

    // Angulo de inclinacao por integracao numerica para o giroscopio - somando a velocidade angular multiplicada pelo intervalo de tempo
    dt=(millis() - temp_prev)/1000;     // Intervalo de tempo
    temp_prev=millis();
    
    angulo_Ref = (atan( (grav_X/(sqrt ((grav_Y*grav_Y)) + ((grav_Z) *(grav_Z))) )))*180/M_PI;   // Valor de referencia para atualizar a cada iteracao  
    angulo = angulo_Ref + (GyrX * dt); // Calcula o angulo de rotacao em torno do eixo X 
    
    // Para ajustar os valores dentro da circunferencia de 0 a 360
    if(angulo < 0){
      angulo_X = angulo + 360;
    }
    else{
      angulo_X = angulo;
    }

   Serial.print("acelerometro: X = ");
   Serial.print(grav_X);
   Serial.print(", Y = ");
   Serial.print(grav_Y);
   Serial.print(", Z = ");
   Serial.println(grav_Z);
   Serial.print("\n");
   Serial.print("acelerometro theta X = ");
   Serial.print(theta_X);
   Serial.print(";");
   Serial.print("giroscopio angulo: X: ");
   Serial.println(angulo_X);
   Serial.print("\n");

    
    delay(100);  // Intervalo de leitura dos dados

    soma_X = soma_X + grav_X;
    soma_Y = soma_Y + grav_Y;
    soma_Z = soma_Z + grav_Z;

    if (i == 100) {

      media_X = soma_X / 100;
      mediaS_X[j] = media_X;
      media_Y = soma_Y / 100;
      mediaS_Y[j] = media_Y;
      media_Z = soma_Z / 100;
      mediaS_Z[j] = media_Z;

      Serial.print("Media do eixo X: ");
      Serial.println(media_X);
      Serial.print("MediaX1 = ");
      Serial.print(mediaS_X[0]);
      Serial.print(" | MediaX2 = ");
      Serial.println(mediaS_X[1]);
      Serial.print("\n");
      Serial.print("Media do eixo Y: ");
      Serial.println(media_Y);
      Serial.print("MediaY1 = ");
      Serial.print(mediaS_Y[0]);
      Serial.print(" | MediaY2 = ");
      Serial.println(mediaS_Y[1]); 
      Serial.print("\n");
      Serial.print("Media do eixo Z: ");
      Serial.println(media_Z);
      Serial.print("MediaZ1 = ");
      Serial.print(mediaS_Z[0]);
      Serial.print(" | MediaZ2 = ");
      Serial.println(mediaS_Z[1]);
      Serial.print("\n");

      media_X = 0;
      soma_X = 0;
      media_Y = 0;
      soma_Y = 0;
      media_Z = 0;
      soma_Z = 0;

      j++;

      if (j == 2) {
        ganho_X = (mediaS_X[0] - mediaS_X[1]) / 2;
        offset_X = (mediaS_X[0] + mediaS_X[1]) / 2;
        ganho_Y = (mediaS_Y[0] - mediaS_Y[1]) / 2;
        offset_Y = (mediaS_Y[0] + mediaS_Y[1]) / 2;
        ganho_Z = (mediaS_Z[0] - mediaS_Z[1]) / 2;
        offset_Z = (mediaS_Z[0] + mediaS_Z[1]) / 2;
        
        Serial.print("Ganho: X = ");
        Serial.print(ganho_X);
        Serial.print(", Y = ");
        Serial.print(ganho_Y);
        Serial.print(", Z = ");
        Serial.println(ganho_Z);;
        Serial.print("\n");
        Serial.print("Offset X = ");
        Serial.print(offset_X); 
        Serial.print(", Y = ");
        Serial.print(offset_Y);
        Serial.print(", Z = ");
        Serial.println(offset_Z);
        
      }
    }
  }
}

// Iniciar leitura dos eixos do acelerometro e chamar as calibracoes desses eixos
void acelerometro(){
  char eixo;
  if(Serial.available() > 0){
    Serial.print("Insira a letra 'i' para iniciar: ");
    eixo = Serial.read();
    Serial.println(eixo);
    if(eixo == 'i'){
      eixos();
    } 
  }
}