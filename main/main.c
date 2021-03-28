/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"

#define TAG "MAIN"
void init_NVS()
{
    //vTaskDelay(pdMS_TO_TICKS(10000));
    printf("iniciando...");
    esp_err_t ret = nvs_flash_init();
    /*
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGE(TAG, "%s, line %d, %s", __func__, __LINE__, esp_err_to_name(ret));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
*/
    ESP_ERROR_CHECK(ret);
}
esp_err_t nvs_get_string(char *str, char *namespace, char *key)
{
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "%s, line %d, %s", __func__, __LINE__, esp_err_to_name(err));
        return err;
    }

    size_t size = 0;
    err = nvs_get_blob(handle, key, NULL, &size);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "%s, line %d, %s", __func__, __LINE__, esp_err_to_name(err));
        return err;
    }
    if (size <= 0)
    {
        ESP_LOGE(TAG, "%s, line %d, %s", __func__, __LINE__, esp_err_to_name(err));
        return err;
    }

    err = nvs_get_blob(handle, key, (void *)str, &size);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "%s, line %d, %s", __func__, __LINE__, esp_err_to_name(err));
        return err;
    }

    nvs_close(handle);
    return ESP_OK;
}
esp_err_t nvs_set_string(char *str, char *namespace, char *key)
{
    nvs_handle handle;
    esp_err_t err;
    err = nvs_open(namespace, NVS_READWRITE, &handle);
    if (err != ESP_OK)
        return err;

    err = nvs_set_blob(handle, key, (void *)str, strlen(str) + 1);

    if (err != ESP_OK)
        return err;

    err = nvs_commit(handle);
    if (err != ESP_OK)
        return err;

    nvs_close(handle);

    return ESP_OK;
}
void app_main(void)
{
    init_NVS();

    char mistring[20] = {0};

    // STRINGS BEEN READED FROM NVS BUT WITH NO SUCCESS
    nvs_get_string(mistring, "main", "ssid");
    printf("ssid:%s\n", mistring);

    nvs_get_string(mistring, "main", "pass");
    printf("pass:%s\n", mistring);

    // PROOF THAT NVS IS WORKING OK
    nvs_set_string("string_saved", "main", "set");
    nvs_get_string(mistring, "main", "set");
    printf("mistring:%s\n", mistring);

    // STARTS HELLO WORDL EXAMPLE FROM IDF FRAMEWORK
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--)
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
