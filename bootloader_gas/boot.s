.code16
.intel_syntax noprefix
.text
.org 0x0

LOAD_SEGMENT = 0x1000             # load the boot loader to segment 1000h

.global main

main:
  jmp short start                 # jump to beginning of code
  nop

bootsector:
 iOEM:          .ascii "DevOS   "    # OEM String
 iSectSize:     .word  0x200         # bytes per sector
 iClustSize:    .byte  1             # sectors per cluster
 iResSect:      .word  1             # #of reserved sectors
 iFatCnt:       .byte  2             # #of FAT copies
 iRootSize:     .word  224           # size of root directory
 iTotalSect:    .word  2880          # total # of sectors if over 32 MB
 iMedia:        .byte  0xF0          # media Descriptor
 iFatSize:      .word  9             # size of each FAT
 iTrackSect:    .word  9             # sectors per track
 iHeadCnt:      .word  2             # number of read-write heads
 iHiddenSect:   .int   0             # number of hidden sectors
 iSect32:       .int   0             # # sectors for over 32 MB
 iBootDrive:    .byte  0             # holds drive that the boot sector came from
 iReserved:     .byte  0             # reserved, empty
 iBootSign:     .byte  0x29          # extended boot sector signature
 iVolID:        .ascii "seri"        # disk serial
 acVolumeLabel: .ascii "MYVOLUME   " # volume label
 acFSType:      .ascii "FAT16   "    # file system type

.func WriteString
WriteString:
  lodsb                   # load byte at ds:si into al (advancing si)
  or     al, al           # test if character is 0 (end)
  jz     WriteString_done # jump to end if 0.

  mov    ah, 0xe          # Subfunction 0xe of int 10h (video teletype output)
  mov    bx, 9            # Set bh (page nr) to 0, and bl (attribute) to white (9)
  int    0x10             # call BIOS interrupt.

  jmp    WriteString      # Repeat for next character.

WriteString_done:
  retw
.endfunc

.func Reboot
 Reboot:
  lea    si, rebootmsg # Load address of reboot message into si
  call   WriteString   # print the string
  xor    ax, ax        # subfuction 0
  int    0x16          # call bios to wait for key
  .byte  0xEA          # machine language to jump to FFFF:0000 (reboot)
  .word  0x0000
  .word  0xFFFF
.endfunc

start:
  # Setup segments:
  cli
  mov  iBootDrive, dl  # save what drive we booted from (should be 0x0)
  mov  ax, cs          # CS = 0x0, since that's where boot sector is (0x07c00)
  mov  ds, ax          # DS = CS = 0x0
  mov  es, ax          # ES = CS = 0x0
  mov  ss, ax          # SS = CS = 0x0
  mov  sp, 0x7C00      # Stack grows down from offset 0x7C00 toward 0x0000.
  sti

  # Display "loading" message:
  lea  si, loadmsg
  call WriteString

  # Reset disk system.
  # Jump to bootFailure on error.
  mov  dl, iBootDrive  # drive to reset
  xor  ax, ax          # subfunction 0
  int  0x13            # call interrupt 13h
  jc   bootFailure     # display error message if carry set (error)

  # End of loader, for now. Reboot.
  call Reboot

bootFailure:
  lea  si, diskerror
  call WriteString
  call Reboot

# PROGRAM DATA
loadmsg:          .asciz "Loading OS...\r\n"
diskerror:        .asciz "Disk error. "
rebootmsg:        .asciz "Press any key to reboot.\r\n"

.fill (510 - (. - main)), 1, 0  # Pad with nulls up to 510 bytes (excl. boot magic)
BootMagic:  .int 0xAA55     # magic word for BIOS
