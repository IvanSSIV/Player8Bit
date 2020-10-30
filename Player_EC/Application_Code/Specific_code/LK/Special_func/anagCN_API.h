enum anagCN_state
{
  ST_ANAGCN_INIT = 0,
  ST_ANAGCN_CHECK_LAN,
  ST_ANAGCN_DELETE_FILE,
  ST_ANAGCN_SEND_REQUEST,
  ST_ANAGCN_READ_LINE,
  ST_ANAGCN_DECODE_LINE,
  ST_ANAGCN_READ_LAST_LINE,
  ST_ANAGCN_DONE,
  ST_ANAGCN_ERROR,
  ST_ANAGCN_FAIL
};

struct user_data {
  char nome[21];
  char cognome[21];
  char badge[11];
};

typedef struct user_data USER_DATA;