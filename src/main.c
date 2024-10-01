#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//*** 2 library บน สำคัญที่สุด ***
#include <driver/gpio.h>
#include <freertos/semphr.h>

SemaphoreHandle_t sem_print; // sem_print = resource guard

static void print(const char* job, const char* text) {
    printf("JOB: %s, TEXT: %s\r\n", job, text);
}

job1_task(void* pvParameter) {
    while(true) {
        if (xSemaphoreTake(sem_print, 1000)) {
            print("JOB1", "Hello Job1");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            xSemaphoreGive(sem_print);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static job2_task(void* pvParameter) {
    while(true) {
        if (xSemaphoreTake(sem_print, 0)) {
            print("JOB2", "Hello Job2");
            xSemaphoreGive(sem_print);
        } else {
            printf("JOB2 print failed\r\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main() {
    vTaskDelay(2000 / portTICK_PERIOD_MS); //เหตุผลของการ delay? เพื่อให้ log โปรแกรมตรงบรรทัดบนๆได้
    sem_print = xSemaphoreCreateMutex();
    xTaskCreate(job1_task, "job1_task", 1024*2, NULL, 1, NULL);
    xTaskCreate(job2_task, "task_led2", 1024*2, NULL, 1, NULL);
}