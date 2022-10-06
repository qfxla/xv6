#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void) {
    int n;
    if (argint(0, &n) < 0)
        return -1;
    exit(n);
    return 0;  // not reached
}

uint64
sys_getpid(void) {
    return myproc()->pid;
}

uint64
sys_fork(void) {
    return fork();
}

uint64
sys_wait(void) {
    uint64 p;
    if (argaddr(0, &p) < 0)
        return -1;
    return wait(p);
}

uint64
sys_sbrk(void) {
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;

    addr = myproc()->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

uint64
sys_sleep(void) {
    int n;
    uint ticks0;


    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (myproc()->killed) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}


//#ifdef LAB_PGTBL
int
sys_pgaccess(void) {
    // lab pgtbl: your code here.
    uint64 addr;
    int len;
    int bitmask;
    if (argaddr(0, &addr) < 0) {   // 内核函数取调用者的参数方法就是这样
        return -1;
    }
    if (argint(1, &len) < 0) {
        return -1;
    }
    if (argint(2, &bitmask) < 0) {
        return -1;
    }
    if (len > 32 || len < 0) {
        return -1;
    }

    /**
     * 核心代码
     * 用int va = addr + i * PGSIZE表示某个页表的第i个的虚拟地址
     * 使用vm_pgaccess去找这个虚拟地址对应的标志位
     * 用res | abit << i把标志位加到res中对应i的位置
     * 最后再把res拷贝到bitmap中，返回给调用的用户进程
     */
    int res = 0;
    struct proc *p = myproc();
    for (int i = 0;i < len;i++) {
        int va = addr + i * PGSIZE;
        int abit = vm_pgaccess(p->pagetable, va);
        res = res | abit << i;
    }
    if (copyout(p->pagetable, bitmask, (char *)&res, sizeof(res)) < 0) {  // Copy from kernel to user.
        return -1;
    }
    return 0;
}
//#endif

uint64
sys_kill(void) {
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void) {
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}
