#include <cnix/config.h>
#include <cnix/kernel.h>
#include <cnix/sched.h>
#include <cnix/asm.h>
#include <cnix/lapic.h>
#include <cnix/traps.h>
#include <cnix/desc.h>

extern void int_lvt_timer();

struct cpu_struct cpu_tab[NR_CPU_MAX] = { 0 };

#define rdcoms(_r,_i) 	{ outb(0x70, 0x80| _i); _r = inb(0x71); }
#define COMS_SEC	0x00
#define HZ 60

static uint64_t __init get_tsc_diff()
{
	volatile long s1, s2;
	uint64_t tsc1, tsc2;

	s1 = s2 = 0;
	rdcoms(s1, COMS_SEC);
	do{
		rdcoms(s2, COMS_SEC);
	}while(s1 == s2);
	tsc1 = rdtsc();

	s1 = s2 = 0;
	rdcoms(s1, COMS_SEC);
	do{
		rdcoms(s2, COMS_SEC);
	}while(s1 == s2);
	tsc2 = rdtsc();

	return tsc2 - tsc1;
}

// Save TS, XMM, YMM ?
struct ctx_struct{
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rflags;
	uint64_t retaddr;
};

static long build_ctx(long* p, long func, long arg)
{
	struct ctx_struct *ctx =(struct ctx_struct*)p - 1;
	ctx->retaddr = func;
	ctx->rdi = arg;
	ctx->rflags = 0x10200;
	return (long)ctx;
}

void putc_loop(long c)
{
	while(1){
//		printk("%c", c);
		__asm__ __volatile__("1:;sti;hlt;");
	}
}

static union thread* init_idle_thread(long cpu_id)
{
	union thread* th = (union thread*)((long)&_brk + cpu_id * 8192);

	th->cpu_id = cpu_id;
	//th->stack = (long)&th->canarry2;
	return th;
}

void __init cpu_init()
{
	struct cpu_struct *cpu = cpu_tab + 0;
	for(int i = 0; i < NR_CPUS; i++, cpu++){
		cpu->idle = init_idle_thread(i);
		cpu->ready = NULL;
	}
}

union thread* kthread(long func, long arg, long cpu_id)
{
	long p = alloc_2page();
	if(!p){
		printk("OOM");
		while(1);
	}

	union thread* th = (union thread*)__p2v(p);
	th->stack = build_ctx(&th->canarry2, func, arg);
	th->cpu_id = cpu_id;
	//th->next = NULL;
	// BUG Note: data race
	struct cpu_struct* cpu = cpu_tab + cpu_id;
	th->next = cpu->ready;
	cpu->ready = th;
	return th;
}

void __init sched_init(long cpu_id)
{
	/*if(cpu_id >= NR_CPU_MAX){
		cli();while(1){}
	}

	//if(cpu_id !=0){
		kthread((long)putc_loop, 'A' + cpu_id, cpu_id);
		kthread((long)putc_loop, 'H' + cpu_id, cpu_id);
	//} else{
		kthread((long)putc_loop, 'C', cpu_id);
	//}
*/
kthread((long)putc_loop, 'H' + cpu_id, cpu_id);
	// Setup Local Timer.
	set_intr_gate(T_LVT_TIMER, (long)int_lvt_timer);
	lapic_write(LAPIC_LVT_TIMER, (1<<17)|T_LVT_TIMER);
	lapic_write(LAPIC_TICR, get_tsc_diff()/HZ/2);
}

extern void __switch_ctx(long *from, long to);
void do_sched()
{
	struct cpu_struct* cpu = cpu_tab+ me->cpu_id;
	union thread* to=cpu->idle;
	// BUG: last have 2.
	if(cpu->ready != NULL){
		to = cpu->ready;
		cpu->ready = to->next;

		union thread *tmp = cpu->ready;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = to;
		to->next = NULL;
	}

	//union thread*  to = (me == cpu->th1) ? cpu->th2: cpu->th1;
	if(to != me)
		__switch_ctx(&me->stack,to->stack);
}

void do_lvt_timer()
{
	lapic_eoi();
	do_sched();
}
