#include "Greenhouse.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>

using namespace std;

Greenhouse::Greenhouse(const shared_ptr<ModbusClient> &modbus_client, const shared_ptr<PicoI2C>&pressure_sensor_I2C) :
        mCO2(modbus_client),
        mHumidity(modbus_client),
        mTemperature(modbus_client),
        mPressure(pressure_sensor_I2C),
        mMIO12_V(modbus_client, 1, 0) {
    if (xTaskCreate(task_automate_greenhouse,
                    "GREENHOUSE",
                    512,
                    (void *) this,
                    tskIDLE_PRIORITY + 3,
                    &mTaskHandle) == pdPASS) {
        Logger::log("Created GREENHOUSE task.\n");
    } else {
        Logger::log("Failed to create GREENHOUSE task.\n");
    }
}

void Greenhouse::task_automate_greenhouse(void *params) {
    auto object_ptr = static_cast<Greenhouse *>(params);
    object_ptr->automate_greenhouse();
}

void Greenhouse::automate_greenhouse() {
    Logger::log("Initiated GREENHOUSE task\n");
    mMIO12_V.write(300);
    mMIO12_V.write(300);
    Logger::log("Initiated GREENHOUSE task\n");
    mMIO12_V.write(100);
    mMIO12_V.write(100);
    stringstream ss;
    while (true) {
        Logger::log("Pressure Sensor is %d\n", 40);
        vTaskDelay(1);
        Logger::log("Test Float val: %.2f\n", 25.65);
        vTaskDelay(1);
        Logger::log("CO2 is %.1f ppm\n", mCO2.update());
        vTaskDelay(1);
        Logger::log("Temp from GMP252 is %.1f C\n", mTemperature.update_GMP252());
        vTaskDelay(1);
        Logger::log("Humidity is %.1f %%\n", mHumidity.update());
        vTaskDelay(1);
        Logger::log("Temp from HMP60 is %.1f C\n", mTemperature.update_HMP60());


        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
