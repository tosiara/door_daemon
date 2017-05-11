const int GPIO_STATE_CLOSED = 0;
const int GPIO_STATE_OPENED = 1;

void event_boot();
int eventDetected();
int insideEvent();
int eventEnded();

char *script_boot  = "/usr/bin/gpio-boot.sh &";
char *script_start = "/usr/bin/gpio-start.sh &";
char *script_end = "";
unsigned int pin_config = 0;

int DEBUG = 0;
