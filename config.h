#ifndef TUT_CONFIG_H
#define TUT_CONFIG_H

typedef struct Config_s {
    double volume;
    double max_volume;
    double sample_rate;
    char  *output_device;
} Config;

#endif
