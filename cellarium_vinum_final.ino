#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <EEPROM.h>

// Configurações gerais
#define LOG_OPTION 1
#define UTC_OFFSET 0
#define EEPROM_SIZE 160  // Cada registro ocupa 16 bytes * 10 registros

// Configuração do LCD
#define col 16
#define lin 2
#define ende 0x27
LiquidCrystal_I2C lcd(ende, col, lin);

// Sensor de tempo e umidade
RTC_DS3231 rtc;
DHT dht(13, DHT11);

// Pinos dos componentes
int pinoLDR = A0;
int pinoBuzzer = 7;
int ledRed = 6, ledGreen = 5, ledBlue = 4;
int pinoBotao = 2;

// Limites e gaps
float tempMin = 15.0; // Limite de temperatura mínimo
float tempMax = 25.0; // Limite de temperatura máximo
float luzMin = 0.0; // Limite de luminosidade mínimo
float luzMax = 30.0; // Limite de luminosidade máximo
float umiMin = 30.0; // Limite de umidade mínimo
float umiMax = 50.0; // Limite de umidade máximo
float gapTemp = 2.5; // Range de tolerância antes da anomalia da temperatura - Indicador de risco
float gapLuz = 5.0; // Range de tolerância antes da anomalia da luminosidade - Indicador de risco
float gapUmi = 5.0; // Range de tolerância antes da anomalia da umidade - Indicador de risco

// Estados do sistema
int estadoTela = 0;
unsigned long tempoAnterior = 0;
unsigned long debounceTime = 0;
const int intervalo = 1000; // Atualiza a cada 1s
int posEEPROM = 0; // Posição atual na EEPROM

// Arrays para armazenar as últimas 5 leituras - Para calcular a média
float leiturasTemp[5];
float leiturasUmi[5];
float leiturasLuz[5];
int iteradorAtual = 0;
int numeroLeituras = 0;

//Configuração de tempo
uint32_t lastTimestamp = 0;

//Garrafa vazia
byte name0x0[8] = { B00000, B00000, B00000, B00001, B00010, B00010, B00001, B00001 };
byte name1x0[8] = { B00000, B00000, B00000, B11000, B00100, B00100, B01000, B01000 };
byte name0x1[8] = { B00010, B00100, B01000, B01000, B01000, B01000, B00100, B00011 };
byte name1x1[8] = { B00100, B00010, B00001, B00001, B00001, B00001, B00010, B11100 };
//Garrafa cheia
byte cheia1[8] = { B00000, B00000, B00000, B00001, B00011, B00011, B00001, B00001 };
byte cheia2[8] = { B00000, B00000, B00000, B11000, B11100, B11100, B11000, B11000 };
byte cheia3[8] = { B00011, B00111, B01111, B01111, B01111, B01111, B00111, B00011 };
byte cheia4[8] = { B11100, B11110, B11111, B11111, B11111, B11111, B11110, B11100 };
//Garrafa cheia deitada
byte deitada1[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00111 };
byte deitada2[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000 };
byte deitada3[8] = { B01111, B11111, B11111, B11111, B11111, B11111, B01111, B00111 };
byte deitada4[8] = { B11000, B11100, B11111, B11111, B11111, B11100, B11000, B10000 };
byte deitada5[8] = { B00000, B11000, B11100, B11100, B11100, B11000, B00000, B00000 };
//Garrafa deitada vazando
byte vazando1[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00111 };
byte vazando2[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B10000 };
byte vazando3[8] = { B01000, B10000, B10000, B10000, B10001, B10011, B01111, B00111 };
byte vazando4[8] = { B11000, B00100, B00011, B00011, B11111, B11100, B11000, B10000 };
byte vazando5[8] = { B00000, B11000, B00100, B11111, B11101, B11000, B00000, B00000 };
byte vazando6[8] = { B00000, B00000, B00000, B00000, B11000, B01100, B00110, B00011 };
//Inundação
byte inu1[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu2[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu3[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu4[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu5[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu6[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu7[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu8[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu9[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu10[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu11[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu12[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu13[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu14[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };
byte inu15[] = { B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111 };

void setup() {
    // Inicializa o serial (para leitura da EEPROM)
    Serial.begin(9600);
    // Inicializa o LCD
    lcd.init();
    lcd.backlight();
    // Declarando as saídas de acordo com o componente
    pinMode(pinoBotao, INPUT_PULLUP);
    pinMode(pinoBuzzer, OUTPUT);
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    // LED brilhando em branco
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledBlue, HIGH);
    // Inicializa o sensor de temperatura
    dht.begin();
   // Caso haja erro no RTC
    if (!rtc.begin()) {
        lcd.print("Erro RTC");
        while (1);
    }
    // Em caso de sucesso na inicialização, configura a data e a hora armazenada no LCD
    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    //Garrafa vazia
    lcd.createChar(0, name0x0);
    lcd.createChar(1, name1x0);
    lcd.createChar(2, name0x1);
    lcd.createChar(3, name1x1);
    lcd.createChar(4, cheia1);
    lcd.createChar(5, cheia2);
    lcd.createChar(6, cheia3);
    lcd.createChar(7, cheia4);

    lcd.setCursor(1,0);
    lcd.write(byte(0));
    
    lcd.setCursor(2,0);
    lcd.write(byte(1));
    
    lcd.setCursor(1,1);
    lcd.write(byte(2));
    
    lcd.setCursor(2,1);
    lcd.write(byte(3));
    delay(1200);
    lcd.clear();
      
    //Garrafa cheia
    lcd.setCursor(1,0);
    lcd.write(byte(4));
    
    lcd.setCursor(2,0);
    lcd.write(byte(5));
    
    lcd.setCursor(1,1);
    lcd.write(byte(6)); 
    
    lcd.setCursor(2,1);
    lcd.write(byte(7));
    
    delay(1500);
    lcd.clear();
    
    //Garrafa cheia deitada
    lcd.createChar(8, deitada1);
    lcd.createChar(9, deitada2);
    lcd.createChar(10, deitada3);
    lcd.createChar(11, deitada4);
    lcd.createChar(12, deitada5);
    
    lcd.setCursor(4,0);
    lcd.write(byte(8));
    
    lcd.setCursor(5,0);
    lcd.write(byte(9));
     
    lcd.setCursor(4,1);
    lcd.write(byte(10));
    
    lcd.setCursor(5,1);
    lcd.write(byte(11));
    
    lcd.setCursor(6,1);
    lcd.write(byte(12));
    
    delay(1000);
    lcd.clear();
    
    //Garrafa deitada vazando
    lcd.createChar(13, vazando1);
    lcd.createChar(14, vazando2);
    lcd.createChar(15, vazando3);
    lcd.createChar(0, vazando4);
    lcd.createChar(1, vazando5);
    lcd.createChar(2, vazando6);
    
    lcd.setCursor(4,0);
    lcd.write(byte(13));
     
    lcd.setCursor(5,0);
    lcd.write(byte(14));
    
    lcd.setCursor(4,1);
    lcd.write(byte(15));
    
    lcd.setCursor(5,1);
    lcd.write(byte(0));
    
    lcd.setCursor(6,1);
    lcd.write(byte(1));
    
    lcd.setCursor(7,1);
    lcd.write(byte(2));
    
    delay(1500);
    lcd.clear();
    
    //Inundando
    lcd.createChar(3, inu1);
    lcd.createChar(4, inu2);
    lcd.createChar(5, inu3);
    lcd.createChar(6, inu4);
    lcd.createChar(7, inu5);
    lcd.createChar(8, inu6);
    lcd.createChar(9, inu7);
    lcd.createChar(10, inu8);
    lcd.createChar(11, inu9);
    lcd.createChar(12, inu10);
    lcd.createChar(13, inu11);
    lcd.createChar(14, inu12);
    lcd.createChar(15, inu13);
    lcd.createChar(0, inu14);
    lcd.createChar(1, inu15);
    
    lcd.setCursor(0, 1);  
    lcd.write(byte(3));  
  
    lcd.setCursor(1, 1);  
    lcd.write(byte(4));  
  
    lcd.setCursor(2, 1);  
    lcd.write(byte(5));  
  
    lcd.setCursor(3, 1);  
    lcd.write(byte(6));  
  
    lcd.setCursor(4, 1);  
    lcd.write(byte(7));  
  
    lcd.setCursor(5, 1);  
    lcd.write(byte(8));  
  
    lcd.setCursor(6, 1);  
    lcd.write(byte(9));  
  
    lcd.setCursor(7, 1);  
    lcd.write(byte(10));  
  
    lcd.setCursor(8, 1);  
    lcd.write(byte(11));  
  
    lcd.setCursor(9, 1);  
    lcd.write(byte(12));  
  
    lcd.setCursor(10, 1);  
    lcd.write(byte(13));  
  
    lcd.setCursor(11, 1);  
    lcd.write(byte(14));  
  
    lcd.setCursor(12, 1);  
    lcd.write(byte(15));  
  
    lcd.setCursor(13, 1);  
    lcd.write(byte(0));  
  
    lcd.setCursor(14, 1);  
    lcd.write(byte(1));  
  
    lcd.setCursor(15, 1);  
    lcd.write(byte(2));  
    
    delay(500);
    
    // Nome do projeto
    lcd.setCursor(3,0);
    lcd.print("CELLARIUM");
    
    lcd.setCursor(5,1);
    lcd.print("VINUM");
              
    delay(2500);
    lcd.clear();

    // Bordão do projeto
    lcd.setCursor(4,0);
    lcd.print("A ADEGA"); 
    lcd.setCursor(3,1);
    lcd.print("DA REALEZA");
    delay(2500);
    lcd.clear();
}

// Função auxiliar construída para acionar o alarme e ligar os leds quando necessário
void ativarAlarme(int nivel = 0) {
    switch (nivel) {
        case 1: // 
            digitalWrite(ledRed, HIGH);
            digitalWrite(ledGreen, LOW);
            digitalWrite(ledBlue, LOW);
            tone(pinoBuzzer, 1000, 1000);
            break;
        case 2:
            digitalWrite(ledRed, HIGH);
            digitalWrite(ledGreen, LOW);
            digitalWrite(ledBlue, HIGH);
            noTone(pinoBuzzer);
            break;
        default:
            digitalWrite(ledRed, LOW);
            digitalWrite(ledGreen, HIGH);
            digitalWrite(ledBlue, LOW);
            noTone(pinoBuzzer);
    }
}

// Função para registrar as anomalias 
void salvarEEPROM() {
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    int ldr = analogRead(pinoLDR);
    float luz = map(ldr, 1023, 0, 0, 100);
    
    // Obter data e hora
    DateTime now = rtc.now();
    uint32_t timestamp = now.unixtime(); // Obtém o timestamp em segundos

    // Verifica se passou pelo menos 60 segundos desde a última gravação
    if (timestamp - lastTimestamp < 60) {
        return; // Sai da função se não passou tempo suficiente
    }
    
    // Atualizar o `lastTimestamp` somente após gravar
    lastTimestamp = timestamp;

    // Verifica as condições para salvar na EEPROM
    if ((luz > luzMax) || (luz < luzMin) ||
        (temperatura > tempMax) || (temperatura < tempMin) ||
        (umidade > umiMax) || (umidade < umiMin)) {

        if (posEEPROM >= EEPROM_SIZE) posEEPROM = 0; // Buffer circular

        // Salvar temperatura, umidade, luz e hora
        EEPROM.put(posEEPROM, temperatura);          // 4 bytes
        EEPROM.put(posEEPROM + 4, umidade);          // 4 bytes
        EEPROM.put(posEEPROM + 8, luz);              // 4 bytes
        EEPROM.put(posEEPROM + 12, timestamp);       // 4 bytes (data/hora como uint32_t)

        posEEPROM += 16; // Cada registro ocupa 16 bytes
    }
}

// Função auxiliar para calcular a média das leituras das grandezas
float calculaMedia(float readings[], int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += readings[i];
    }
    return sum / size;
}

// Função auxiliar para atualizar a tela conforme o botão é apertado e os ciclos de loop acontecem
void atualizarTela(int estado) {
    lcd.clear();
    DateTime now = rtc.now();
    now = DateTime(now.unixtime() + UTC_OFFSET * 3600);  // Aplica o offset UTC

    switch (estado) {
        case 0: // Tela de data e hora
            lcd.print("Data: ");
            lcd.print(now.day() < 10 ? "0" : "");
            lcd.print(now.day());
            lcd.print("/");
            lcd.print(now.month() < 10 ? "0" : "");
            lcd.print(now.month());
            lcd.print("/");
            lcd.print(now.year());

            lcd.setCursor(0, 1);
            lcd.print("Hora: ");
            lcd.print(now.hour() < 10 ? "0" : "");
            lcd.print(now.hour());
            lcd.print(":");
            lcd.print(now.minute() < 10 ? "0" : "");
            lcd.print(now.minute());
            lcd.print(":");
            lcd.print(now.second() < 10 ? "0" : "");
            lcd.print(now.second());
            break;

        case 1: // Tela de temperatura
            lcd.print("Temp: ");
            lcd.print(String(calculaMedia(leiturasTemp, numeroLeituras)));
            lcd.write(byte(223)); // Exibe o símbolo de grau (°)
            lcd.print("C");
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            if (calculaMedia(leiturasTemp, numeroLeituras) > tempMax - gapTemp) {
                lcd.print("Alta");
            } else if (calculaMedia(leiturasTemp, numeroLeituras) < tempMin + gapTemp) {
                lcd.print("Baixa");
            } else {
                lcd.print("Boa");
            }
            break;

        case 2: // Tela de luminosidade
            lcd.print("Luz: ");
            lcd.print(String(calculaMedia(leiturasLuz, numeroLeituras)) + "%");
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            if (calculaMedia(leiturasLuz, numeroLeituras) > luzMax - gapLuz) {
                lcd.print("Alta");
            } else if (calculaMedia(leiturasLuz, numeroLeituras) < luzMin + gapLuz) {
                lcd.print("Baixa");
            } else {
                lcd.print("Boa");
            }
            break;

        case 3: // Tela de umidade
            lcd.print("Umid: ");
            lcd.print(String(calculaMedia(leiturasUmi, numeroLeituras)) + "%");
            lcd.setCursor(0, 1);
            lcd.print("Status: ");
            if (calculaMedia(leiturasUmi, numeroLeituras) > umiMax - gapUmi) {
                lcd.print("Alta");
            } else if (calculaMedia(leiturasUmi, numeroLeituras) < umiMin + gapUmi) {
                lcd.print("Baixa");
            } else {
                lcd.print("Boa");
            }
            break;
    }
}

void loop() {
    // Para visualizar o serial
    if(LOG_OPTION) exibirEEPROM();

    // Variável auxiliar na troca de telas
    unsigned long tempoAtual = millis();
   
    // Troca de tela no clique do botão
    if (digitalRead(pinoBotao) == LOW && (tempoAtual - debounceTime) > 10) { // Debounce ajustado para 10 ms
        debounceTime = tempoAtual; // Atualiza o tempo de debounce
        estadoTela = (estadoTela + 1) % 4; // Muda a tela
    }

    // Atualização cronometrada da EEPROM, em casos de anomalia
    if (tempoAtual - tempoAnterior >= intervalo) {
        tempoAnterior = tempoAtual;
        atualizarTela(estadoTela);
        salvarEEPROM();
    }

    // Leitura dos sensores
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    float tensaoLDR = analogRead(pinoLDR);
    float intensidadeLuz = map(tensaoLDR, 1023, 0, 0, 100); // Transformando a leitura de tensão no LDR para taxa de luminosidade

    // Armazenar as leituras nos arrays
    leiturasTemp[iteradorAtual] = temperatura;
    leiturasUmi[iteradorAtual] = umidade;
    leiturasLuz[iteradorAtual] = intensidadeLuz;

    // Atualizar o índice e o número de leituras
    iteradorAtual = (iteradorAtual + 1) % 5;
    if (numeroLeituras < 5) {
        numeroLeituras++;
    }

    // Calcular a média das leituras inseridas
    float mediaTemp = calculaMedia(leiturasTemp, numeroLeituras);
    float mediaUmi = calculaMedia(leiturasUmi, numeroLeituras);
    float mediaLuz = calculaMedia(leiturasLuz, numeroLeituras);

    // Registro dos valores capturados pelos sensores no serial - para acompanhar funcionamento
    Serial.println("Temperatura: " + String(mediaTemp));
    Serial.println("Umidade: " + String(mediaUmi));
    Serial.println("Luminosidade: " + String(mediaLuz));
    Serial.println("/");

    // Condição para determinar anomalia
    bool condicao_fora = (mediaLuz > luzMax) || (mediaLuz < luzMin) ||
        (mediaTemp > tempMax) || (mediaTemp < tempMin) ||
        (mediaUmi > umiMax) || (mediaUmi < umiMin);
   // Condição para indicar eminente anomalia
    bool condicao_risco = (mediaLuz > (luzMax - gapLuz)) ||
        (mediaLuz < (luzMin + gapLuz)) ||
        (mediaTemp > (tempMax - gapTemp)) ||
        (mediaTemp < (tempMin + gapTemp)) ||
        (mediaUmi > (umiMax - gapUmi)) ||
        (mediaUmi < (umiMin + gapUmi));

    // Ativar o tratamento 
    if (condicao_fora)
    {
        ativarAlarme(1);  // Alarme apenas para condição "fora", de anomalia. Led vermelho
    }
    else if (condicao_risco)
    {
        ativarAlarme(2);  // Led liga azul (amarelo é difícil de distinguir) para indicar eminente anomalia
    }
    else
    {
        ativarAlarme();  // Nenhum alarme, led ligando verde
    }
    delay(1000);
}

// Função para mostrar aos registros da EEPROM no serial
void exibirEEPROM() {
    for (int pos = 0; pos < EEPROM_SIZE; pos += 16) { // Cada registro completo ocupa 16 bytes
        float temperatura, umidade, luz;
        uint32_t timestamp;

        // Ler dados da EEPROM
        EEPROM.get(pos, temperatura);    // Lê 4 bytes para a temperatura
        EEPROM.get(pos + 4, umidade);    // Lê 4 bytes para a umidade
        EEPROM.get(pos + 8, luz);        // Lê 4 bytes para a luz
        EEPROM.get(pos + 12, timestamp); // Lê 4 bytes para o timestamp

        // Verifica se os dados são válidos (não preenchidos com 0xFF)
        if (isnan(temperatura) || isnan(umidade) || luz < 0 || luz > 100) break;

        // Converter timestamp para data e hora (supondo que use um RTC)
        DateTime datetime = DateTime(timestamp);

        // Exibir os dados no monitor serial
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print(" °C, Umidade: ");
        Serial.print(umidade);
        Serial.print(" %, Luz: ");
        Serial.print(luz);
        Serial.print(" %, Data/Hora: ");
        Serial.print(datetime.year(), DEC);
        Serial.print("-");
        Serial.print(datetime.month(), DEC);
        Serial.print("-");
        Serial.print(datetime.day(), DEC);
        Serial.print(" ");
        Serial.print(datetime.hour(), DEC);
        Serial.print(":");
        Serial.print(datetime.minute(), DEC);
        Serial.print(":");
        Serial.println(datetime.second(), DEC);
    }
}
