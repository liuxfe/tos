#ifndef _CNIX_SCHED_H
#define _CNIX_SCHED_H

union thread{
	struct{
		long stack;
		long cpu_id;
		union thread* next; // ready list
		long canarry1;
	};
	struct{
		long dummy[8192/sizeof(long) -1];
		long canarry2;
	};
};
_Static_assert(sizeof(union thread) == 8192, "");

struct cpu_struct{
	union thread *idle;
	union thread *ready;
};

extern struct cpu_struct cpu_tab[NR_CPU_MAX];

static inline union thread* CURTHREAD()
{
	uint64_t rsp;
	__asm__ __volatile__("movq %%rsp, %%rax":"=a"(rsp));
	return (union thread*)(rsp & ~8191ULL);
}

#define me CURTHREAD()

extern union thread* kthread(long func, long arg, long cpu_id);

#endif
