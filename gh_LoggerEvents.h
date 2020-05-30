// -----------------------------------------------------------------------
// Определение событий логгера
//				С целью сокращения использования оперативной памяти программой логгер пишет журналы в кодированном виде. 
//				Перевод журнала в текстовую форму оcуществляется внешней программой GHLogDecode
//        тексты берутся из комментариев в конце строки напротив каждого кода в данном файле
//
// -----------------------------------------------------------------------
// Журнал активности (запись через RecordActivity)
//        Формат журнала CSV
//        Date         Time      Device_ID   Event_ID   SubEvent_ID   #ARG1   #ARG2
//        YYYY-MM-DD   hh:mm:ss  %d          %d         %d            %d        %d
//
// -----------------------------------------------------------------------
// Журнал наблюдений (датчиков) (запись через RecordSensors)
// -----------------------------------------------------------------------
//        Формат журнала CSV
//        TEarth   TAir   TBoard  TOut  TEarth2   T_TAFanIn   T_TAFanOut  
//        %f       %f     %f      %f    %f        %f          %f


// -----------------------------------------------------------------------
// Коды устройств
// -----------------------------------------------------------------------
#define DEV_BOARD               0     // МикроКонтроллер
#define DEV_FAN                 1     // Вентилятор
#define DEV_SIDE_WINDOW         2     // Боковое окно
#define DEV_DOOR_WINDOW         3     // Торцевое окно
#define DEV_BARREL_VALVE        4     // Клапан бочки 
#define DEV_WATERING_LINE1      5     // Клапан грядки 1
#define DEV_WATERING_LINE2      6     // Клапан крядки 2

// -----------------------------------------------------------------------
// Коды событий МикроКонтроллера
// -----------------------------------------------------------------------
#define EVT_BOARD_ON                0x00      // Start operation
#define S_EVT_BOARD_ON_JUSTON       0x01      // Start operation
#define EVT_BOARD_SD                0x08      // SDCard
#define S_EVT_BOARD_SD_REINIT       0x09    // Успешный перезапуск SDCard
#define S_EVT_BOARD_SD_ERRFILESIZE  0x0A    // ОШИБКА! Файл журнала не растет. Требуется перезапуск SDCard
#define S_EVT_BOARD_SD_INITFAIL     0x0B    // ОШИБКА! Инициализация SDCard не успешна
#define S_EVT_BOARD_SD_ERRFILEOPEN  0x0C    // ОШИБКА! Файл не может быть открыт     

// -----------------------------------------------------------------------
// Коды событий Земляного Аккумулятора (Вентилятора)
// -----------------------------------------------------------------------
#define EVT_FAN_OFF               0x10      // Вентилятор ВЫКЛ.
#define S_EVT_FAN_OFF_SMALLDT     0x11      // Разница темп-р мала. Вентилятор ВЫКЛ. 
#define S_EVT_FAN_OFF_SUNHIDE     0x12      // Солнце скрылось. Вентилятор ВЫКЛ.
#define S_EVT_FAN_OFF_HOTNIGHT    0x13      // Ночь, не холодно. Вентилятор ВЫКЛ.
#define S_EVT_FAN_OFF_JUSTOFF     0x14      // Вентилятор ВЫКЛ.

#define EVT_FAN_ON                0x20      // Вентилятор ВКЛ.
#define S_EVT_FAN_ON_HOTDAY       0x21      // Солнце жарит. Вентилятор ВКЛ.
#define S_EVT_FAN_ON_COLDNIGHT    0x22      // Ночь, холодно, греемся. Вентилятор ВКЛ.
#define S_EVT_FAN_ON_JUSTON       0x23      // Вентилятор ВКЛ.

#define EVT_FAN_SETMODE           0x30      // Установка режима работы вентилятора
#define S_EVT_FAN_SETMODE_AUTO    0x31      // Переход в автоматический режим управления вентилятором
#define S_EVT_FAN_SETMODE_MANUAL  0x32      // Переход в ручной режим управления вентилятором

// -----------------------------------------------------------------------
// Коды событий Бокового Окна
// -----------------------------------------------------------------------
#define EVT_SW_STOP                   0x40  // Остановка мотора
#define S_EVT_SW_STOP_HALT            0x41  // Экстренная остановка         
#define S_EVT_SW_STOP_CLOSEDBYLS      0x42  // Окно закрылось по концевику
#define S_EVT_SW_STOP_OPENEDBYLS      0x43  // Окно открылось по концевику      
#define S_EVT_SW_STOP_CLOSEDBYTIMER   0x44  // Окно закрылось по таймеру
#define S_EVT_SW_STOP_OPENEDBYTIMER   0x45  // Окно открылось по таймеру

#define EVT_SW_START                  0x50      // Запуск мотора
#define S_EVT_SW_START_TOCLOSE        0x51      // Форточка закрывается
#define S_EVT_SW_START_TOOPEN         0x52      // Форточка открывается
#define S_EVT_SW_START_CLOSEBYTEMP    0x53      // Закрываем окно, т.к. достигнута температура закрытия
#define S_EVT_SW_START_OPENBYTEMP     0x54      // Открываем окно, т.к. достигнута температура открытия
#define S_EVT_SW_START_CLOSEFORNIGHT  0x55      // Закрываем окно, т.к. ночь на дворе

#define EVT_SW_SETMODE                0x60      // Установка режима работы окна
#define S_EVT_SW_SETMODE_AUTO         0x61      // Переход в автоматический режим управления окном
#define S_EVT_SW_SETMODE_MANUAL       0x62      // Переход в ручной режим управления окном

#define EVT_SW_ALARM                  0x70      // Режим тревоги окна
#define S_EVT_SW_ALARM_ON             0x71      // Установка флага тревоги окна
#define S_EVT_SW_ALARM_OFF            0x72      // Сброс флага тревоги окна
