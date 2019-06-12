    .syntax unified

    @ --------------------------------
    .global main
main:
    @ Stack the return address (lr) in addition to a dummy register (ip) to
    @ keep the stack 8-byte aligned.
    stmfd   sp!, {fp, lr}
    add     fp, sp, #4
    sub     sp, sp, #8

    @ Load the argument and perform the call. This is like 'printf("...")' in C.
    ldr     r0, =port
    ldr     r1, =02
    bl      open

    str     r0, [fp, #-8]
    ldr     r1, =0x0703
    ldr     r2, =0x46
    bl      ioctl
    
    ldr     r0, [fp, #-8]
    ldr     r1, =variable
    str     r0,[r1]
    ldr     r1,=buffer_L1
    ldr     r2, =25
    bl      write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L2
    ldr r2, =25
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L3
    ldr r2, =25 
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L4
    ldr r2, =25 
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L5
    ldr r2, =25 
    bl  write
    
    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L6
    ldr r2, =25 
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L7
    ldr r2, =25 
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    ldr r1, =buffer_L8
    ldr r2, =25 
    bl  write

    ldr r1, =variable
    ldr r0, [r1]
    bl close
 
   @ write the data

    @ cleanup?

    @ Exit from 'main'. This is like 'return 0' in C.
    mov     r3, #0
    mov     r0, r3
    sub     sp, fp, #4
    ldmfd   sp!, {fp, pc}

print:
    .asciz "test: %c\n"
port:
    .asciz  "/dev/i2c-1"

buffer_L1:
   .byte 0 @relative address
	@Red
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
	@Blue
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
	@Green
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000

buffer_L2:
   .byte 24
        @Red
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
        @Green
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000

buffer_L3:
   .byte 48
	@Red
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
	@Blue
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
	@Green
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000

buffer_L4:
   .byte 72
        @Red
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
	@Green
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000

buffer_L5:
   .byte 96
        @Red
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
        @Green
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000

buffer_L6:
   .byte 120
        @Red
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
        @Green
   .byte 0b000000
   .byte 0b011111
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000

buffer_L7:
   .byte 144
        @Red
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000
        @Green
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b000000
   .byte 0b000000
   .byte 0b111111
   .byte 0b111111
   .byte 0b000000

buffer_L8:
   .byte 168
	@Red
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
        @Blue
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
        @Green
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000
   .byte 0b000000


.data
variable: .byte 0 
