#include "platform/espidf_runtime.h"

#include "app.h"
#include "drivers/twai_driver.h"
#include "espnow/signals.h"
#include "esp_sleep.h"
#include "driver/gpio.h"

static constexpr unsigned long SLEEP_TIMEOUT_MS = 30000;
static constexpr uint64_t SLEEP_WAKEUP_US = 60ULL * 1000000ULL;

static void enterDeepSleep()
{
    Serial.println("[SLEEP] Entering deep sleep, timer+GPIO wake");
    Serial.flush();
    // 定时器唤醒（兜底）+ CAN RX 引脚下降沿唤醒（总线活动时恢复）
    esp_sleep_enable_timer_wakeup(SLEEP_WAKEUP_US);
    esp_sleep_enable_ext0_wakeup(TWAI_RX_PIN, 0);
    esp_deep_sleep_start();
}

static void app_main_setup()
{
    gpio_num_t twaiTx = TWAI_TX_PIN;
    gpio_num_t twaiRx = TWAI_RX_PIN;
    {
        Preferences canPrefs;
        if (canPrefs.begin("can", false))
        {
            int8_t tx = canPrefs.getChar("tx", -1);
            int8_t rx = canPrefs.getChar("rx", -1);
            canPrefs.end();
            if (tx >= 0 && tx <= 39)
                twaiTx = (gpio_num_t)tx;
            if (rx >= 0 && rx <= 39)
                twaiRx = (gpio_num_t)rx;
        }
    }
    appSetup<TWAIDriver>(std::make_unique<TWAIDriver>(twaiTx, twaiRx), "ESP32 TWAI ready @ 500k");
#ifdef ESP32_DASHBOARD
    {
        auto &ctx = AppContext::instance();
        mcpDashboardSetup(ctx.handler.get(), ctx.driver.get());
    }
#endif
}

static void app_main_loop()
{
    appLoop<TWAIDriver>();
#ifdef ESP32_DASHBOARD
    mcpDashboardLoop();
#endif

    // Deep sleep: UI_powerOff → immediate, else BMS condition + 30s timeout
    if (espnowCurData.vehicleLocked)
    {
        if (espnowCurData.uiPowerOff)
        {
            delay(2000);
            enterDeepSleep();
        }
        else if (lastCanSignalMs > 0 && millis() - lastCanSignalMs >= SLEEP_TIMEOUT_MS)
        {
            enterDeepSleep();
        }
    }
}

extern "C" void app_main(void)
{
    esp_err_t nvsErr = nvs_flash_init();
    if (nvsErr == ESP_ERR_NVS_NO_FREE_PAGES || nvsErr == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        nvsErr = nvs_flash_init();
    }
    ESP_ERROR_CHECK(nvsErr);

    app_main_setup();
    while (true)
    {
        app_main_loop();
        yield();
    }
}
