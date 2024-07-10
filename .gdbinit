set confirm off
target remote localhost:26000
set disassemble-next-line on
set pagination off
break kmain
c