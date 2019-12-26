#define GPIO_STATE_CLOSED 0
#define GPIO_STATE_OPENED 1
#define GPIO_OUT_LOW 0
#define GPIO_OUT_HIGH 1

#define GPIO_SLEEP 100000

void event_boot();
int eventDetected();
int insideEvent();
int eventEnded();
int readExternal();
int writeOutput(int);

char *script_boot  = "/usr/bin/gpio-boot.sh &";
char *script_start = "/usr/bin/gpio-start.sh &";
char *script_end = "";
unsigned int pin_config = 0;
unsigned int out_pin = 0;

int DEBUG = 0;
int DRY_RUN = 0;

