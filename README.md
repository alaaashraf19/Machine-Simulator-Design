# Machine-Simulator-Design

This code provides a model and a design simulator for the Vole machine and its language that is capable of simulating its operation and running the program.


The simulator will:
1- Offer a menu of choices
2- Allow the user to load a new program from a file
3- Fetch instructions a step by step to IR and validate it is a valid step and execute it
4- Allow the user to display the status of the registers, PC, IR, memory and screen at the end of
program execution or after each step in a suitable format. (In text format)


Data is taken as a file of space separated hex numbers representing the instructions, e.g. 0x1 0x0 0xFF means load R0 with memory content in location 255d.
