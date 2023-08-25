# heapster 

- there is simple UAF in "Add node" and "Del node" 
- And there is OOB read in validation().
- Attack is able to get stack address and libc address using memory leak oracle. 
- And calcuate one_shot gadget. 
- And There are another Heap memory leak in print() method. 
- You can read the safelink address via memory leak. 
- And decrpyt safelink and calcuation fd pointer.
- An attack overwrite return address to one_shot gadget for control flow hijecking.

