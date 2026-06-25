# Keti OS - Development Logs


## Log 1 - June 14

I am going to log this as it goes. Currently, Keti is just a simple OS, and it just prints the name and my name.

Eventually I am going to spruce up this README and hopefully I can make the OS better. Maybe this is like one of those director's cut type notes lol.

On a serious note though, what I want Keti OS to look like is pretty far in the future (hopefully not that far) - but here's the vision:

- Some games, probably **Chess** (hopefully I can figure out a way to bring in chess.com as an app?)
- **Tetris**, and maybe more
- A **calculator** type setup - nothing too complicated, something simple and fun

I don't want a Windows-level GUI, but anything that looks appealing to the eye and is not a black screen that is static should be a win.

**Note to self:** Rather than running your `nasm` and `qemu` lines continuously, just make a **Makefile** (thank you ECE 264 :>). Also install the assembly extensions.

---

## Log 2 - June 16

Up till now I managed to add the `link.ld` and the `loader.asm` files. These are basically the entry points to using C - the loader has a statement going to `eax` (thank you Little OS Book!), and basically helps GRUB load the kernel and continue.

Next is `link.ld`. This is a map that has the object files you need - everything you write can be split, and the script helps distribute it. The files have all the things mentioned: one for read-only variables, unassigned data, assigned data, and all code sections. Little OS Book said that going with 4KB is the strat, and the kernel needs to be loaded at a memory address of **≥ 1MB**. Below that is taken by the BIOS and GRUB itself (seriously, Little OS Book is the GOAT).

After much debugging and seeing why the code is stuck at "read from DVD and CD" (with the help of Claude), we have managed to fix the bootloader - and Keti OS is in **32-bit protected mode!** The fix was in `build/isodir/boot/grub/grub.cfg`: the `set timeout` had to be changed from `0` to `3` so there is some delay before boot. Now that we have a successful bootloader (GRUB), C can be run!

So far (and I'm gonna commit for now), what exists is mentioned above. I am improving the Makefile to have the run commands. `make` just runs everything - you can do `make run`, `make clean` (`make all` does run I'm pretty sure).

Also, if you `make clean` and it says any `.o` file not found, try running:

```bash
i686-linux-gnu-gcc -m32 -ffreestanding -c kernel/vga.c -o kernel/vga.o
```

…nvm, fixed.

---

## Log 3 - June 17

Tiring session today - did a lot of GDT stuff and reading. Basically a lot of segmentation-based stuff. Added the assembly file and it is simple and self-explanatory.

I also added the stuff in VGA regarding the cursor, so it can handle next line and overflow easily. So in terms of VGA, next would be **scrolling**.

Another thing to note: the GDT allows Keti to have its own memory map and not rely on GRUB's. So Keti can remember stuff - peak.

You can read this stuff in **Chapter 5** of the book.

Unfortunately, I am burnt out for the day and I can't see visual progress (which I can't wait for), so I'm gonna study for my classes now… yipee :<

---

## Log 4 - June 19

Insane session today. First I configured the ports, and now the keyboard successfully works - so user input works (or my input). Interrupts have also been configured, and the timer has been added.

A lot of work has been done, and notice that **Phase 3** (or what Claude says is a phase) is complete.

Now I have to deal with memory management and stuff. I don't know why it's being phrased as a boss battle. We will see.

---

## Log 5 - June 23

A lot of reading in the past few sessions because of the memory allocation. So what I did so far was make sure `kmalloc` and `kfree` are set up. I commented appropriately as well, so check it out.

I also did the paging and set up the physical memory management. So, big session today.

One thing to note: my logs are getting smaller because I am reaching the end and there is less yap now. So, peak stuff. Also, the files have been neatly ordered and look better now, I feel.

Possible future endeavors:

Shell (Phase 5, now)
Clock display (Phase 5+)
sysinfo command (Phase 5+)
Auth screen (Phase 5+)
RTC / calendar (Phase 6)
Text editor (Phase 6)
Filesystem (Phase 6)
Chess / Tetris (Phase 6)
Screensaver (whenever)
Shutdown command (whenever)
Color themes (whenever)

Future idea as well:
Core OS: shell, filesystem, games (current work)
Keti Language: simple interpreted language, native to the OS with a built-in tutorial command
Keti AI: small inference engine in C, no dependencies tiny model embedded or loaded from disk

---

## Log 6 - June 24

A lot of frontend work today. When I say frontend, nothing flashy. Just adjusting keys to include shift and arrows. Built the shell, commands, and the clock (HH.MM.SS, with timezones and Daylight savings).
When i press up and down arrows, it goes remembers previous commands and stuff.

Tomorrow I will do the backend for the shell, which is user mode, syscall, context and process control. I want to include a proper scroll that can remember previous stuff, and can move up and down. So I will do that eventually. After this, it will mostly be user based stuff, like tetris, chess and stuff. Eventually file manager will come in and networking, and auth ofc.

## Log 7 - June 25

Finally phase 5 is also done. This means keti now has user mode and it has run code in ring 3. So what i did is a lot of reading and coding. Had to change the gdt size to allow more stuff for TSS (task state segment) and I made a new folder process, where it has all that. So now it also allows syscall and do that. Scheduler has also been implemented, so that means it allows context switching and multi-task. Its a multi-turn type setup allowing each process to run for 100ms and saves, goes to the next. Good stuff. Now time to get more things on.

Looks like the chess, tetris and Calculator dream may not be so far off lmao. Well gg, and a very tiring session.

Also for the AI, I had some nice ideas. Maybe instead of having a Search engine like chrome. Maybe the inbuilt AI is the search and it can filter content and provide stuff to you.

Updated phase list:

Phase 6:
  - Scrollback (really Phase 5 polish)
  - VFS layer
  - Ramdisk/initrd  
  - ELF loader
  - Auth system
  - Calculator (quick win)
  - Tetris (first real game)
  - Chess
  - USB flash + send to friends
Phase 7:
  - Graphics mode (Mode 13h)
  - Keti language
  - AI/SLM engine
Phase 8:
  - Networking
  - Full GUI