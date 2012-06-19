; Boot loader philosophy:
;	ROM loads us to some arbitrary location
;	Move the boot code to the link address (8M)
;	Call decompress_kernel()
;		Relocate the initrd, zimage, and residual data to 8M
;		Decompress the kernel to 0
;	Jump to the kernel entry
;			-- Cort


