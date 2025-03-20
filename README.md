# 📊 **Data Logger para Monitoramento Ambiental** 🌍

Este projeto visa criar um dispositivo de registro de dados (data logger) dedicado ao monitoramento de condições ambientais em espaços controlados. O sistema monitora a temperatura, a umidade relativa do ar e a luminosidade em tempo real, armazenando essas informações em uma memória EEPROM para análises posteriores.

## Objetivo

Desenvolver um dispositivo para o registro contínuo de dados ambientais, utilizando um microcontrolador ATmega 328P e diversos sensores para coleta de informações sobre temperatura, umidade e luminosidade. O sistema será equipado com uma interface de usuário simples, utilizando um display LCD e botões para interação.

## Funcionalidades

-  🌡️ **Monitoramento de Temperatura**: Mede a temperatura ambiente utilizando um sensor DHT-11.
- 💧 **Monitoramento de Umidade**: Mede a umidade relativa do ar com o sensor DHT-11.
- 💡 **Monitoramento de Luminosidade**: Mede a intensidade da luz ambiente através de um sensor LDR.
- 🎲 **Armazenamento de Dados**: Utiliza memória EEPROM para armazenar as leituras dos sensores com timestamp.
- **Exibição de Dados**: Mostra as leituras dos sensores em tempo real no display LCD I2C.
- 🔊 **Alerta Sonoro e Visual**: LEDs e buzzer são usados para alertar o usuário em caso de condições fora dos limites pré-estabelecidos.

  ## Informações de Uso
  - Ao ligar o sistema, uma animação será exibida, representando uma garrafa de vinho tombando e sendo esvaziada, enquanto o nome da empresa aparece ("CELLARIUM VINUM"). Após isso, o slogan "ADEGA DA REALEZA" é exibido.
  - O LED RGB ficará na cor branca enquanto inicializa o sistema.
  - Terminada a inicialização, o display passa a exibir a data e hora atuais, sendo possível alterar a tela por meio de um botão. Dessa forma, será possível visualizar os valores fornecidos pelos sensores.
  - 🟢 O LED verde simboliza que os gatilhos estão dentro dos parâmetros.
  - 🟣 O LED roxo simboliza uma iminência ao erro, ou seja, algum dos gatilhos está próximo a exceder o limite.
  - 🔴 O LED vermelho simboliza que algum gatilho passou do limite.
  - Os valores, quando fora dos limites, são gravados na memória EEPROM com data e horário, sendo possível acompanha-los no monitor serial.

## Diferencial
- O nosso código é capaz de alertar o usuário caso haja a iminência de uma anomalia nos valores recomendados.
- Exemplo: No código, definimos o limite de temperatura máxima como 25ºC. Ao exceder a temperatura, o LED fica com Luz Vermelha.
- Caso a temperatura esteja próxima de 25°C (A partir de 22.5ºC) o LED ficará com a Luz Roxa, indicando o usuário que algum valor está próximo de ultrapassar o limite aceitável.
  
## Especificações Técnicas

- **Microcontrolador**: ATmega 328P
- **Armazenamento**: Memória EEPROM para registro dos dados.
- **Relógio de Tempo Real (RTC)**: Para marcação precisa dos timestamps.
- **Interface de Usuário**: Display LCD 16x2 com interface I2C.
- **Controles**: Botões para navegação e configuração do dispositivo.
- **Indicadores Visuais**: LEDs para status operacional e alertas.
- **Alertas Sonoros**: Buzzer para avisos sonoros.
- **Sensores**: DHT-11 (Temperatura e Umidade) e LDR (Luminosidade).

## Níveis dos Gatilhos (Triggers)

| Sensor        | Faixa de Gatilho   |
|---------------|--------------------|
| Temperatura   | 15°C a 25°C        |
| Luminosidade  | 0% a 30%           |
| Umidade       | 30% a 50%          |

## Lista de Materiais

1. **MCU (Microcontrolador)**: ATmega 328P - Arduino Uno R3
2. **LDR (Sensor de Luminosidade)** + **Resistor de 10KOhm**
3. **DHT-11** (Sensor de Temperatura e Umidade)
4. **LCD 16x2** - I2C
5. **Bateria de 9V** + Suporte para bateria
6. **RTC (Real Time Clock)**
7. **Buzzer**
8. Protoboard, jumpers, LEDs, resistores

## 🔧 Instalação

1. Monte o circuito utilizando a protoboard e os componentes listados na seção anterior.
2. Conecte o **LCD 16x2 I2C** ao Arduino utilizando a interface I2C.
3. Conecte os sensores **DHT-11** e **LDR** aos pinos do Arduino.
4. O **RTC** também deve ser conectado ao Arduino para garantir que as leituras sejam marcadas com timestamps precisos.

### </> Carregando o Código

1. Abra o código fonte no **Arduino IDE**.
2. Conecte o Arduino ao seu computador via cabo USB.
3. Selecione a porta correta no Arduino IDE.
4. Carregue o código no Arduino.

Ao conectar o Arduino, o display LCD será acionado automaticamente e os dados serão exibidos. Use os botões para alternar entre as diferentes leituras.

## Uso

Após o carregamento do código no Arduino:

1. Conecte o dispositivo à alimentação (por exemplo, usando uma bateria de 9V).
2. O **LCD** exibirá as leituras de temperatura, umidade e luminosidade em tempo real.
3. O usuário pode interagir com o dispositivo utilizando os botões para alternar entre as leituras.
4. Quando os valores de temperatura, umidade ou luminosidade saírem da faixa definida, o sistema acionará os **alertas sonoros e visuais**.

## 🤝 Contribuindo

Se você deseja contribuir com este projeto, siga as etapas abaixo:
1. Faça um **fork** deste repositório.
2. Crie uma branch para sua melhoria ou correção (`git checkout -b feature/nova-funcionalidade`).
3. Faça as alterações necessárias e **commit** suas mudanças (`git commit -am 'Adiciona nova funcionalidade'`).
4. Envie suas alterações para o repositório remoto (`git push origin feature/nova-funcionalidade`).
5. Abra um **pull request** para revisão.


## Licença

Este projeto está licenciado sob a **MIT License** - consulte o arquivo [LICENSE](LICENSE) para mais detalhes.

## Contribuidores

Este projeto foi desenvolvido por:

*Arthur Destro*  
*Alex Saifi de Souza*  
*Gustavo Mauriz*  
*Vinicius Strazza*

Agradecimentos especiais a todos que contribuíram com ideias, sugestões e suporte durante o desenvolvimento.


## 📧 Contato

Caso tenha dúvidas ou sugestões sobre o projeto, entre em contato comigo por [e-mail](strazza2005@gmail.com).
