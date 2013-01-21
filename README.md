# xnix

A small micro-kernel for meant for learning.  Might find some use for it one day.
Written by Aaron Blakely <aaron@ephasic.org>

## Cross-Compiler

xnix builds with a i586 gcc cross compiler, if you don't have one just run tools/mkcrosscompiler.sh

## Building

*NOTE:* KVM/QUEMU support is dodgy.  I recommend using bochs, or actual hardware.

You'll need to make an initrd, to do so compile tools/mkinitrd/mkinitrd.c and run it, 
then coppy the initrd.img file to the kernel dir.

### Make Targets

| Target   | Descripton                                 |
|:---------|:-------------------------------------------|
| default  | Builds a kernel.bin binary.                |
| test     | Builds and runs in KVM.                    |
| floppy   | Builds and creates floppy image.           |
| bochs    | Creates floppy image and runs in bochs.    |

## Support

Hop in our IRC channel: irc.alphachat.net #ephasic
