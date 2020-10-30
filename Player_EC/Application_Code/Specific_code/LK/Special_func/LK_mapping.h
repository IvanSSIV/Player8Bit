struct map_data {
  uint8_t sportello_hardware[20];    // Singole unità di sportello che conterranno il numero di sportello che
                                     // rappresentano.
};

typedef struct map_data MAP_DATA;

enum lk_mapping_state {
  ST_MAPPING_INIT = 0,
  ST_MAPPING_CHECK_LAN,
  ST_MAPPING_GET_FROM_LAN,
  ST_MAPPING_GET_FROM_SD,
  ST_MAPPING_DONE,
  ST_MAPPING_ERROR,
  ST_MAPPING_FAIL
};