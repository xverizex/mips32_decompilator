.data

  space:.asciiz  " "      
  head: .asciiz  "The Fibonacci numbers are:\n"
  fib: .word   0 : 15    
  size: .word  15  
       
.text

main:
	c.le.d $f0, $f2

loop:
	jr $ra
