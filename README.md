# Controle de Servomotor por PWM

Este projeto utiliza o microcontrolador **RP2040** (Raspberry Pi Pico) para controlar o ângulo de um servomotor usando o módulo **PWM** (Pulse Width Modulation). O sistema foi desenvolvido utilizando a ferramenta **Pico SDK** e simula o controle de um **motor micro servo padrão**, com a configuração de PWM realizada na **GPIO 22** e um LED RGB controlado pela GPIO 12. O sistema também inclui uma movimentação suave entre os ângulos de 0° e 180°.

## Link do vídeo de descrição de funcionamento
https://drive.google.com/file/d/1dkyEH13dAd7nshAwoZFp4mAfRm8Z1clr/view?usp=sharing

## Funcionalidades do Programa

1. **Controle do Servomotor por PWM:**
   - O programa configura a GPIO 22 do RP2040 para emitir um sinal PWM com frequência de **50Hz** (período de 20ms).
   - O ciclo de trabalho (duty cycle) é ajustado para diferentes posições do servomotor:
     - **180 graus:** O ciclo de trabalho é configurado para **2400µs**.
     - **90 graus:** O ciclo de trabalho é configurado para **1470µs**.
     - **0 grau:** O ciclo de trabalho é configurado para **500µs**.
   - Após cada movimento, o sistema aguarda 5 segundos antes de realizar o próximo movimento.

2. **Movimentação Suave do Servomotor:**
   - O servomotor realiza um movimento contínuo entre os ângulos de 0° e 180°.
   - A posição do servomotor é alterada gradualmente em incrementos de **±5µs**, com um atraso de **10ms** entre os ajustes para garantir uma movimentação suave.

3. **Controle do LED RGB (GPIO 12):**
   - O programa utiliza uma interrupção do PWM para controlar o brilho de um LED RGB conectado à GPIO 12.
   - A iluminação do LED varia suavemente entre os valores de 0 a 255 (brilho), com uma alteração gradual no nível de luminosidade, simulando um efeito de "fade" (desvanecimento).
   - O comportamento do LED é controlado por uma interrupção, que faz com que o brilho suba até o valor máximo de 255 e depois desça até 0, repetidamente.

## Componentes Utilizados

- **Microcontrolador**: Raspberry Pi Pico W (RP2040)
- **Servomotor**: Motor Micro Servo Padrão
- **LED RGB**: Conectado à GPIO 12 para controle de brilho

## Código Fonte

O código realiza as seguintes etapas:

1. Inicializa o PWM para controlar o servomotor e o LED RGB.
2. Configura a GPIO 22 para enviar um sinal PWM de 50Hz, com ciclos de trabalho ajustáveis.
3. Realiza movimentos do servomotor em 0°, 90° e 180°, com tempos de espera entre cada posição.
4. Realiza a movimentação suave do servomotor entre os ângulos de 0° e 180°.
5. Controla o LED RGB para alterar sua luminosidade utilizando interrupções de PWM.

## Como executar

# Caso seja no simulador:

1. Compile o código
2. Entre em diagram.json
3. Clique no botão de executar e veja a execução do programa
   
# Caso seja na placa (para ver o LED)

1. Conecte à placa BitDogLab no computador via cabo USB.
2.Compile o código.
3.Faça o BOOTSEL na placa e clique em run e assista a execução
