.code16
.intel_syntax noprefix
.text
.org 0x0

LOAD_SEGMENT = 0x1000             # load the boot loader to segment 1000h

.global my_main

my_main:
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
 iTrackSect:    .word  18            # sectors per track
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
  pusha
WriteString_loop:
  lodsb                   # load byte at ds:si into al (advancing si)
  or     al, al           # test if character is 0 (end)
  jz     WriteString_done # jump to end if 0.

  mov    ah, 0xe          # Subfunction 0xe of int 10h (video teletype output)
  mov    bx, 9            # Set bh (page nr) to 0, and bl (attribute) to white (9)
  int    0x10             # call BIOS interrupt.

  jmp    WriteString_loop      # Repeat for next character.

WriteString_done:
  popa
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
  mov  bp, 0x7C00
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

  # Read sector 1 and print it.
  mov ax, 1
  lea bx, sectorMsg
  call ReadSector
  lea si, sectorMsg
  call WriteString

  mov ax, 2
  lea bx, sectorMsg
  call ReadSector
  lea si, sectorMsg
  call WriteString

  # End of loader, for now. Reboot.
  call Reboot

bootFailure:
  lea  si, diskerror
  call WriteString
  call Reboot

# PROGRAM DATA
debugmsg:         .asciz "Debug...\r\n"
loadmsg:          .asciz "\r\n\r\n\r\n\r\nLoading OS11...\r\n"
diskerror:        .asciz "Disk error. "
diskreaderror:        .asciz "Disk read error. "
rebootmsg:        .asciz "Press any key to reboot.\r\n"

# Read sector with logical address (LBA) AX into data
# buffer at ES:BX. This function uses interrupt 13h, subfunction ah=2.
.func ReadSectorLBA2
ReadSectorLBA2:

  mov     ax, 0x0201                  # Subfunction 2, read 1 sector
  mov     dl, iBootDrive              # from this drive
  mov     cx, 2                       # (cylinder 0, sector 1)
  mov     dh, 0                       # Head 0
  int     0x13
  jc      ReadSectorLBA2_fail
  jmp     ReadSectorLBA2_end

ReadSectorLBA2_fail:
  lea  si, diskreaderror
  call WriteString

ReadSectorLBA2_end:
  ret
.endfunc


# Read sector with logical address (LBA) AX into data
# buffer at ES:BX. This function uses interrupt 13h, subfunction ah=2.
.func ReadSector
ReadSector:
  xor     cx, cx                      # Set try count = 0

 readsect:
  push    ax                          # Store logical block
  push    cx                          # Store try number
  push    bx                          # Store data buffer offset

  # Calculate cylinder, head and sector:
  # Cylinder = (LBA / SectorsPerTrack) / NumHeads
  # Sector   = (LBA mod SectorsPerTrack) + 1
  # Head     = (LBA / SectorsPerTrack) mod NumHeads

  mov     bx, iTrackSect              # Get sectors per track
  xor     dx, dx
  div     bx                          # Divide (dx:ax/bx to ax,dx)
                                      # Quotient (ax) =  LBA / SectorsPerTrack
                                      # Remainder (dx) = LBA mod SectorsPerTrack
  inc     dx                          # Add 1 to remainder, since sector
  mov     cl, dl                      # Store result in cl for int 13h call.

  mov     bx, iHeadCnt                # Get number of heads
  xor     dx, dx
  div     bx                          # Divide (dx:ax/bx to ax,dx)
                                      # Quotient (ax) = Cylinder
                                      # Remainder (dx) = head
  mov     ch, al                      # ch = cylinder
  xchg    dl, dh                      # dh = head number

  # Call interrupt 0x13, subfunction 2 to actually
  # read the sector.
  # al = number of sectors
  # ah = subfunction 2
  # cx = sector number
  # dh = head number
  # dl = drive number
  # es:bx = data buffer
  # If it fails, the carry flag will be set.
  mov     ax, 0x0201                  # Subfunction 2, read 1 sector
  mov     dl, iBootDrive              # from this drive
  pop     bx                          # Restore data buffer offset.
  int     0x13
  jc      readfail

  # On success, return to caller.
  pop     cx                          # Discard try number
  pop     ax                          # Get logical block from stack
  ret

  # The read has failed.
  # We will retry four times total, then jump to boot failure.
 readfail:
  pop     cx                          # Get try number
  inc     cx                          # Next try
  cmp     cx, word ptr 4              # Stop at 4 tries
  je      bootFailure

  # Reset the disk system:
  xor     ax, ax
  int     0x13

  # Get logical block from stack and retry.
  pop     ax
  jmp     readsect
.endfunc

.fill (510 - (. - my_main)), 1, 0  # Pad with nulls up to 510 bytes (excl. boot magic)
BootMagic:  .short 0xAA55     # magic word for BIOS

# Sector 1
sectorMsg:          .asciz "This is a message from sector 1.\r\n"

.fill (1024 - (. - my_main)), 1, 0  # Pad with nulls up to 1024 bytes

# Sector 1
sectorMsg2:          .asciz "This is a message from sector 2.\r\n"