;******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
;* File Name          : startup_stm32f40_41xxx.s
;* Author             : MCD Application Team
;* @version           : V1.4.0
;* @date              : 04-August-2014
;* Description        : STM32F40xxx/41xxx devices vector table for MDK-ARM toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the system clock and the external SRAM mounted on 
;*                        STM324xG-EVAL board to be used as data memory (optional, 
;*                        to be enabled by user)
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; 
; Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
; You may not use this file except in compliance with the License.
; You may obtain a copy of the License at:
; 
;        http://www.st.com/software_license_agreement_liberty_v2
; 
; Unless required by applicable law or agreed to in writing, software 
; distributed under the License is distributed on an "AS IS" BASIS, 
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
; 
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET_ram, DATA, READONLY
                EXPORT  __Vectors_ram
                EXPORT  __Vectors_End_ram
                EXPORT  __Vectors_Size_ram

__Vectors_ram   DCD     0               ; Top of Stack
                DCD     0              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                   ; Window WatchDog                                        
                DCD     PVD_IRQHandler                    ; PVD through EXTI Line detection                        
                DCD     TAMP_STAMP_IRQHandler             ; Tamper and TimeStamps through the EXTI line            
                DCD     RTC_WKUP_IRQHandler               ; RTC Wakeup through the EXTI line                       
                DCD     FLASH_IRQHandler                  ; FLASH        
                DCD     RCC_IRQHandler                    ; RCC          
                DCD     EXTI0_IRQHandler                  ; EXTI Line0   
                DCD     EXTI1_IRQHandler                  ; EXTI Line1   
                DCD     EXTI2_IRQHandler                  ; EXTI Line2   
                DCD     EXTI3_IRQHandler                  ; EXTI Line3   
                DCD     EXTI4_IRQHandler                  ; EXTI Line4   
                DCD     DMA1_Stream0_IRQHandler           ; DMA1 Stream 0
                DCD     DMA1_Stream1_IRQHandler           ; DMA1 Stream 1
                DCD     DMA1_Stream2_IRQHandler           ; DMA1 Stream 2
                DCD     DMA1_Stream3_IRQHandler           ; DMA1 Stream 3
                DCD     DMA1_Stream4_IRQHandler           ; DMA1 Stream 4
                DCD     DMA1_Stream5_IRQHandler           ; DMA1 Stream 5
                DCD     DMA1_Stream6_IRQHandler           ; DMA1 Stream 6
                DCD     ADC_IRQHandler                    ; ADC1, ADC2 and ADC3s 
                DCD     CAN1_TX_IRQHandler                ; CAN1 TX 
                DCD     CAN1_RX0_IRQHandler               ; CAN1 RX0
                DCD     CAN1_RX1_IRQHandler               ; CAN1 RX1
                DCD     CAN1_SCE_IRQHandler               ; CAN1 SCE
                DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s  
                DCD     TIM1_BRK_TIM9_IRQHandler          ; TIM1 Break and TIM9  
                DCD     TIM1_UP_TIM10_IRQHandler          ; TIM1 Update and TIM10
                DCD     TIM1_TRG_COM_TIM11_IRQHandler     ; TIM1 Trigger and Commutation and TIM11
                DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare
                DCD     TIM2_IRQHandler                   ; TIM2      
                DCD     TIM3_IRQHandler                   ; TIM3      
                DCD     TIM4_IRQHandler                   ; TIM4      
                DCD     I2C1_EV_IRQHandler                ; I2C1 Event
                DCD     I2C1_ER_IRQHandler                ; I2C1 Error
                DCD     I2C2_EV_IRQHandler                ; I2C2 Event
                DCD     I2C2_ER_IRQHandler                ; I2C2 Error
                DCD     SPI1_IRQHandler                   ; SPI1      
                DCD     SPI2_IRQHandler                   ; SPI2      
                DCD     USART1_IRQHandler                 ; USART1    
                DCD     USART2_IRQHandler                 ; USART2    
                DCD     USART3_IRQHandler                 ; USART3    
                DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s                 
                DCD     RTC_Alarm_IRQHandler              ; RTC Alarm (A and B) through EXTI Line 
                DCD     OTG_FS_WKUP_IRQHandler            ; USB OTG FS Wakeup through EXTI line   
                DCD     TIM8_BRK_TIM12_IRQHandler         ; TIM8 Break and TIM12 
                DCD     TIM8_UP_TIM13_IRQHandler          ; TIM8 Update and TIM13
                DCD     TIM8_TRG_COM_TIM14_IRQHandler     ; TIM8 Trigger and Commutation and TIM1
                DCD     TIM8_CC_IRQHandler                ; TIM8 Capture Compare
                DCD     DMA1_Stream7_IRQHandler           ; DMA1 Stream7        
                DCD     FSMC_IRQHandler                   ; FSMC 
                DCD     SDIO_IRQHandler                   ; SDIO 
                DCD     TIM5_IRQHandler                   ; TIM5 
                DCD     SPI3_IRQHandler                   ; SPI3 
                DCD     UART4_IRQHandler                  ; UART4
                DCD     UART5_IRQHandler                  ; UART5 
                DCD     TIM6_DAC_IRQHandler               ; TIM6 and DAC1&2 underrun errors                   
                DCD     TIM7_IRQHandler                   ; TIM7         
                DCD     DMA2_Stream0_IRQHandler           ; DMA2 Stream 0
                DCD     DMA2_Stream1_IRQHandler           ; DMA2 Stream 1
                DCD     DMA2_Stream2_IRQHandler           ; DMA2 Stream 2
                DCD     DMA2_Stream3_IRQHandler           ; DMA2 Stream 3
                DCD     DMA2_Stream4_IRQHandler           ; DMA2 Stream 4
                DCD     ETH_IRQHandler                    ; Ethernet     
                DCD     ETH_WKUP_IRQHandler               ; Ethernet Wakeup through EXTI line                      
                DCD     CAN2_TX_IRQHandler                ; CAN2 TX 
                DCD     CAN2_RX0_IRQHandler               ; CAN2 RX0
                DCD     CAN2_RX1_IRQHandler               ; CAN2 RX1
                DCD     CAN2_SCE_IRQHandler               ; CAN2 SCE
                DCD     OTG_FS_IRQHandler                 ; USB OTG FS
                DCD     DMA2_Stream5_IRQHandler           ; DMA2 Stream 5
                DCD     DMA2_Stream6_IRQHandler           ; DMA2 Stream 6
                DCD     DMA2_Stream7_IRQHandler           ; DMA2 Stream 7
                DCD     USART6_IRQHandler                 ; USART6    
                DCD     I2C3_EV_IRQHandler                ; I2C3 event
                DCD     I2C3_ER_IRQHandler                ; I2C3 error 
                DCD     OTG_HS_EP1_OUT_IRQHandler         ; USB OTG HS End Point 1 Out
                DCD     OTG_HS_EP1_IN_IRQHandler          ; USB OTG HS End Point 1 In 
                DCD     OTG_HS_WKUP_IRQHandler            ; USB OTG HS Wakeup through EXTI
                DCD     OTG_HS_IRQHandler                 ; USB OTG HS 
                DCD     DCMI_IRQHandler                   ; DCMI       
                DCD     CRYP_IRQHandler                   ; CRYP crypto
                DCD     HASH_RNG_IRQHandler               ; Hash and Rng
                DCD     FPU_IRQHandler                    ; FPU
__Vectors_End_ram

__Vectors_Size_ram  EQU  __Vectors_End_ram - __Vectors_ram

                AREA    |.text|, CODE, READONLY

                IMPORT  NMI_Handler                ; NMI Handler
                IMPORT  HardFault_Handler          ; Hard Fault Handler
                IMPORT  MemManage_Handler          ; MPU Fault Handler
                IMPORT  BusFault_Handler           ; Bus Fault Handler
                IMPORT  UsageFault_Handler         ; Usage Fault Handler
                IMPORT  SVC_Handler                ; SVCall Handler
                IMPORT  DebugMon_Handler           ; Debug Monitor Handler
                IMPORT  PendSV_Handler             ; PendSV Handler
                IMPORT  SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                IMPORT  WWDG_IRQHandler                   [WEAK]    ; Window WatchDog                             
                IMPORT  PVD_IRQHandler                    [WEAK]    ; PVD through EXTI Line detection             
                IMPORT  TAMP_STAMP_IRQHandler             [WEAK]    ; Tamper and TimeStamps through the EXTI line 
                IMPORT  RTC_WKUP_IRQHandler               [WEAK]    ; RTC Wakeup through the EXTI line            
                IMPORT  FLASH_IRQHandler                  [WEAK]    ; FLASH         
                IMPORT  RCC_IRQHandler                    [WEAK]    ; RCC           
                IMPORT  EXTI0_IRQHandler                  [WEAK]    ; EXTI Line0    
                IMPORT  EXTI1_IRQHandler                  [WEAK]    ; EXTI Line1    
                IMPORT  EXTI2_IRQHandler                  [WEAK]    ; EXTI Line2    
                IMPORT  EXTI3_IRQHandler                  [WEAK]    ; EXTI Line3    
                IMPORT  EXTI4_IRQHandler                  [WEAK]    ; EXTI Line4    
                IMPORT  DMA1_Stream0_IRQHandler           [WEAK]    ; DMA1 Stream 0 
                IMPORT  DMA1_Stream1_IRQHandler           [WEAK]    ; DMA1 Stream 1 
                IMPORT  DMA1_Stream2_IRQHandler           [WEAK]    ; DMA1 Stream 2 
                IMPORT  DMA1_Stream3_IRQHandler           [WEAK]    ; DMA1 Stream 3 
                IMPORT  DMA1_Stream4_IRQHandler           [WEAK]    ; DMA1 Stream 4 
                IMPORT  DMA1_Stream5_IRQHandler           [WEAK]    ; DMA1 Stream 5 
                IMPORT  DMA1_Stream6_IRQHandler           [WEAK]    ; DMA1 Stream 6 
                IMPORT  ADC_IRQHandler                    [WEAK]    ; ADC1, ADC2 and ADC3s                        
                IMPORT  CAN1_TX_IRQHandler                [WEAK]    ; CAN1 TX 
                IMPORT  CAN1_RX0_IRQHandler               [WEAK]    ; CAN1 RX0
                IMPORT  CAN1_RX1_IRQHandler               [WEAK]    ; CAN1 RX1
                IMPORT  CAN1_SCE_IRQHandler               [WEAK]    ; CAN1 SCE
                IMPORT  EXTI9_5_IRQHandler                [WEAK]    ; External Line[9:5]s  
                IMPORT  TIM1_BRK_TIM9_IRQHandler          [WEAK]    ; TIM1 Break and TIM9  
                IMPORT  TIM1_UP_TIM10_IRQHandler          [WEAK]    ; TIM1 Update and TIM10
                IMPORT  TIM1_TRG_COM_TIM11_IRQHandler     [WEAK]    ; TIM1 Trigger and Commutation and TIM11
                IMPORT  TIM1_CC_IRQHandler                [WEAK]    ; TIM1 Capture Compare                        
                IMPORT  TIM2_IRQHandler                   [WEAK]    ; TIM2      
                IMPORT  TIM3_IRQHandler                   [WEAK]    ; TIM3      
                IMPORT  TIM4_IRQHandler                   [WEAK]    ; TIM4      
                IMPORT  I2C1_EV_IRQHandler                [WEAK]    ; I2C1 Event
                IMPORT  I2C1_ER_IRQHandler                [WEAK]    ; I2C1 Error
                IMPORT  I2C2_EV_IRQHandler                [WEAK]    ; I2C2 Event
                IMPORT  I2C2_ER_IRQHandler                [WEAK]    ; I2C2 Error
                IMPORT  SPI1_IRQHandler                   [WEAK]    ; SPI1      
                IMPORT  SPI2_IRQHandler                   [WEAK]    ; SPI2      
                IMPORT  USART1_IRQHandler                 [WEAK]    ; USART1    
                IMPORT  USART2_IRQHandler                 [WEAK]    ; USART2    
                IMPORT  USART3_IRQHandler                 [WEAK]    ; USART3    
                IMPORT  EXTI15_10_IRQHandler              [WEAK]    ; External Line[15:10]s                       
                IMPORT  RTC_Alarm_IRQHandler              [WEAK]    ; RTC Alarm (A and B) through EXTI Line       
                IMPORT  OTG_FS_WKUP_IRQHandler            [WEAK]    ; USB OTG FS Wakeup through EXTI line         
                IMPORT  TIM8_BRK_TIM12_IRQHandler         [WEAK]    ; TIM8 Break and TIM12                  
                IMPORT  TIM8_UP_TIM13_IRQHandler          [WEAK]    ; TIM8 Update and TIM13                 
                IMPORT  TIM8_TRG_COM_TIM14_IRQHandler     [WEAK]    ; TIM8 Trigger and Commutation and TIM14
                IMPORT  TIM8_CC_IRQHandler                [WEAK]    ; TIM8 Capture Compare            
                IMPORT  DMA1_Stream7_IRQHandler           [WEAK]    ; DMA1 Stream7
                IMPORT  FSMC_IRQHandler                   [WEAK]    ; FSMC        
                IMPORT  SDIO_IRQHandler                   [WEAK]    ; SDIO        
                IMPORT  TIM5_IRQHandler                   [WEAK]    ; TIM5        
                IMPORT  SPI3_IRQHandler                   [WEAK]    ; SPI3        
                IMPORT  UART4_IRQHandler                  [WEAK]    ; UART4       
                IMPORT  UART5_IRQHandler                  [WEAK]    ; UART5       
                IMPORT  TIM6_DAC_IRQHandler               [WEAK]    ; TIM6 and DAC1&2 underrun errors 
                IMPORT  TIM7_IRQHandler                   [WEAK]    ; TIM7         
                IMPORT  DMA2_Stream0_IRQHandler           [WEAK]    ; DMA2 Stream 0
                IMPORT  DMA2_Stream1_IRQHandler           [WEAK]    ; DMA2 Stream 1
                IMPORT  DMA2_Stream2_IRQHandler           [WEAK]    ; DMA2 Stream 2
                IMPORT  DMA2_Stream3_IRQHandler           [WEAK]    ; DMA2 Stream 3
                IMPORT  DMA2_Stream4_IRQHandler           [WEAK]    ; DMA2 Stream 4
                IMPORT  ETH_IRQHandler                    [WEAK]    ; Ethernet     
                IMPORT  ETH_WKUP_IRQHandler               [WEAK]    ; Ethernet Wakeup through EXTI line 
                IMPORT  CAN2_TX_IRQHandler                [WEAK]    ; CAN2 TX           
                IMPORT  CAN2_RX0_IRQHandler               [WEAK]    ; CAN2 RX0     
                IMPORT  CAN2_RX1_IRQHandler               [WEAK]    ; CAN2 RX1     
                IMPORT  CAN2_SCE_IRQHandler               [WEAK]    ; CAN2 SCE     
                IMPORT  OTG_FS_IRQHandler                 [WEAK]    ; USB OTG FS 
                IMPORT  DMA2_Stream5_IRQHandler           [WEAK]    ; DMA2 Stream 5 
                IMPORT  DMA2_Stream6_IRQHandler           [WEAK]    ; DMA2 Stream 6 
                IMPORT  DMA2_Stream7_IRQHandler           [WEAK]    ; DMA2 Stream 7 
                IMPORT  USART6_IRQHandler                 [WEAK]    ; USART6        
                IMPORT  I2C3_EV_IRQHandler                [WEAK]    ; I2C3 event      
                IMPORT  I2C3_ER_IRQHandler                [WEAK]    ; I2C3 error                  
                IMPORT  OTG_HS_EP1_OUT_IRQHandler         [WEAK]    ; USB OTG HS End Point 1 Out
                IMPORT  OTG_HS_EP1_IN_IRQHandler          [WEAK]    ; USB OTG HS End Point 1 In 
                IMPORT  OTG_HS_WKUP_IRQHandler            [WEAK]    ; USB OTG HS Wakeup through EXTI   
                IMPORT  OTG_HS_IRQHandler                 [WEAK]    ; USB OTG HS
                IMPORT  DCMI_IRQHandler                   [WEAK]    ; DCMI  
                IMPORT  CRYP_IRQHandler                   [WEAK]    ; CRYP crypto 
                IMPORT  HASH_RNG_IRQHandler               [WEAK]    ; Hash and Rng
                IMPORT  FPU_IRQHandler                    [WEAK]    ; FPU

				END                
;************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****
