
#define SYSCLK 72000000
#define PRESCALER 72
#define Fmax (SYSCLK/PRESCALER) //максимальная частота
#define NPER 0xFFFF//полный счётчик таймера
//#define  PORT_MOTO    GPIOB
//#define  X_PIN_STEP    GPIO_Pin_9
//#define  X_PIN_DIR    GPIO_Pin_7
//#define  X_PIN_EN    GPIO_Pin_6

	  _Bool  DR; //переменные направления и запуска
	  _Bool  EN;
	  _Bool  S;
u16 STEP;
u16 PER; //значения по умолчанию
	float Line;//путь по оси

 typedef struct
 	{
	 u8 STEP; //длительность ипульса величина счётчика
	 //
	 int OB_P; //кофициент об/пин
	 u16 AX_P;//ускорение прирощение счётчика на степ
	 u16 V_PER_MIN;//скорость мах, минимальный период таймера

	 _Bool  S_PAR;// STEP акливное значениепринимает 1 или 0
	 _Bool  D_PAR;//DIR  акливное значениепринимает 1 или 0
	 _Bool  E_PAR;//ENABLED  акливное значениепринимает 1 или 0

 	} AXS;// обзываем структуру

 	AXS X; //создаём обьёкт преременных из структур
 //	AXS Y;
 //	AXS Z;
 //	AXS A;


 	void INI_STEP_MOTO();
 	void INI_MOTO_GPIO();
 	void INI_MOTO_TIM();
 	 void StartMotor();
 	 void StoptMotor();



