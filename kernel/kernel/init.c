/* $Id: init.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * First task
 *
 */
#include <keffos/config.h>
#include <keffos/syscall.h>
#include <asm/io.h>

/*
 * fake libc functions
 */
int write(int fd, const void *buf, size_t count)
{
	syscall_parameters_t p;
	int ret;
	int a,b,c,d,e,f,g;
	p.p0 = (void*)buf;
	p.i32_0 = fd; 
	p.size = count;
	//  printk("{\n");
	__asm__("int $0x81"
		: "=a" (ret), "=b"  (b),"=c" (c) ,"=D" (d),"=S" (e)
		: "a" (SYS_write), "d" (&p)
		//	  : "%ebx","%ecx","%esi","%edi"
		
		);
	//  printk("}\n");
	return ret;
}

void ne2k(void)
{
#if defined(CONFIG_MAC) || defined(CONFIG_AMIGA_PCMCIA) || \
    defined(CONFIG_ARIADNE2) || defined(CONFIG_ARIADNE2_MODULE)
#define EI_SHIFT(x)     (ei_local->reg_offset[x])
#else
#define EI_SHIFT(x)     (x)
#endif

#define E8390_CMD       EI_SHIFT(0x00)  /* The command register (for all pages) */
/* Page 0 register offsets. */
#define EN0_CLDALO      EI_SHIFT(0x01)  /* Low byte of current local dma addr  RD */
#define EN0_STARTPG     EI_SHIFT(0x01)  /* Starting page of ring bfr WR */
#define EN0_CLDAHI      EI_SHIFT(0x02)  /* High byte of current local dma addr  RD */
#define EN0_STOPPG      EI_SHIFT(0x02)  /* Ending page +1 of ring bfr WR */
#define EN0_BOUNDARY    EI_SHIFT(0x03)  /* Boundary page of ring bfr RD WR */
#define EN0_TSR         EI_SHIFT(0x04)  /* Transmit status reg RD */
#define EN0_TPSR        EI_SHIFT(0x04)  /* Transmit starting page WR */
#define EN0_NCR         EI_SHIFT(0x05)  /* Number of collision reg RD */
#define EN0_TCNTLO      EI_SHIFT(0x05)  /* Low  byte of tx byte count WR */
#define EN0_FIFO        EI_SHIFT(0x06)  /* FIFO RD */
#define EN0_TCNTHI      EI_SHIFT(0x06)  /* High byte of tx byte count WR */
#define EN0_ISR         EI_SHIFT(0x07)  /* Interrupt status reg RD WR */
#define EN0_CRDALO      EI_SHIFT(0x08)  /* low byte of current remote dma address RD */
#define EN0_RSARLO      EI_SHIFT(0x08)  /* Remote start address reg 0 */
#define EN0_CRDAHI      EI_SHIFT(0x09)  /* high byte, current remote dma address RD */
#define EN0_RSARHI      EI_SHIFT(0x09)  /* Remote start address reg 1 */
#define EN0_RCNTLO      EI_SHIFT(0x0a)  /* Remote byte count reg WR */
#define EN0_RCNTHI      EI_SHIFT(0x0b)  /* Remote byte count reg WR */
#define EN0_RSR         EI_SHIFT(0x0c)  /* rx status reg RD */
#define EN0_RXCR        EI_SHIFT(0x0c)  /* RX configuration reg WR */
#define EN0_TXCR        EI_SHIFT(0x0d)  /* TX configuration reg WR */
#define EN0_COUNTER0    EI_SHIFT(0x0d)  /* Rcv alignment error counter RD */
#define EN0_DCFG        EI_SHIFT(0x0e)  /* Data configuration reg WR */
#define EN0_COUNTER1    EI_SHIFT(0x0e)  /* Rcv CRC error counter RD */
#define EN0_IMR         EI_SHIFT(0x0f)  /* Interrupt mask reg WR */
#define EN0_COUNTER2    EI_SHIFT(0x0f)  /* Rcv missed frame error counter RD */

/*  Register accessed at EN_CMD, the 8390 base addr.  */
#define E8390_STOP      0x01    /* Stop and reset the chip */
#define E8390_START     0x02    /* Start the chip, clear reset */
#define E8390_TRANS     0x04    /* Transmit a frame */
#define E8390_RREAD     0x08    /* Remote read */
#define E8390_RWRITE    0x10    /* Remote write  */
#define E8390_NODMA     0x20    /* Remote DMA */
#define E8390_PAGE0     0x00    /* Select page chip registers */
#define E8390_PAGE1     0x40    /* using the two high-order bits */
#define E8390_PAGE2     0x80    /* Page 3 is invalid. */

/* Bits in EN0_ISR - Interrupt status register */
#define ENISR_RX        0x01    /* Receiver, no error */
#define ENISR_TX        0x02    /* Transmitter, no error */
#define ENISR_RX_ERR    0x04    /* Receiver, with error */
#define ENISR_TX_ERR    0x08    /* Transmitter, with error */
#define ENISR_OVER      0x10    /* Receiver overwrote the ring */
#define ENISR_COUNTERS  0x20    /* Counters need emptying */
#define ENISR_RDC       0x40    /* remote dma complete */
#define ENISR_RESET     0x80    /* Reset completed */
#define ENISR_ALL       0x3f    /* Interrupts we will enable */

#define NE_CMD          0x00
#define NE_DATAPORT     0x10    /* NatSemi-defined port window offset. */
#define NE_RESET        0x1f    /* Issue a read to reset, a write to clear. */
#define NE_IO_EXTENT    0x20

  int regd;
  int ioaddr = 0x300;
  int reg0;
  int t, irq = -1;

  reg0 = inb(ioaddr);
  outb(E8390_NODMA | E8390_PAGE1 | E8390_STOP, ioaddr + E8390_CMD);
  regd = inb(ioaddr + 0x0d);

  outb(0xff, ioaddr + 0x0d);
  outb(E8390_NODMA | E8390_PAGE0, ioaddr + E8390_CMD);
  
  (void)inb(ioaddr + EN0_COUNTER0); /* Clear the counter by reading. */
  if (inb(ioaddr + EN0_COUNTER0) != 0) {
    outb(reg0, ioaddr);
    outb(regd, ioaddr + 0x0d);    /* Restore the old values. */
    printk("ne2k Not detected\n");
    return;
  }
  printk("Detected ne2k at ioaddr=0x%x irq=%d\n", ioaddr, irq);
  
  /* DON'T change these to inb_p/outb_p or reset will fail on clones. */
  outb(inb(ioaddr + NE_RESET), ioaddr + NE_RESET);
  while ((inb(ioaddr + EN0_ISR) & ENISR_RESET) == 0);
  
  outb(0xff, ioaddr + EN0_ISR);         /* Ack all intr. */

}

/*
 * this is supposed to set values to be inherited to
 * all processes and then jump to the loaded init program.
 * So from now on, the code acts like it's in user-mode
 * except for the fact that it's still in pl0.
 * Oh yeah, and we don't have libc. And it's in kernel-memory.
 * Hmm, maybe not that much like user-mode after all.
 */
void init(void)
{
#if 0
	__asm__("cli");
	for(;;);
#else
	int r;
	int c;
	char *str = "Hello world\n";
	printk("init()\n\n");
	for(;;);
	//        12345678901234 5
	write(1, "-----blah-----\n", 15);
	for (c = 0; c < 10; c++) {
		if (0 > (r = write(1, str, strlen(str)))) {
			printk("Error when in write()!\n");
		} else {
			//      printk("syscall returned %d\n", r);
		}
	}
	printk("Text not written was: %s\n", str);
	ne2k();
	for(;;);
#endif
}
