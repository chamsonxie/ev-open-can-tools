#pragma once

#include "../can_frame_types.h"
#include "can_driver.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"
#include <driver/twai.h>
#pragma GCC diagnostic pop
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <cassert>
#include <string.h>

class TWAIDriver : public CanDriver
{
public:
    static constexpr bool kSupportsISR = false;

    TWAIDriver(gpio_num_t txPin, gpio_num_t rxPin)
        : txPin_(txPin), rxPin_(rxPin) {}

    bool init() override
    {
        if (!mutex_)
            mutex_ = xSemaphoreCreateMutex();
        if (!mutex_)
            return false;

        g_config_ = TWAI_GENERAL_CONFIG_DEFAULT(txPin_, rxPin_, TWAI_MODE_LISTEN_ONLY);
        g_config_.rx_queue_len = 32;
        g_config_.tx_queue_len = 1;

        t_config_ = TWAI_TIMING_CONFIG_500KBITS();
        f_config_ = TWAI_FILTER_CONFIG_ACCEPT_ALL();

        lock();
        driverOK_ = installAndStartLocked();
        unlock();
        return driverOK_;
    }

    void setFilters(const uint32_t *ids, uint8_t count) override
    {
        lock();
        if (count == 0)
        {
            // 放开过滤：嗅探器/全总线模式，接收所有帧
            f_config_ = TWAI_FILTER_CONFIG_ACCEPT_ALL();
            exactFilterCount_ = 0;
            stopAndUninstallLocked();
            driverOK_ = installAndStartLocked();
            unlock();
            return;
        }

        uint32_t differ = 0;
        for (uint8_t i = 1; i < count; i++)
        {
            differ |= ids[0] ^ ids[i];
        }

        uint32_t base = ids[0] & ~differ;
        twai_filter_config_t nextFilter = f_config_;
        nextFilter.acceptance_code = base << 21;
        nextFilter.acceptance_mask = (differ << 21) | 0x001FFFFF;
        nextFilter.single_filter = true;

        // TWAI 只有掩码过滤器；稀疏 ID 集可能通过误报。
        exactFilterCount_ = (count < kMaxExactFilters) ? count : kMaxExactFilters;
        for (uint8_t i = 0; i < exactFilterCount_; i++)
            exactFilterIds_[i] = ids[i];
        f_config_ = nextFilter;
        stopAndUninstallLocked();
        driverOK_ = installAndStartLocked();
        unlock();
    }

    bool enableInterrupt(void (* /*onReady*/)()) override { return false; }

    bool read(CanFrame &frame) override
    {
        for (uint8_t attempt = 0; attempt < kReadDrainBudget; attempt++)
        {
            lock();
            if (!driverOK_)
            {
                tryRecoverLocked();
                unlock();
                return false;
            }

            twai_message_t msg;
            esp_err_t rxErr = twai_receive(&msg, 0);
            if (rxErr != ESP_OK)
            {
                lastReceiveErr_ = rxErr;
                if (rxErr != ESP_ERR_TIMEOUT)
                    receiveErrors_++;
                if (isBusOffLocked())
                    recoverWithCooldownLocked();
                unlock();
                return false;
            }
            bool accepted = exactFilterMatchesLocked(msg.identifier);
            if (!accepted)
                rejectedFrames_++;
            unlock();

            if (!accepted)
                continue;

            frame.id = msg.identifier;
            frame.dlc = (msg.data_length_code <= 8) ? msg.data_length_code : 8;
            memset(frame.data, 0, 8);
            memcpy(frame.data, msg.data, frame.dlc);
            return true;
        }

        return false;
    }

    void diagnosticsJson(char *out, size_t outLen) const override
    {
        if (!out || outLen == 0)
            return;

        twai_status_info_t status = {};
        bool hasStatus = driverInstalled_ && twai_get_status_info(&status) == ESP_OK;
        const twai_status_info_t &s = hasStatus ? status : lastStatus_;
        snprintf(out, outLen,
                 "{\"type\":\"twai\",\"txPin\":%d,\"rxPin\":%d,\"ok\":%s,\"installed\":%s,\"state\":\"%s\",\"msgsToRx\":%u,\"rxErrCounter\":%u,\"rxMissed\":%u,\"rxOverrun\":%u,\"arbLost\":%u,\"busErrors\":%u,\"recoveries\":%u,\"rxErrors\":%u,\"rejected\":%u,\"lastInstallErr\":%d,\"lastStartErr\":%d,\"lastRxErr\":%d}",
                 static_cast<int>(txPin_), static_cast<int>(rxPin_), driverOK_ ? "true" : "false",
                 driverInstalled_ ? "true" : "false", twaiStateName(s.state),
                 static_cast<unsigned int>(s.msgs_to_rx),
                 static_cast<unsigned int>(s.rx_error_counter),
                 static_cast<unsigned int>(s.rx_missed_count),
                 static_cast<unsigned int>(s.rx_overrun_count), static_cast<unsigned int>(s.arb_lost_count),
                 static_cast<unsigned int>(s.bus_error_count), static_cast<unsigned int>(recoveries_),
                 static_cast<unsigned int>(receiveErrors_),
                 static_cast<unsigned int>(rejectedFrames_), static_cast<int>(lastInstallErr_),
                 static_cast<int>(lastStartErr_), static_cast<int>(lastReceiveErr_));
    }

    void diagnosticsSummary(char *out, size_t outLen) const override
    {
        if (!out || outLen == 0)
            return;

        twai_status_info_t status = {};
        bool hasStatus = driverInstalled_ && twai_get_status_info(&status) == ESP_OK;
        const twai_status_info_t &s = hasStatus ? status : lastStatus_;
        snprintf(out, outLen,
                 "TWAI tx=%d rx=%d ok=%s installed=%s state=%s msgsToRx=%u rxErrCounter=%u rxMissed=%u rxOverrun=%u arbLost=%u busErrors=%u recoveries=%u rxErrors=%u rejected=%u lastInstallErr=%d lastStartErr=%d lastRxErr=%d",
                 static_cast<int>(txPin_), static_cast<int>(rxPin_), driverOK_ ? "yes" : "no",
                 driverInstalled_ ? "yes" : "no", twaiStateName(s.state),
                 static_cast<unsigned int>(s.msgs_to_rx),
                 static_cast<unsigned int>(s.rx_error_counter),
                 static_cast<unsigned int>(s.rx_missed_count),
                 static_cast<unsigned int>(s.rx_overrun_count), static_cast<unsigned int>(s.arb_lost_count),
                 static_cast<unsigned int>(s.bus_error_count), static_cast<unsigned int>(recoveries_),
                 static_cast<unsigned int>(receiveErrors_),
                 static_cast<unsigned int>(rejectedFrames_), static_cast<int>(lastInstallErr_),
                 static_cast<int>(lastStartErr_), static_cast<int>(lastReceiveErr_));
    }

private:
    static constexpr uint8_t kMaxExactFilters = 32;
    static constexpr uint8_t kReadDrainBudget = 8;
    static constexpr uint32_t BUSOFF_COOLDOWN_MS = 1000;

    static const char *twaiStateName(twai_state_t state)
    {
        switch (state)
        {
        case TWAI_STATE_STOPPED:
            return "stopped";
        case TWAI_STATE_RUNNING:
            return "running";
        case TWAI_STATE_BUS_OFF:
            return "bus_off";
        case TWAI_STATE_RECOVERING:
            return "recovering";
        default:
            return "unknown";
        }
    }

    bool exactFilterMatchesLocked(uint32_t id) const
    {
        if (exactFilterCount_ == 0)
            return true;
        for (uint8_t i = 0; i < exactFilterCount_; i++)
        {
            if (exactFilterIds_[i] == id)
                return true;
        }
        return false;
    }

    bool isBusOffLocked()
    {
        if (!driverInstalled_)
            return false;
        twai_status_info_t status;
        if (twai_get_status_info(&status) != ESP_OK)
            return false;
        lastStatus_ = status;
        return status.state == TWAI_STATE_BUS_OFF;
    }

    void recoverWithCooldownLocked()
    {
        uint32_t now = millis();
        if (now - lastRecovery_ < BUSOFF_COOLDOWN_MS)
            return;
        lastRecovery_ = now;
        recoveries_++;

        stopAndUninstallLocked();
        driverOK_ = installAndStartLocked();
    }

    void tryRecoverLocked()
    {
        uint32_t now = millis();
        if (now - lastRecovery_ < BUSOFF_COOLDOWN_MS * 10)
            return;
        lastRecovery_ = now;
        recoveries_++;

        stopAndUninstallLocked();
        driverOK_ = installAndStartLocked();
    }

    void lock()
    {
        if (mutex_)
            xSemaphoreTake(mutex_, portMAX_DELAY);
    }

    void unlock()
    {
        if (mutex_)
            xSemaphoreGive(mutex_);
    }

    bool installAndStartLocked()
    {
        lastInstallErr_ = twai_driver_install(&g_config_, &t_config_, &f_config_);
        if (lastInstallErr_ != ESP_OK)
        {
            driverInstalled_ = false;
            return false;
        }
        driverInstalled_ = true;
        lastStartErr_ = twai_start();
        if (lastStartErr_ != ESP_OK)
        {
            twai_driver_uninstall();
            driverInstalled_ = false;
            return false;
        }
        lastInstallErr_ = ESP_OK;
        lastStartErr_ = ESP_OK;
        twai_get_status_info(&lastStatus_);
        return true;
    }

    void stopAndUninstallLocked()
    {
        if (!driverInstalled_)
            return;
        twai_get_status_info(&lastStatus_);
        twai_stop();
        twai_driver_uninstall();
        driverInstalled_ = false;
        driverOK_ = false;
    }

    gpio_num_t txPin_;
    gpio_num_t rxPin_;
    twai_general_config_t g_config_;
    twai_timing_config_t t_config_;
    twai_filter_config_t f_config_;
    SemaphoreHandle_t mutex_ = nullptr;
    bool driverInstalled_ = false;
    bool driverOK_ = false;
    uint32_t lastRecovery_ = 0;
    uint32_t exactFilterIds_[kMaxExactFilters] = {};
    uint8_t exactFilterCount_ = 0;
    twai_status_info_t lastStatus_ = {};
    esp_err_t lastInstallErr_ = ESP_OK;
    esp_err_t lastStartErr_ = ESP_OK;
    esp_err_t lastReceiveErr_ = ESP_OK;
    uint32_t receiveErrors_ = 0;
    uint32_t rejectedFrames_ = 0;
    uint32_t recoveries_ = 0;
};
