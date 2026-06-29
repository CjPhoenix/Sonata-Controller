#include "SD_card.h"

#define CONFIG_BUFFER_SIZE  1024
#define LINE_READ_BUFFER    128

#define CONFIG_FILE         "/.cfg"

typedef struct {
    uint8_t hue_l;
    uint8_t sat_l;
    uint8_t val_l;

    uint8_t hue_r;
    uint8_t sat_r;
    uint8_t val_r;

    int is_lighting_on;
} Config;
static Config GLOBAL_CONFIG;

static char* CONFIG_BUFFER = (char*) malloc(CONFIG_BUFFER_SIZE);

typedef struct {
    char name[64];
    char value[64];
} Config_Var;

void write_config_to_file();
void update_config_from_file();
void update_config_from_str(String);
void update_config_from_buffer();

/**
 * Returns 1 if the persistent config could not be connected
 */
int config_init()
{
    return sd_card_init();
}

void export_global_config_to_string(char** result)
{
    snprintf(*result, CONFIG_BUFFER_SIZE, 
        "%d %d %d %d %d %d %d\r", 
        GLOBAL_CONFIG.hue_l,
        GLOBAL_CONFIG.sat_l,
        GLOBAL_CONFIG.val_l,
        GLOBAL_CONFIG.hue_r,
        GLOBAL_CONFIG.sat_r,
        GLOBAL_CONFIG.val_r,
        GLOBAL_CONFIG.is_lighting_on
    );
}

void write_config_to_file()
{
    export_global_config_to_string(&CONFIG_BUFFER);

    delete_file(CONFIG_FILE);
    write_to_file(CONFIG_FILE, CONFIG_BUFFER);
}

void update_config_from_file()
{
    String file_contents = read_file(CONFIG_FILE);
    file_contents.toCharArray(CONFIG_BUFFER, CONFIG_BUFFER_SIZE);
    
    update_config_from_buffer();
}

void update_config_from_str(String text)
{
    text.toCharArray(CONFIG_BUFFER, CONFIG_BUFFER_SIZE);

    update_config_from_buffer();
}

void update_config_from_buffer()
{
    sscanf(CONFIG_BUFFER, 
        "%d %d %d %d %d %d %d\r", 
        &GLOBAL_CONFIG.hue_l,
        &GLOBAL_CONFIG.sat_l,
        &GLOBAL_CONFIG.val_l,
        &GLOBAL_CONFIG.hue_r,
        &GLOBAL_CONFIG.sat_r,
        &GLOBAL_CONFIG.val_r,
        &GLOBAL_CONFIG.is_lighting_on
    );
}