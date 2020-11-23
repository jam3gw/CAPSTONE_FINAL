#ifndef MOTOR_H
#define MOTOR_H

#include <msp430.h>
#include <stddef.h>

#define SLEEP_BIT                      BIT0
#define SLEEP_PORT                     P1OUT
#define SLEEP_DDR                      P1DIR
#define SET_SLEEP_AS_AN_OUTPUT         SLEEP_DDR |= SLEEP_BIT
#define ENABLE_SLEEP                   SLEEP_PORT |= SLEEP_BIT

#define NENBL_BIT                      BIT4
#define NENBL_PORT                     P2OUT
#define NENBL_DDR                      P2DIR
#define SET_NENBL_AS_AN_OUTPUT         NENBL_DDR |= NENBL_BIT
#define LOW_NENBL                   NENBL_PORT &= ~NENBL_BIT
#define HIGH_NENBL                   NENBL_PORT |= NENBL_BIT

#define STEP_BIT                      BIT6
#define STEP_PORT                     P2OUT
#define STEP_DDR                      P2DIR
#define SET_STEP_AS_AN_OUTPUT         STEP_DDR |= STEP_BIT
#define ENABLE_STEP                   STEP_PORT |= STEP_BIT
#define DISABLE_STEP                  STEP_PORT &= ~STEP_BIT

#define DIR_BIT                      BIT5
#define DIR_PORT                     P2OUT
#define DIR_DDR                      P2DIR
#define SET_DIR_AS_AN_OUTPUT         DIR_DDR |= DIR_BIT
#define ENABLE_DIR                   DIR_PORT |= DIR_BIT
#define DISABLE_DIR                   DIR_PORT &= ~DIR_BIT

#define M0_BIT                      BIT3
#define M0_PORT                     P1OUT
#define M0_DDR                      P1DIR
#define SET_M0_AS_AN_OUTPUT         M0_DDR |= M0_BIT
#define LOW_M0                   M0_PORT &= ~M0_BIT
#define HIGH_M0                   M0_PORT |= M0_BIT

#define M1_BIT                      BIT2
#define M1_PORT                     P1OUT
#define M1_DDR                      P1DIR
#define SET_M1_AS_AN_OUTPUT         M1_DDR |= M1_BIT
#define LOW_M1                   M1_PORT &= ~M1_BIT
#define HIGH_M1                   M1_PORT |= M1_BIT

#define CONFIG_BIT                      BIT1
#define CONFIG_PORT                     P2OUT
#define CONFIG_DDR                      P2DIR
#define SET_CONFIG_AS_AN_OUTPUT         CONFIG_DDR |= CONFIG_BIT
#define ENABLE_CONFIG                   CONFIG_PORT |= CONFIG_BIT

#define NFAULT_BIT                      BIT4
#define NFAULT_PORT                     P1IN
#define NFAULT_DDR                      P1DIR
#define SET_NFAULT_AS_AN_INPUT          NFAULT_DDR &= ~NFAULT_BIT

#endif
