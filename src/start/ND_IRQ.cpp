/**
 * @file ND_IRQ.hpp
 * @author Adrián Arroyo Calle
 * @brief Interrupt Requests
 * */

#include <ND_IRQ.hpp>
#include <ND_Ports.hpp>
#include <ND_IDT.hpp>
#include <ND_ISR.hpp>
#include <ND_Screen.hpp>

void* irq[16]=
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
extern "C"
void ND_IRQ_InstallHandler(int irq_code,void (*handler)(struct regs* r))
{
	irq[irq_code]=(void*)handler;
}
void ND::IRQ::UninstallHandler(int irq_code)
{
	irq[irq_code]=0;
}
void ND::IRQ::Install()
{
	ND::Screen::SetColor(ND_SIDE_FOREGROUND,ND_COLOR_BLACK);
	ND::Screen::PutString("\nInstalling IRQ...");
	
	
	
	ND::Screen::SetColor(ND_SIDE_FOREGROUND,ND_COLOR_GREEN);
	ND::Screen::PutString("done");
	
	asm volatile("sti");
}
void ND::IRQ::Remap(int pic1, int pic2)
{
#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01
	/* send ICW1 */
	ND::Ports::OutputB(PIC1, ICW1);
	ND::Ports::OutputB(PIC2, ICW1);

	/* send ICW2 */
	ND::Ports::OutputB(PIC1 + 1, pic1);	/* remap */
	ND::Ports::OutputB(PIC2 + 1, pic2);	/*  pics */

	/* send ICW3 */
	ND::Ports::OutputB(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
	ND::Ports::OutputB(PIC2 + 1, 2);

	/* send ICW4 */
	ND::Ports::OutputB(PIC1 + 1, ICW4);
	ND::Ports::OutputB(PIC2 + 1, ICW4);

	/* disable all IRQs */
	ND::Ports::OutputB(PIC1 + 1, 0xFF);
}
void ND::IRQ::IRQ1()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	32\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ2()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	33\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ3()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	34\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ4()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	35\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ5()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	36\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ6()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	37\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ7()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	38\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ8()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	39\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ9()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	40\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ10()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	41\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ11()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	42\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ12()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	43\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ13()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	44\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ14()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	45\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ15()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	46\n"
	"jmp ND_IRQ_Common"
	);
}
void ND::IRQ::IRQ16()
{
	asm volatile(
	"cli	\n"
	"pushl	0\n"
	"pushl	47\n"
	"jmp ND_IRQ_Common"
	);
}

extern "C"
void ND_IRQ_Handler(struct regs* r)
{
	ND::Screen::PutString("HANDLER CALLED");
	void (*handler)(struct regs *r);
	if(r->int_no >= 40)
	{
		ND::Ports::OutputB(0xA0,0x20);
	}
	ND::Ports::OutputB(0x20,0x20);
	handler=(void (*)(struct regs *r))irq[r->int_no - 32];
	if(handler)
	{
		handler(r);
	}	
}
extern "C"
void ND_IRQ_Common()
{
	asm volatile(
	"pusha	\n"
	"push	%ds\n"
	"push	%es\n"
	"push	%fs\n"
	"push	%gs\n"
	"movw $0x10, %ax \n"
	"movw %ax, %ds \n"
	"movw %ax, %es \n"
	"movw %ax, %fs \n"
	"movw %ax, %gs \n"
	"movl %esp, %eax \n"
	"push %eax \n"
	"movl $ND_IRQ_Handler, %eax \n"
	"call *%eax \n"
	"popl %eax \n"
	"popl %ds \n"
	"popl %es \n"
	"popl %fs \n"
	"popl %gs \n"
	"popa \n"
	"addl 8, %esp \n"
	"sti	\n"
	"iret \n"
	);
}