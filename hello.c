/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#include <linux/slab.h>

#include <linux/ktime.h>

struct my_list_head {
struct my_list_head *next;
ktime_t time;
};


MODULE_AUTHOR("Trandashyr Julia");
MODULE_DESCRIPTION("Hello, world");
MODULE_LICENSE("Dual BSD/GPL");

static struct my_list_head *header;


static uint times;
module_param(times, uint, 0444);
MODULE_PARM_DESC(times, "The amount of times to print hello world");

static int __init hello(void)
{
uint i;

struct my_list_head *first_var, *second_var;
pr_info("times: %d\n", times);

if (times == 0) {
	pr_warn("The parameter is 0 \n");
} else if (times >= 5 && times <= 10) {
	pr_warn("5 <= param <= 10 \n");
} 
BUG_ON (times > 10);


header = kmalloc(sizeof(struct my_list_head *), GFP_KERNEL);
first_var = header;

for (i = 0; i < times; i++) {
	if ( i == 7){ first_var = NULL; }
	first_var->next = kmalloc(sizeof(struct my_list_head), GFP_KERNEL);
	first_var->time = ktime_get();
	pr_info("Hello World!\n");
	second_var = first_var;
	first_var = first_var->next;
	
}
if (times != 0) {
	kfree(second_var->next);
	second_var->next = NULL;
}

return 0;
}


static void __exit hello_exit(void)
{
struct my_list_head *var;

while (header != NULL && times != 0) {
	var = header;
	pr_info("Time : %lld", var->time);
	header = var->next;
	kfree(var);
}

if (times == 0)
	pr_info("Time is 0 because print wasn't used\n");

pr_info("");
}


module_init(hello);
module_exit(hello_exit);
