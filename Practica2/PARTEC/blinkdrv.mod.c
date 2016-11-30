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
	{ 0xc6c01fa, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x56adb695, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0x148229db, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0x5f3a3861, __VMLINUX_SYMBOL_STR(usb_deregister_dev) },
	{ 0xfd75cb0, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xf15e6c6b, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x966bacc, __VMLINUX_SYMBOL_STR(usb_register_dev) },
	{ 0xb01f4263, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xce9a8441, __VMLINUX_SYMBOL_STR(usb_control_msg) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x9b142f2d, __VMLINUX_SYMBOL_STR(usb_find_interface) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x69af87a5, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x733c3b54, __VMLINUX_SYMBOL_STR(kasprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v20A0p41E5d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "64C52CB54CF3B3CE32B182B");
