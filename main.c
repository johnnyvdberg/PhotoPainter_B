#include "EPD_Test.h" // Examples
#include "run_File.h"

#include "led.h"
#include "waveshare_PCF85063.h" // RTC
#include "DEV_Config.h"
#include "duration.h"
#include "pico/rand.h"
#include "fastrange.h"

#include <time.h>

extern const char *fileList;
extern char pathName[];

#define enChargingRtc 0

/*
Mode 0: Automatically get pic folder names and sort them
Mode 1: Automatically get pic folder names but not sorted
Mode 2: pic folder name is not automatically obtained, users need to create fileList.txt file and write the picture name in TF card by themselves
Mode 3: Pick completely random picture, no limits
*/
int Mode = 0;

void run_display(Time_data Time, Time_data alarmTime, char hasCard);
void run_display_mode_3(Time_data Time, Time_data alarmTime, char hasCard);
void setConfiguration(Time_data *Time, Time_data *alarmTime);

float measureVBAT(void)
{
    float Voltage = 0.0;
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    Voltage = result * conversion_factor * 3;
    printf("Raw value: 0x%03x, voltage: %f V\n", result, Voltage);
    return Voltage;
}

void chargeState_callback()
{
    if (DEV_Digital_Read(VBUS))
    {
        if (!DEV_Digital_Read(CHARGE_STATE))
        { // is charging
            ledCharging();
        }
        else
        { // charge complete
            ledCharged();
        }
    }
}

void run_display(Time_data Time, Time_data alarmTime, char hasCard)
{
    if (Mode == 3)
    {
        run_display_mode_3(Time, alarmTime, hasCard);
        return;
    }

    if (hasCard)
    {
        setFilePath();
        EPD_7in3e_display_BMP(pathName, measureVBAT(), true); // display bmp
    }
    else
    {
        printf("led_ON_PWR...\r\n");
        EPD_7in3e_display(measureVBAT());
    }
    DEV_Delay_ms(100);
    PCF85063_clear_alarm_flag(); // clear RTC alarm flag
#if enChargingRtc
    rtcRunAlarm(Time, alarmTime); // RTC run alarm
#endif
}

void run_display_mode_3(Time_data Time, Time_data alarmTime, char hasCard)
{
    char filePath[2048];
    if (hasCard)
    {
        selectRandomFile(filePath, sizeof(filePath));
        EPD_7in3e_display_BMP(filePath, measureVBAT(), false);
    }
    else
    {
        printf("led_ON_PWR...\r\n");
        EPD_7in3e_display(measureVBAT());
    }
    DEV_Delay_ms(100);
    PCF85063_clear_alarm_flag(); // clear RTC alarm flag
#if enChargingRtc
    rtcRunAlarm(Time, alarmTime); // RTC run alarm
#endif
}

int main(void)
{
    printf("Init...\r\n");

    if (DEV_Module_Init() != 0)
    { // DEV init
        return -1;
    }

    Time_data Time = {2024 - 2000, 3, 31, 0, 0, 0};
    Time_data alarmTime = Time;
    setConfiguration(&Time, &alarmTime);

    watchdog_enable(8 * 1000, 1); // 8s
    DEV_Delay_ms(100);
    PCF85063_init(); // RTC init
    
    rtcRunAlarm(Time, alarmTime);  // RTC run alarm
    gpio_set_irq_enabled_with_callback(CHARGE_STATE, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, chargeState_callback);

    char isCard = 0;

    if (measureVBAT() < 3.1)
    { // battery power is low
        printf("low power ...\r\n");
        PCF85063_alarm_Time_Disable();
        ledLowPower(); // LED flash for Low power
        powerOff();    // BAT off
        return 0;
    }
    else
    {
        printf("work ...\r\n");
        ledPowerOn();
    }

    if (!sdTest())
    {
        isCard = 1;
        if (Mode == 0)
        {
            printf("Mode 0\r\n");
            sdScanDir();
            file_sort();
        }
        if (Mode == 1)
        {
            printf("Mode 1\r\n");
            sdScanDir();
        }
        if (Mode == 2)
        {
            printf("Mode 2\r\n");
            file_cat();
        }
        if (Mode == 3)
        {
            printf("Mode 3\r\n");
        }
    }
    else
    {
        isCard = 0;
    }

    if (!DEV_Digital_Read(VBUS))
    { // no charge state
        run_display(Time, alarmTime, isCard);
    }
    else
    { // charge state
        chargeState_callback();
        while (DEV_Digital_Read(VBUS))
        {
            measureVBAT();
#if enChargingRtc
            if (!DEV_Digital_Read(RTC_INT))
            { // RTC interrupt trigger
                printf("rtc interrupt\r\n");
                setConfiguration(&Time, &alarmTime);
                run_display(Time, alarmTime, isCard);
            }
#endif

            if (!DEV_Digital_Read(BAT_STATE))
            { // KEY pressed
                printf("key interrupt\r\n");
                run_display(Time, alarmTime, isCard);
            }
            DEV_Delay_ms(200);
        }
    }

    printf("power off ...\r\n");
    powerOff(); // BAT off

    return 0;
}

void setConfiguration(Time_data *Time, Time_data *alarmTime)
{
    Config config;
    int status = readConfig("0:/config.txt", &config);
    if (status == 0)
    {
        printf("Using config as defined in config.txt\r\n");
        Mode = config.mode;
        int64_t randomTime = fastrange32(get_rand_32(), diff_durations(config.min_refresh, config.max_refresh)) + config.min_refresh;
        printf("Waking up after: %lld seconds because this is between %d and %d\n", randomTime, config.min_refresh, config.max_refresh);
        struct tm alarm_tm = to_tm(*alarmTime);
        alarm_tm = add_duration_to_time(alarm_tm, randomTime);
        *alarmTime = from_tm(alarm_tm);
        printf("Alarm Time: %04u-%02u-%02u %02u:%02u:%02u\n", alarmTime->years, alarmTime->months, alarmTime->days, alarmTime->hours, alarmTime->minutes, alarmTime->seconds);
    }
    else
    {
        printf("Not using config as defined in config.txt. Error: %d\r\n", status);
        alarmTime->hours += 1;
        Mode = 3;
    }
}