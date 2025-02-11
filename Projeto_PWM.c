#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_GPIO 22  // Pino do servomotor
#define PWM_FREQ 50    // Frequência do PWM em Hz (50Hz = 20ms de período)
#define PWM_WRAP 20000 // PWM_WRAP baseado em um clock de 1MHz (20ms = 20000 ticks)
#define LEDPin 12

void set_pwm_duty(uint gpio, uint duty_us)
{
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint level = (duty_us * PWM_WRAP) / 20000; // Converte tempo para duty cycle proporcional
    pwm_set_gpio_level(gpio, level);
}

// tratamento da interrupção do PWM
void wrapHandler()
{
    static int fade = 0;                          // nível de iluminação
    static bool rise = true;                      // flag para elevar ou reduzir a iluminação
    pwm_clear_irq(pwm_gpio_to_slice_num(LEDPin)); // resetar o flag de interrupção

    if (rise)
    {           // caso a iluminação seja elevada
        fade++; // aumenta o nível de brilho
        if (fade > 255)
        {                 // caso o fade seja maior que 255
            fade = 255;   // iguala fade a 255
            rise = false; // muda o flag rise para redução do nível de iluminação
        }
    }
    else
    {           // caso a iluminação seja reduzida
        fade--; // reduz o nível de brilho
        if (fade < 0)
        {                // Icaso o fade seja menor que 0
            fade = 0;    // iguala fade a 0
            rise = true; // muda o flag rise para elevação no nível de iluminação
        }
    }

    pwm_set_gpio_level(LEDPin, fade * fade); // define o ciclo ativo (Ton) de forma quadrática, para acentuar a variação de luminosidade.
}

// Configuração do PWM com interrupção
uint pwm_setup_irq()
{

    gpio_set_function(LEDPin, GPIO_FUNC_PWM);      // habilitar o pino GPIO como PWM
    uint sliceNum = pwm_gpio_to_slice_num(LEDPin); // obter o canal PWM da GPIO

    pwm_clear_irq(sliceNum);                              // resetar o flag de interrupção para o slice
    pwm_set_irq_enabled(sliceNum, true);                  // habilitar a interrupção de PWM para um dado slice
    irq_set_exclusive_handler(PWM_IRQ_WRAP, wrapHandler); // Definir um tipo de interrupção.
    // Esta interrupção (PWM_IRQ_WRAP) é gerada quando um contador de um slice atinge seu valor de wrap
    irq_set_enabled(PWM_IRQ_WRAP, true); // Habilitar ou desabilitar uma interrupção específica

    pwm_config config = pwm_get_default_config(); // obtem a configuração padrão para o PWM
    pwm_config_set_clkdiv(&config, 4.f);          // define o divisor de clock do PWM
    pwm_init(sliceNum, &config, true);            // inicializa o PWM com as configurações do objeto

    return sliceNum;
}

int main()
{
    stdio_init_all();

    // Configura GPIO 22 como saída PWM
    gpio_set_function(SERVO_GPIO, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_GPIO);
    uint sliceNum = pwm_setup_irq(); // função que inicializa o PWM com interrupção por wrap

    pwm_set_irq_enabled(sliceNum, true); // habilita a interrupção

    // Configura PWM com período de 20ms (50Hz)
    pwm_set_wrap(slice, PWM_WRAP);
    pwm_set_clkdiv(slice, 125.0); // Ajuste do clock
    pwm_set_enabled(slice, true);

    // Move para 180° e espera 5 segundos
    set_pwm_duty(SERVO_GPIO, 2400);
    sleep_ms(5000);

    // Move para 90° e espera 5 segundos
    set_pwm_duty(SERVO_GPIO, 1470);
    sleep_ms(5000);

    // Move para 0° e espera 5 segundos
    set_pwm_duty(SERVO_GPIO, 500);
    sleep_ms(5000);

    // Movimento suave entre 0° e 180° continuamente
    while (1)
    {
        pwm_set_irq_enabled(sliceNum, true); // habilita a interrupção

        for (int pos = 500; pos <= 2400; pos += 5)
        { // De 0° para 180°
            set_pwm_duty(SERVO_GPIO, pos);
            sleep_ms(10);
        }
        for (int pos = 2400; pos >= 500; pos -= 5)
        { // De 180° para 0°
            set_pwm_duty(SERVO_GPIO, pos);
            sleep_ms(10);
            pwm_set_irq_enabled(sliceNum, false);
        }
    }

    return 0;
}
