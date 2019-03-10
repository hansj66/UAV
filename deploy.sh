newt install
newt build boot
newt create-image boot 1.0.0
newt load boot
newt build $1 
newt create-image $1 1.0.0
newt load $1 

