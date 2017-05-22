#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/setup.h>

static char proc_cmdline[COMMAND_LINE_SIZE];

static int cmdline_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", proc_cmdline);
	return 0;
}

static int cmdline_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cmdline_proc_show, NULL);
}

static const struct file_operations cmdline_proc_fops = {
	.open		= cmdline_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_cmdline_init(void)
{
	/* SafetyNet bypass: show androidboot.verifiedbootstate=green */
	char *color, *delim;

	color = strstr(saved_command_line, "androidboot.verifiedbootstate=");
	if (color) {
		color = strchr(color, '=');
		delim = strchr(color, ' ');
		if (!delim) {/* last argument on the cmdline */
			delim = "";
		}
		scnprintf(proc_cmdline, COMMAND_LINE_SIZE, "%.*sgreen%s",
			  (int)(color - saved_command_line + 1),
			  saved_command_line, delim);
	} else {
		strncpy(proc_cmdline, saved_command_line, COMMAND_LINE_SIZE);
	}

	proc_create("cmdline", 0, NULL, &cmdline_proc_fops);
	return 0;
}
module_init(proc_cmdline_init);
