/*
    MultiClip - more than one clipboard to cp/paste
    Copyright (C) 2014 Kadir CETINKAYA - Esref OZTURK

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    kadir@kadircetinkaya.me
    esref@esrefozturk.com
*/

#include "multiclip.h"
#include <asm/siginfo.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>

#define SIG_TEST 44

int dev_open(struct inode *inode, struct file *filp);
ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off);
ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off);
char *boardBuf[NBROFBUF+1];
int sizeBuf[NBROFBUF+1];
int pid = 0;
int pressed, press[256], c,v, lctrl, rctrl;

struct file_operations mc_fops =
{
	.write = dev_write,
	.read = dev_read,
	.open = dev_open
};

void sendSignal(int type, int code)
{
	int ret;
	struct siginfo info;
	struct task_struct *t;
	
	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = SIG_TEST;
	info.si_code = SI_QUEUE;
	info.si_int = (code&0xFF) | (type << 8);
	
	rcu_read_lock();
	t = pid_task(find_pid_ns(pid, &init_pid_ns), PIDTYPE_PID);	
	if(t == NULL)
	{
		printk(KERN_DEBUG "[multiclip]: no such pid\n");
		rcu_read_unlock();
		return;
	}
	rcu_read_unlock();
	ret = send_sig_info(SIG_TEST, &info, t);
	if (ret < 0)
		printk(KERN_DEBUG "[multiclip]: error sending signal\n");
}

int kdb_notifier(struct notifier_block* nb, unsigned long code, void* _param)
{
	int i;
	struct keyboard_notifier_param *param = _param;
	
	if(code == KBD_KEYCODE)
	{
		if(param->down)
		{
			switch(param->value)
			{
				case KEY_LEFTCTRL:
					lctrl=1;
					break;
				case KEY_RIGHTCTRL:
					rctrl=1;
					break;
				case KEY_C:
					c = 1;
					break;
				case KEY_V:
					v = 1;
					break;
				default:
					press[param->value] = 1;
					break;
			}
		}
		else
		{
			switch(param->value)
			{
				case KEY_LEFTCTRL:
					lctrl=0;
					break;
				case KEY_RIGHTCTRL:
					rctrl=0;
					break;
				case KEY_C:
					c = 0;
					break;
				case KEY_V:
					v = 0;
					break;
				default:
					press[param->value] = 0;
					break;
			}
		}
	}
	if(param->down && !pressed)
	{
		if(lctrl || rctrl)
		{
			for(i=0;i<256;i++)
				if(press[i])
					break;
			if(i>0)
				sendSignal(0, i);
				
			if(c)
			{
				pressed = 1;

				printk(KERN_DEBUG "[multiclip]: copy %d\n", i);
				sendSignal(0, i);
			}
			if(v)
			{
				pressed = 1;

				printk(KERN_DEBUG "[multiclip]: paste %d\n", i);
				sendSignal(1, i);
			}
		}
	}
	else
		pressed = 0;
	return NOTIFY_OK;
}

int mc_init(void)
{
	int i;
	
	register_chrdev(MLTCLP_MAJOR, "multiclip", &mc_fops);
	register_keyboard_notifier(&nb);
	
	for(i=0;i<=NBROFBUF;i++)
		boardBuf[i] = kmalloc(sizeof(char*)*128, GFP_USER), sizeBuf[i] = 128;
	
	printk(KERN_DEBUG "[multiclip]: loaded\n");
	return 0;
}

void mc_exit(void)
{
	int i;
	unregister_chrdev(MLTCLP_MAJOR, "multiclip");
	
	printk(KERN_DEBUG "[multiclip]: unload-1\n");
    
	unregister_keyboard_notifier(&nb);
	
	printk(KERN_DEBUG "[multiclip]: unload-2\n");
		
	for(i=0;i<=NBROFBUF;i++)
		if(boardBuf[i]!=NULL)
			kfree(boardBuf[i]);
	
	printk(KERN_DEBUG "[multiclip]: unloaded\n");
}

int dev_open(struct inode *inode, struct file *filp)
{
	printk(KERN_DEBUG "[multiclip]: Opening %d\n", MINOR(inode->i_rdev));
	
	filp->private_data = (void*)MINOR(inode->i_rdev);
	return 0;
}

ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	int minor = (int)filp->private_data;
	printk(KERN_DEBUG "[multiclip]: Read from %d with %d bytes, offset:%d\n", minor, len, *off);
	
	if(minor>NBROFBUF)
		return -1;
	
	if(minor == NBROFBUF)
	{
		sprintf(boardBuf[minor], "%d", pid);
		len = strlen(boardBuf[minor]+*off);
		copy_to_user(buff, boardBuf[minor]+*off, len);
		*off += len; 
		return len;
	}
		
	if(*off>=strlen(boardBuf[minor]))
		return 0;
		
	if(len>strlen(boardBuf[minor]+*off))
		len = strlen(boardBuf[minor]+*off);
	
	if(copy_to_user(buff, boardBuf[minor]+*off, len))
	{
		printk(KERN_DEBUG "[multiclip]: Memo Alloc Error :C\n");
		return -1;
	}
	
	*off += len;
	return len;
}

ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	int minor = (int)filp->private_data, i;
	
	printk(KERN_DEBUG "[multiclip]: Write to %d with %d bytes\n", minor, len);
	
	if(minor>NBROFBUF)
		return -1;
		
	if(minor == NBROFBUF)
	{
		sscanf(buff, "%d", &pid);
		return len;
	}
		
	if(len>sizeBuf[minor])
	{
		kfree(boardBuf[minor]);
		boardBuf[minor] = kmalloc(len+5, GFP_USER);
		sizeBuf[minor] = len+2;
	}
	
	for(i=0;i<len;i++)
		boardBuf[minor][i] = buff[i];
	boardBuf[minor][i]=0;
	
	return len;
}

MODULE_LICENSE("GPL");

module_init(mc_init);
module_exit(mc_exit);

