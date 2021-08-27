/* $Id: syscall.c,v 1.2 2001/05/31 21:32:22 marvin Exp $
 *
 * KeffOS
 *
 * Syscalls
 *
 */
#include <keffos/config.h>
#include <keffos/errno.h>
#include <keffos/limits.h>
#include <keffos/syscall.h>

/*
 * demultiplexer
 */
int syscall(int function, syscall_parameters_t *parms)
{
	//  printd(1, "syscall(%d, 0x%x)\n", function, parms);
	return; // bug out FIXME
	switch (function) {
	case SYS_test:
		printk("%s", (char*)parms->p0);
		return 0;
	case SYS_shutdown:
		return sys_shutdown(parms->i32_0);
	case SYS_read:
		return sys_read(parms->i32_0, parms->p0, parms->size);
	case SYS_write:
		return sys_write(parms->i32_0, parms->p0, parms->size);
	case SYS_mount:
	case SYS_umount:
	case SYS_open:
	case SYS_close:
	case SYS_chmod:
	case SYS_chown:
	case SYS_sleep:
		return -(errno = ENOSYS);
	default:
		printk("Warning: unknown syscall\n");
		return -(errno = ENOSYS);
	}
}

int sys_read(int fd, void *buf, size_t count)
{
	if (!fd) { /* stdin */
		/* block */
		for(;;);
	}
	/*
	 * assume fd does not exist
	 */
	return -(errno = EBADF);
}

int sys_write(int fd, const void *buf, size_t count)
{
	if (fd == 1) { /* stdout */
		/* block */
		printk("%s", buf);
	} else {
		/*
		 * assume fd does not exist
		 */
		return -(errno = EBADF);
	}
	return count;
}


/*
 * reboot_hard, reboot_soft, halt, power_off, 
 */
int sys_shutdown(int parm)
{
	return -(errno = ENOSYS);
}

int sys_chmod(const char *path, int mode)
{
	return -(errno = ENOSYS);
}

int sys_chown(const char *path, uid_t uid, gid_t gid)
{
	return -(errno = ENOSYS);
}

int sys_setpriority(pid_t pid, int priority)
{
	return -(errno = ENOSYS);
}

int sys_getpriority(pid_t pid)
{
	return -(errno = ENOSYS);
}

int sys_time(void *store_here)
{
	return -(errno = ENOSYS);
}

int sys_sethostname(const char *name, int len)
{
	return -(errno = ENOSYS);
}

int sys_gethostname(const char *name, int len)
{
	strncpy(name, "keffo", len);
	return 0;
}

int sys_setdomainname(const char *name, int len)
{
	return -(errno = ENOSYS);
}

int sys_getdomainname(const char *name, int len)
{
	strncpy(name, "testnet", len);
	return 0;
}

int sys_umask(int mode)
{
	return -(errno = ENOSYS);
}

int sys_open(const char *path, int flags)
{
	return -(errno = ENOSYS);
}

int sys_create(const char *path, int mode)
{
	return -(errno = ENOSYS);
}

int sys_close(int fd)
{
	return -(errno = ENOSYS);
}

int sys_uname(void *store_here)
{
}
