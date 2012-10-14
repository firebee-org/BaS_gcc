/*
 * last file of the list. In the original sources, this marks the end of the
 * copy loop that copies the BaS to its final place.
 *
 * FIXME: bogus mark of end of code
 *
 * no need to do it that way - this file will probably vanish and be replaced by
 * a matching entry in the linker script.
 */
void copy_end(void)
{
	asm(".globl		copy_end\n\t");
	asm("copy_end:	nop\n\t");
}
