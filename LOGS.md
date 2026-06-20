#LOGS

Log 1 (06/14): 

I am going to log this as it goes, currently, Keti is just a simple OS, and it just prints the name and my name.

Eventually I am going to spruce up this README and hopefully I can make the OS better. Maybe this is like one 
of those directors cut type note lol.

On a serious note though, what I want KetiOS to look like is pretty far in the future (hopefully not that far) but I want to 
add some games, probably Chess (hopefully I can figure out a way to bring in chess.com as an app?), i have thought of tetris, and maybe more
Probably will make a calculator type setup, but it wont be anything too complicated, something simple and fun. I don't want a windows level GUI but
anything that looks appealing to the eye and is not a black screen that is static, should  be a win.

One thing to note is, rather than running your qasm and nemu lines continuously, just make a MAKEFILE (thank you ECE 264 :>).
Also install the assembly extensions.

Log 2 (06/16):

Up till now I managed to add the link.ld and the loader.asm files, these are basically the entry points to using C and the loader has the CAFEBABE statement going to eax (Thank you Little OS Book!), and basically helps the GRUB load the kernel and continues. Next is the link.ld, so basically this is a map that has the object files you need, like everything you write can be split and the script helps distribute it. The files have all the things mentioned, there is one for read-only variable, unassigned data, assigned data, and all code sections. Little OS Book said that going with 4KB is the strat and the kernel needs to be loaded at a memory address of >= 1MB. Below that is taken by the BIOS and GRUB itself (Seriously, Little OS Book is the GOAT). 

After much debugging and seeing why the code is stuck at read from DVD and CD, (with the help of Claude) we have managed to fix the bootloader and the Keti-OS is in a 32-bit protected mode! The fix was in build/isodir/boot/grub/grub.cfg, the set timeout had to be changed from 0 to 3 so there is some delay before boot. Now that we have the successful bootloader (GRUB), and C can be run!

So far (and Im gonna commit for now), what exists are mentioned above. I am improving the make file to have the run commands.
make just runs everything (i mean the command is there so you can do make run, make clean (make all does run im pretty sure)). 

Also if you make clean and it says any .o file not found, try running 

i686-linux-gnu-gcc -m32 -ffreestanding -c kernel/vga.c -o kernel/vga.o

nvm, fixed.

Log 3 (06/17):

Tiring session today, did a lot of GDT stuff and reading. Basically a lot of seg based stuff. added the assembly file and it is basically simple and self explanatory. I also added the stuff in vga regarding the cursor, so it can handle next line and overflow easily. So in terms of VGA, next would be scrolling. Another thing to note is GDT allows it to have its own memory map and doesn't rely on GRUBs. So Keti can remember stuff, peak.

You can read this stuff in Chapter 5 in the book.

Unfortunately, I am burnt out for the day and I can't see visual progress (which I can't wait for), so Im gonna study for my classes now...yipee :<

Log 4 (06/19):

Insane session today, first I configured the ports and now the keyboard successfully works. So user input works (or my input). Interrupts have also been configured and timer has also been added. A lot of work has been done and notice that phase 3 (or what Claude says is a phase) is complete. 

Now I have to deal with memory management and stuff. I don't know why its being phrased as a boss battle. we will see.