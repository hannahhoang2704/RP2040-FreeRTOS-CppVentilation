#ifndef FREERTOS_GREENHOUSE_RTOS_INFRASTRUCTURE_H
#define FREERTOS_GREENHOUSE_RTOS_INFRASTRUCTURE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
// RTOS task notifications require breathing room
// OLED gets crazy if events are sent too frequently -- i.e. with Rotor
static const uint64_t TASK_NOTIFICATION_RATE_LIMIT_US{35000};
const int16_t CO2_MAX{1500};
const int16_t CO2_MIN{0};
const char INIT_CHAR{'.'};
const char MAX_STRING_LEN{64};

enum program_state {
    STATUS,
    NETWORK
};

enum network_phase {
    NEW_API,
    NEW_SSID,
    NEW_PW
};

enum connection_state {
    bCONNECTING,
    bCONNECTED,
    bNOT_CONNECTED
};

enum storage_data {
    CO2_target,
    API_str,
    SSID_str,
    PW_str
};

enum thing_speak_event{
    bSEND = 0x01,
    bRECEIVE = 0x02,
    bRECONNECT = 0x04
};

struct RTOS_infrastructure {
    QueueHandle_t qState{nullptr};
    QueueHandle_t qNetworkPhase{nullptr};
    QueueHandle_t qCO2TargetPending{nullptr};
    QueueHandle_t qCO2TargetCurrent{nullptr};
    QueueHandle_t qCO2Measurement{nullptr};
    QueueHandle_t qPressure{nullptr};
    QueueHandle_t qFan{nullptr};
    QueueHandle_t qHumidity{nullptr};
    QueueHandle_t qTemperature{nullptr};
    QueueHandle_t qCharPending{nullptr};
    QueueHandle_t qConnectionState{nullptr};
    QueueHandle_t qNetworkStrings[3] {[NEW_API] = nullptr,
                                      [NEW_SSID] = nullptr,
                                      [NEW_PW] = nullptr};
    QueueHandle_t qStorageQueue{nullptr};

    SemaphoreHandle_t sUpdateGreenhouse{nullptr};
    SemaphoreHandle_t sUpdateDisplay{nullptr};
    EventGroupHandle_t xThingSpeakEvent;
};

#endif //FREERTOS_GREENHOUSE_RTOS_INFRASTRUCTURE_H
