#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_waitx(void)
{
  uint64 p, raddr, waddr;
  int rtime, wtime;
  argaddr(0, &p);
  argaddr(1, &raddr);
  argaddr(2, &waddr);
  int ret = waitx(p,&rtime,&wtime);
  struct proc *proc = myproc();
  if (copyout(proc->pagetable, raddr, (char*)&rtime , sizeof(int)) < 0)
    return -1;
  if (copyout(proc->pagetable, waddr, (char*)&wtime , sizeof(int)) < 0)
    return -1;
  return ret;
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  int arg_no;
  argint(0,&arg_no);
  myproc()->msk_trace = arg_no;
  return myproc()->trapframe->a0;
}

uint64
sys_sigalarm(void)
{
  int ticks;
  argint(0,&ticks);
  myproc()->p_ticks = ticks;
  myproc()->current_ticks = 0;
  myproc()->issigalarm = 1;
  uint64 handler;
  argaddr(1,&handler);
  myproc()->handler = handler;
  return myproc()->trapframe->a0;
}

uint64
sys_sigreturn(void)
{
  struct proc* p = myproc();
  (p->copy_trapframe->kernel_hartid) = (p->trapframe->kernel_hartid);
  (p->copy_trapframe->kernel_satp) = (p->trapframe->kernel_satp);
  (p->copy_trapframe->kernel_sp) = (p->trapframe->kernel_sp);
  (p->copy_trapframe->kernel_trap) = (p->trapframe->kernel_trap);
  *(p->trapframe) = *(p->copy_trapframe);
  p->issigalarm = 1;
  p->current_ticks = 0;
  return p->trapframe->a0;
}

uint64
sys_settickets(void)
{
  int tickets;
  argint(0,&tickets);
  myproc()->tickets = tickets;
  lbs_last_ticket +=tickets-1;
  return myproc()->trapframe->a0;
}

extern struct proc proc[NPROC];
uint64
sys_set_priority(void)
{
  int ret = 0;
  struct proc* p;
  int pid;
  argint(1,&pid);
  int new_prio;
  argint(0,&new_prio);
  for(p = proc; p < &proc[NPROC]; p++)
  {
    if(p->pid == pid){
      ret = p->static_prio;
      p->static_prio = new_prio;
      p->niceness = 5;
      break;
    }
  }
  if(ret > p->static_prio) yield();
  return ret;
}
