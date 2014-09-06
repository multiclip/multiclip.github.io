#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xca05c877, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x9ed554b3, __VMLINUX_SYMBOL_STR(unregister_keyboard_notifier) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x25563496, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x2f7c36b3, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x96554810, __VMLINUX_SYMBOL_STR(register_keyboard_notifier) },
	{ 0x7ee0d724, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x52963671, __VMLINUX_SYMBOL_STR(send_sig_info) },
	{ 0x760b04b6, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0xbee9e8f8, __VMLINUX_SYMBOL_STR(find_pid_ns) },
	{ 0x5ba2c23c, __VMLINUX_SYMBOL_STR(init_pid_ns) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "70D8DAD28A06A7D50591847");
