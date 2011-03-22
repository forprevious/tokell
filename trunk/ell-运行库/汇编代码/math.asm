;//为所有编译器内函数提供汇编码，值得注意的是，该汇编码实现了
;//编译器内函数的一般行为。
;//----------------------------------------------------------
;//始于版本1.0.0
;//上次修改09.05.04

    AREA _text, CODE, READONLY
    
;//函数 __rt_sdiv 实现了有符号整数的除法。
;//一般而言，针对了 16 位的 thumb 指令。
;//事实上，函数 __rt_sdiv_arm 针对了 16 位 thumb 指令和 32 位
;//arm 指令，此处通过一个模式切换，达到了 thumb 和 arm 转换的
;//目的。
;//----------------------------------------------------------
;//始于版本1.0.0
;//上次修改09.05.04

		CODE16
    EXPORT __rt_sdiv
    
__rt_sdiv
    
    BX 			PC
    DCB		  0, 0
    
    CODE32
		EXPORT __rt_sdiv_arm
    
__rt_sdiv_arm

    ANDS    R2, R0, #0x80000000
    RSBMI   R0, R0, #0
    EORS    R3, R2, R1,ASR#32
    RSBCS   R1, R1, #0
    RSBS    R12, R0, R1,LSR#3
    BCC     loc_80B4
    RSBS    R12, R0, R1,LSR#8
    BCC     loc_8078
    MOV     R0, R0,LSL#8
    ORR     R2, R2, #0xFF000000
    RSBS    R12, R0, R1,LSR#4
    BCC     loc_80A8
    RSBS    R12, R0, R1,LSR#8
    BCC     loc_8078
    MOV     R0, R0,LSL#8
    ORR     R2, R2, #0xFF0000
    RSBS    R12, R0, R1,LSR#8
    MOVCS   R0, R0,LSL#8
    ORRCS   R2, R2, #0xFF00
    RSBS    R12, R0, R1,LSR#4
    BCC     loc_80A8
    RSBS    R12, R0, #0
    BCS     __rt_div0
    
loc_8074
    
    MOVCS   R0, R0,LSR#8

loc_8078

    RSBS    R12, R0, R1,LSR#7
    SUBCS   R1, R1, R0,LSL#7
    ADC     R2, R2, R2
    RSBS    R12, R0, R1,LSR#6
    SUBCS   R1, R1, R0,LSL#6
    ADC     R2, R2, R2
    RSBS    R12, R0, R1,LSR#5
    SUBCS   R1, R1, R0,LSL#5
    ADC     R2, R2, R2
    RSBS    R12, R0, R1,LSR#4
    SUBCS   R1, R1, R0,LSL#4
    ADC     R2, R2, R2

loc_80A8

    RSBS    R12, R0, R1,LSR#3
    SUBCS   R1, R1, R0,LSL#3
    ADC     R2, R2, R2

loc_80B4

    RSBS    R12, R0, R1,LSR#2
    SUBCS   R1, R1, R0,LSL#2
    ADC     R2, R2, R2
    RSBS    R12, R0, R1,LSR#1
    SUBCS   R1, R1, R0,LSL#1
    ADC     R2, R2, R2
    RSBS    R12, R0, R1
    SUBCS   R1, R1, R0
    ADCS    R2, R2, R2
    BCS     loc_8074
    EORS    R0, R2, R3,ASR#31
    ADD     R0, R0, R3,LSR#31
    RSBCS   R1, R1, #0
    BX      LR

;//函数 __rt_udiv 实现了无符号整数的除法。
;//一般而言，针对了 16 位的 thumb 指令。
;//事实上，函数 __rt_udiv_arm 针对了 16 位 thumb 指令和 32 位
;//arm 指令，此处通过一个模式切换，达到了 thumb 和 arm 转换的
;//目的。
;//----------------------------------------------------------
;//始于版本1.0.0
;//上次修改09.05.04

		CODE16
		EXPORT __rt_udiv
		
__rt_udiv

		BX		PC
    DCB 0, 0

		CODE32
		EXPORT __rt_udiv_arm
		
__rt_udiv_arm

		MOV     R2, #0
		RSBS    R12, R0, R1,LSR#3
		BCC     loc_818C
		RSBS    R12, R0, R1,LSR#8
		BCC     loc_8150
		MOV     R0, R0,LSL#8
		ORR     R2, R2, #0xFF000000
		RSBS    R12, R0, R1,LSR#4
		BCC     loc_8180
		RSBS    R12, R0, R1,LSR#8
		BCC     loc_8150
		MOV     R0, R0,LSL#8
		ORR     R2, R2, #0xFF0000
		RSBS    R12, R0, R1,LSR#8
		MOVCS   R0, R0,LSL#8
		ORRCS   R2, R2, #0xFF00
		RSBS    R12, R0, R1,LSR#4
		BCC     loc_8180
		RSBS    R12, R0, #0
		BCS     __rt_div0

loc_814C

		MOVCS   R0, R0,LSR#8

loc_8150

		RSBS    R12, R0, R1,LSR#7
		SUBCS   R1, R1, R0,LSL#7
		ADC     R2, R2, R2
		RSBS    R12, R0, R1,LSR#6
		SUBCS   R1, R1, R0,LSL#6
		ADC     R2, R2, R2
		RSBS    R12, R0, R1,LSR#5
		SUBCS   R1, R1, R0,LSL#5
		ADC     R2, R2, R2
		RSBS    R12, R0, R1,LSR#4
		SUBCS   R1, R1, R0,LSL#4
		ADC     R2, R2, R2

loc_8180

		RSBS    R12, R0, R1,LSR#3
		SUBCS   R1, R1, R0,LSL#3
		ADC     R2, R2, R2

loc_818C

		RSBS    R12, R0, R1,LSR#2
		SUBCS   R1, R1, R0,LSL#2
		ADC     R2, R2, R2
		RSBS    R12, R0, R1,LSR#1
		SUBCS   R1, R1, R0,LSL#1
		ADC     R2, R2, R2
		RSBS    R12, R0, R1
		SUBCS   R1, R1, R0
		ADCS    R2, R2, R2
		BCS     loc_814C
		MOV     R0, R2
		BX      LR

;//函数 __rt_div0 实现了对零的除法。
;//事实上，我们在这里默认不处理除零操作。
;//----------------------------------------------------------
;//始于版本1.0.0
;//上次修改09.05.04

	  CODE32
    EXPORT __rt_div0
    
__rt_div0

    END