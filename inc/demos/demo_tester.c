/**
 * @file demo_tester.c
 * @author MITurley
 * @date 2026-06-03
 * @brief demo tester <SF>
 */

 #include "demo_tester.h"
 #include "demo_adc.c"

int demo_tester() {
    printf("hello world");
    demo_adc(10);
    return 0;
}