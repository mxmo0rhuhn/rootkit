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

// Kernel live patch 
//#include <linux/livepatch.h>

int hijack_init(void) {
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
