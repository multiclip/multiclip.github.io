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

static char lctrl=0, rctrl=0, c=0, v=0, pressed=0;
static char press[256] = {0};
static char * envp[] = { "HOME=/", NULL };
static char * argv[] = { "/bin/multiclip", NULL, NULL, NULL };

char *itoa(int x)
{
	int i;
	char *a = kmalloc(10, GFP_USER);
	
	for(i=0;i<10;i++)
		a[i] = 0;
	
	i=0;
	while(x)
	{
		a[i++] = x%10 + '0';
		x/=10;
	}
	x=i-1;
	for(i=0;i*2<x;i++)
	{
		char t = a[i];
		a[i] = a[x-i];
		a[x-i] = t;
	}
	return a;
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
			if(c)
			{
				pressed = 1;
				printk(KERN_DEBUG "[multiclip]: copy\n");
				argv[1] = "COPY";
				if(argv[2]!=NULL)
					kfree(argv[2]);
				argv[2] = NULL;
				for(i=0;i<256;i++)
					if(press[i])
					{
						argv[2] = itoa(i);
						break;
					}
				printk(KERN_DEBUG "[multiclip]: %s %d\n", argv[2], i);
				call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
			}
			if(v)
			{
				pressed = 1;
				printk(KERN_DEBUG "[multiclip]: paste\n");
				argv[1] = "PASTE";
				if(argv[2]!=NULL)
					kfree(argv[2]);
				argv[2] = NULL;
				for(i=0;i<256;i++)
					if(press[i])
					{
						argv[2] = itoa(i);
						break;
					}
				printk(KERN_DEBUG "%s %d\n", argv[2], i);
				call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
			}
		}
	}
	else
		pressed = 0;
	return NOTIFY_OK;
}

int mc_init(void)
{
	register_keyboard_notifier(&nb);
	
	printk(KERN_DEBUG "[multiclip]: loaded\n");
	return 0;
}

void mc_exit(void)
{
	unregister_keyboard_notifier(&nb);
	if(argv[2]!=NULL)
		kfree(argv[2]);	
	
	printk(KERN_DEBUG "[multiclip]: unloaded\n");
}

MODULE_LICENSE("GPL");

module_init(mc_init);
module_exit(mc_exit);

