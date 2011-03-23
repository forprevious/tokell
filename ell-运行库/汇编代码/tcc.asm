
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


    		END