.data

  space:.asciiz  " "      
  head: .asciiz  "The Fibonacci numbers are:\n"
  fib: .word   0 : 15    
  size: .word  15  
       
.text

main:
	cache 0x3, 0xa($sp)

loop:
	jr $ra
