#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"

#define led1 2

uint8_t led_level = 0;

static const char *tag = "Main";

//Manejador del timer - variable global
TimerHandle_t xTimers;

//Ticks = periodos de la CPU
int interval = 100;
int timerId = 1;

//Creacion de funciones
esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);   


//Se declara arriba del main porque es una funcion ya existente dentro de ESP-IDF
void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(tag,"Event was called from timer");
    blink_led();
}



void app_main(void)
{
    //Llamado de funciones
    init_led();
    set_timer();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);                                                   //Resetea el pin
    gpio_set_direction(led1, GPIO_MODE_OUTPUT);        //Define al gpio led1 como salida
    return ESP_OK;                                                            // Buena practica de programacion, retornando un OK
}

esp_err_t blink_led(void)
{
    led_level = !led_level;                     //Va cambiando el valor actual a led_level
    gpio_set_level (led1, led_level);     //Setea el valor que tiene led_level a LED1
    return ESP_OK;                              //Buena practica de programacion en ESP-IDF, retornando un OK
}

esp_err_t set_timer(void)
{
    ESP_LOGI(tag,"Timer init configuration");

    //Declaration of one timer
    xTimers = xTimerCreate( "Timer",                                        //Just a text name, not used by the kernel.
                                            (pdMS_TO_TICKS(interval)),          //The timer period in ticks. (pasamos de ms a ticks)
                                            pdTRUE,                                         //The timers will auto-reload themselves when they expire.
                                            (void *) timerId,                              //Assign each timer a unique id equal to its array index.
                                            vTimerCallback                              //Each timer calls the same callback when it expires.
    );

    if(xTimers == NULL)
    {
        // The timer was not created.
        ESP_LOGE(tag, "The timer was not created");
    }
    else
    {
        if(xTimerStart(xTimers,0) != pdPASS)
        {
                //The timer could not be set into the Active state.
                ESP_LOGE(tag, "The timer could not be set into the Active state");
        }
    }
    return ESP_OK;
}