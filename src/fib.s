	.syntax unified
	.arch armv7-m
	.text
	.align 2
	.thumb
	.thumb_func

	.global fibonacci
	.type fibonacci, function

fibonacci:
	push { r4, r5, r6, r7, r8, r10, r11, lr}

    cmp r0,#0                   @n<=0, f(n)=0
    ble .L3

    @invariants ,m(current Fibonacci) start as 1
    mov r5,#1                   @a = [r6,r5] = F(m)
    mov r6,#0
    mov r7,#1                   @b = [r8,r7] = F(m+1)
    mov r8,#0

    clz r4,r0                   @count leading zero of r0
                                @really surprised this operation exists
    rsbs r4,r4,#30              @(start looking second digit ,skip m=0)
    blt exit                    @when r0 = 1 ,skip
loop:    
    @doubling Fibonacci here
    @use[r3,r2] & [r11,r10] & r1 as temp variable

    @calculate new F(2m)
    lsl r1,r8,#1            @rsc not supportes in thumb
                            @can not barrel shift r8 on substraction
    rsbs r2,r5,r7,LSL #1    @temp = (b<<1)-a
    sbc r3,r1,r6
    add r3,r3,r7,LSR #31    @add r7 msb to high word for shift

    mul r11,r5,r3           @temp2 = a*temp = a*(2*b-a) = F(2m)
    mla r11,r6,r2,r11
    umull r10,r11,r5,r2     @now, [r11,r10] = F(2m)

    @f(2m+1)
    mul r1,r8,r7            @temp = a^2
    umull r7,r8,r7,r7       @result go directly into b
    add r8,r8,r1,LSL #1
     
    mul r1,r6,r5            @temp = b^2 + a^2 = F(2m+1)
    umlal r7,r8,r5,r5
    add r8,r8,r1,LSL #1     @now,b = [r8,r7] = F(2m+1)

    @update a : m*=2 (b already updated)
    mov r5,r10
    mov r6,r11

    @advance one conditionally
    lsr r1,r0,r4
    tst r1,#1
    beq no_adv
    adds r2,r5,r7
    adc r3,r6,r8
    mov r5,r7               @a = F(m+1)
    mov r6,r8                   
    mov r7,r2               @b = F(m+2) ,m++
    mov r8,r3
no_adv:
    cbz r4,exit             @operation complete
    sub r4,r4,#1
    b loop

exit:
    mov r0,r5
    mov r1,r6
	pop { r4, r5, r6, r7, r8, r10, r11, pc}
.L3:
	mov r0, #0			            @ R0 = 0
	pop { r4, r5, r6, r7, r8, r10, r11, pc}

	.size fibonacci, .-fibonacci
	.end
