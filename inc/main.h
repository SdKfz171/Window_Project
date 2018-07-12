
void _init_us();
void _init_ms();
void _stop_timer();
void Delay_ms(uint32_t mSecs);
void Delay_us(uint32_t uSecs);
void blink_gpio_task(void *pvParameters);
void Door_State_Check(void *pvParameters);
void DHT11_Received(void *pvParameters);
int Motor_Direction_Check(int value, int dest);
void Motor_Move(int direction);
void Step_Move(int step);