
;	Executable Linked-Library 1.0.0.
;	Architecture : ARMv7.																						
;	(c)	Techniques of Knowledge
;		an open source group since 2008
;		page : http://www.tok.cc
;		email : wqw85@sina.com


    AREA _ell_text, CODE, READONLY
    


    		CODE16
		EXPORT __call_via_0r0
 
__call_via_0r0

    		BX      R0




		CODE16
		EXPORT __call_via_r0

__call_via_r0

		BX		R0



		CODE16
		EXPORT __call_via_r1
__call_via_r1

		BX		R1



		CODE16
		EXPORT __call_via_r2

__call_via_r2

		BX		R2




		CODE16
		EXPORT __call_via_r3

__call_via_r3

		BX		R3



		CODE16
		EXPORT __call_via_r4

__call_via_r4


		BX		R4

		CODE16
		EXPORT __call_via_r5

__call_via_r5

		BX		R5


		CODE16
		EXPORT __call_via_r6

__call_via_r6

		BX		R6

		CODE16
		EXPORT __call_via_r7

__call_via_r7

		BX		R7


		CODE16
		EXPORT __rt_memcpy

__rt_memcpy
		

		BX		PC
		
		
		CODE32
               	EXPORT __32_rt_memcpy
__32_rt_memcpy                             ; CODE XREF: __16__rt_memcpyj
                 CMP     R2, #3
                 BLS     _memcpy_lastbytes
                 ANDS    R12, R0, #3
                 BEQ     _memcpy_dest_aligned
                 LDRB    R3, [R1],#1
                 CMP     R12, #2
                 ADD     R2, R2, R12
                 LDRLSB  R12, [R1],#1
                 STRB    R3, [R0],#1
                 LDRCCB  R3, [R1],#1
                 SUB     R2, R2, #4
                 STRLSB  R12, [R0],#1
                 STRCCB  R3, [R0],#1
 ; End of function __rt_memcpy


 ; =============== S U B R O U T I N E =======================================


_memcpy_dest_aligned                    ; CODE XREF: __rt_memcpy+Cj
                 ANDS    R3, R1, #3
                 BEQ     __rt_memcpy_w
                 SUBS    R2, R2, #4
                 BCC     _memcpy_lastbytes
                 LDR     R12, [R1,-R3]!
                 CMP     R3, #2
                 BEQ     _memcpy_src2_loop
                 BHI     _memcpy_src3_loop
 ; End of function _memcpy_dest_aligned


 ; =============== S U B R O U T I N E =======================================


_memcpy_src1_loop                       ; CODE XREF: _memcpy_src1_loop+14j
                 MOV     R3, R12,LSR#8
                 LDR     R12, [R1,#4]!
                 SUBS    R2, R2, #4
                 ORR     R3, R3, R12,LSL#24
                 STR     R3, [R0],#4
                 BCS     _memcpy_src1_loop
                 ADD     R1, R1, #1
                 B       _memcpy_lastbytes
 ; End of function _memcpy_src1_loop


 ; =============== S U B R O U T I N E =======================================


_memcpy_src2_loop                       ; CODE XREF: _memcpy_dest_aligned+18j
                                         ; _memcpy_src2_loop+14j
                 MOV     R3, R12,LSR#16
                 LDR     R12, [R1,#4]!
                 SUBS    R2, R2, #4
                 ORR     R3, R3, R12,LSL#16
                 STR     R3, [R0],#4
                 BCS     _memcpy_src2_loop
                 ADD     R1, R1, #2
                 B       _memcpy_lastbytes
 ; End of function _memcpy_src2_loop


 ; =============== S U B R O U T I N E =======================================


_memcpy_src3_loop                       ; CODE XREF: _memcpy_dest_aligned+1Cj
                                         ; _memcpy_src3_loop+14j
                 MOV     R3, R12,LSR#24
                 LDR     R12, [R1,#4]!
                 SUBS    R2, R2, #4
                 ORR     R3, R3, R12,LSL#8
                 STR     R3, [R0],#4
                 BCS     _memcpy_src3_loop
                 ADD     R1, R1, #3
                 B       _memcpy_lastbytes
 ; End of function _memcpy_src3_loop

                 CODE16

 ; =============== S U B R O U T I N E =======================================

 ; Attributes: thunk

                 EXPORT __16__rt_memcpy_w
__16__rt_memcpy_w
                 BX      PC
 ; End of function __16__rt_memcpy_w

 ; ---------------------------------------------------------------------------
                 ALIGN 4
                 CODE32

 ; =============== S U B R O U T I N E =======================================


                 EXPORT __rt_memcpy_w
__rt_memcpy_w                           ; CODE XREF: _memcpy_dest_aligned+4j
                                         ; __16__rt_memcpy_wj
                 STMFD   SP!, {R4,LR}
                 SUBS    R2, R2, #0x20
                 BCC     _memcpy_small
 ; End of function __rt_memcpy_w


 ; =============== S U B R O U T I N E =======================================


_memcpy_aligned_loop                    ; CODE XREF: _memcpy_aligned_loop+14j
                 LDMCSIA R1!, {R3,R4,R12,LR}
                 STMCSIA R0!, {R3,R4,R12,LR}
                 LDMCSIA R1!, {R3,R4,R12,LR}
                 STMCSIA R0!, {R3,R4,R12,LR}
                 SUBCSS  R2, R2, #0x20
                 BCS     _memcpy_aligned_loop
 ; End of function _memcpy_aligned_loop


 ; =============== S U B R O U T I N E =======================================


_memcpy_small                           ; CODE XREF: __rt_memcpy_w+8j
                 MOVS    R12, R2,LSL#28
                 LDMCSIA R1!, {R3,R4,R12,LR}
                 STMCSIA R0!, {R3,R4,R12,LR}
                 LDMMIIA R1!, {R3,R4}
                 STMMIIA R0!, {R3,R4}
                 LDMFD   SP!, {R4,LR}
                 MOVS    R12, R2,LSL#30
                 LDRCS   R3, [R1],#4
                 STRCS   R3, [R0],#4
                 BXEQ    LR
 ; End of function _memcpy_small


 ; =============== S U B R O U T I N E =======================================


                 EXPORT _memcpy_lastbytes
_memcpy_lastbytes                       ; CODE XREF: __rt_memcpy+4j
                                         ; _memcpy_dest_aligned+Cj ...
                 MOVS    R2, R2,LSL#31
                 LDRMIB  R2, [R1],#1
                 LDRCSB  R3, [R1],#1
                 LDRCSB  R12, [R1],#1
                 STRMIB  R2, [R0],#1
                 STRCSB  R3, [R0],#1
                 STRCSB  R12, [R0],#1
                 BX      LR

    		END