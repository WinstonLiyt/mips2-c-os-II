#include "includes.h"

#define BOTH_EMPTY (UART_LS_TEMT | UART_LS_THRE)

#define WAIT_FOR_XMITR                       \
    do                                       \
    {                                        \
        lsr = REG8(UART_BASE + UART_LS_REG); \
    } while ((lsr & BOTH_EMPTY) != BOTH_EMPTY)

#define WAIT_FOR_THRE                        \
    do                                       \
    {                                        \
        lsr = REG8(UART_BASE + UART_LS_REG); \
    } while ((lsr & UART_LS_THRE) != UART_LS_THRE)

#define TASK_STK_SIZE 512 // 256

OS_STK TaskStartStk[TASK_STK_SIZE];

char Info[100] = "  @Game: Number Guess\n  @Author: 2150260 ytLi\n  @Copyright: CS@Tongji\n";

void uart_init(void)
{
    INT32U divisor;

    /* Set baud rate */

    divisor = (INT32U)IN_CLK / (16 * UART_BAUD_RATE);

    REG8(UART_BASE + UART_LC_REG) = 0x80;
    REG8(UART_BASE + UART_DLB1_REG) = divisor & 0x000000ff;
    REG8(UART_BASE + UART_DLB2_REG) = (divisor >> 8) & 0x000000ff;
    REG8(UART_BASE + UART_LC_REG) = 0x00;

    /* Disable all interrupts */

    REG8(UART_BASE + UART_IE_REG) = 0x00;

    /* Set 8 bit char, 1 stop bit, no parity */

    REG8(UART_BASE + UART_LC_REG) = UART_LC_WLEN8 | (UART_LC_ONE_STOP | UART_LC_NO_PARITY);

    // uart_print_str("UART initialize done ! \n");
    return;
}

void uart_putc(char c)
{
    unsigned char lsr;
    WAIT_FOR_THRE;
    REG8(UART_BASE + UART_TH_REG) = c;
    if (c == '\n')
    {
        WAIT_FOR_THRE;
        REG8(UART_BASE + UART_TH_REG) = '\r';
    }
    WAIT_FOR_XMITR;
}

void uart_print_str(char *str)
{
    INT32U i = 0;
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL()

    while (str[i] != 0)
    {
        uart_putc(str[i]);
        i++;
    }

    OS_EXIT_CRITICAL()
}

void gpio_init()
{
    REG32(GPIO_BASE + GPIO_OE_REG) = 0xffffffff;
    REG32(GPIO_BASE + GPIO_INTE_REG) = 0x00000000;
    // gpio_out(0x0f0f0f0f);
    uart_print_str("GPIO initialize done ! \n");
    return;
}

void gpio_out(INT32U number)
{
    REG32(GPIO_BASE + GPIO_OUT_REG) = number;
}

INT32U gpio_in()
{
    INT32U temp = 0;

    temp = REG32(GPIO_BASE + GPIO_IN_REG);

    return temp;
}

INT32U uart_getc(void)
{
    unsigned char lsr;

    // Wait until data is available in the receiver
    do
    {
        lsr = REG8(UART_BASE + UART_LS_REG);
    } while ((lsr & 0x01) == 0); // Wait for Data Ready bit (0x01) to be set

    return (INT32U)REG8(UART_BASE + UART_TH_REG); // Read and return the received data
}

/*******************************************

    ÉèÖÃcompareŒÄŽæÆ÷£¬²¢ÇÒÊ¹ÄÜÊ±ÖÓÖÐ¶Ï

********************************************/
void OSInitTick(void)
{
    INT32U compare = (INT32U)(IN_CLK / OS_TICKS_PER_SEC);

    asm volatile("mtc0   %0,$9" : : "r"(0x0));
    asm volatile("mtc0   %0,$11" : : "r"(compare));
    asm volatile("mtc0   %0,$12" : : "r"(0x10000401));
    // uart_print_str("OSInitTick Done!!!\n");

    return;
}

void TaskStart(void *pdata)
{
    // INT32U count = 0;
    pdata = pdata; /* Prevent compiler warning                 */
    OSInitTick();  /* don't put this function in main()*/

    INT32U predefined_number = 5; // 预定义的数字
    char user_input[10];
    INT32U user_number = 0;
    uart_print_str(Info);

    uart_print_str("The game begins! \n");
    uart_print_str("Try to guess the number (0-99):\n");
    INT32U gamer = 0;
    for (;;)
    {
        INT32U data;
        data = gpio_in();
        INT32U ready = data << 31;
        INT32U choice = data >> 1;
        INT32U gamer_now = choice & 0x00000080;

        if (gamer != gamer_now) // gamer_now 是1
        {
            gamer = gamer_now;
            // 闪灯提示
            uart_print_str("\n");
            gpio_out(0x80000000);
            uart_print_str("start! \n");

            // 获得用户输入的数字
            INT32U u_6 = choice & 0x00000040;
            INT32U u_5 = choice & 0x00000020;
            INT32U u_4 = choice & 0x00000010;
            INT32U u_3 = choice & 0x00000008;
            INT32U u_2 = choice & 0x00000004;
            INT32U u_1 = choice & 0x00000002;
            INT32U u_0 = choice & 0x00000001;

            user_number = (u_6 ? 64 : 0) + (u_5 ? 32 : 0) + (u_4 ? 16 : 0) + (u_3 ? 8 : 0) + (u_2 ? 4 : 0) + (u_1 ? 2 : 0) + (u_0 ? 1 : 0);
            uart_print_str("Your guess is ");
            user_input[0] = '0' + (user_number / 10);
            user_input[1] = '0' + (user_number % 10);
            user_input[2] = '\0';
            uart_print_str(user_input);
            uart_print_str(".\n");

            // 比较用户输入的数字和预定义的数字
            if (user_number == predefined_number)
            {
                uart_print_str("Congratulations! You guessed the correct number!\n");
            }
            else if (user_number > predefined_number)
            {
                uart_print_str("Your guess is too high. Try again!\n");
            }
            else
            {
                uart_print_str("Your guess is too low. Try again!\n");
            }
        }
        else
        {
            // uart_print_str("not ready! \n");
            gpio_out(0x00010000);
        }
    }
}

void main()
{
    OSInit();

    uart_init();

    gpio_init();

    OSTaskCreate(TaskStart,
                 (void *)0,
                 &TaskStartStk[TASK_STK_SIZE - 1],
                 0);

    OSStart();
}
