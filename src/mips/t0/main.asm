.data

  space:.asciiz  " "      
  head: .asciiz  "The Fibonacci numbers are:\n"
  fib: .word   0 : 15    
  size: .word  15  
       
.text

main:
	cfc2 $v0, $cc0

loop:
	jr $ra
