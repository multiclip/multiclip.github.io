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

#ifndef multiclip_H
#define multiclip_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>
#include <linux/input.h>
#include <linux/kmod.h>
#include <linux/slab.h>

int mc_init(void);
void mc_exit(void);
int kdb_notifier(struct notifier_block* nb, unsigned long code, void* _param);

struct notifier_block nb =
{
  .notifier_call = kdb_notifier
};

#endif
