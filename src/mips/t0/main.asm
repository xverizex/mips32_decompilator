.data

  space:.asciiz  " "      
  head: .asciiz  "The Fibonacci numbers are:\n"
  fib: .word   0 : 15    
  size: .word  15  
       
.text

main:
	cfc1 $t0, $f0

loop:
	jr $ra
