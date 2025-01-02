# 16-bit CPU Project  

## Overview  
This project is a custom-designed **16-bit CPU**, developed as part of my university coursework. It adheres to specific architectural and functional requirements, offering insights into CPU design and implementation, including data flow, instruction handling, and operational efficiency.  

## Features  
- **Memory Architecture**:  
  - 4 x 16-bit RAM blocks with 5-bit addressing.  
  - 3 MARs (Memory Address Registers) and 3 MBRs (Memory Buffer Registers).  
  - A ROM-based Program Control Unit (PCU).  

- **Instruction Set**:  
  - Implements 19 instructions, including arithmetic, logic, and control operations.  
  - Special instructions like matrix multiplication (Mat Mult), block load/store, and memory debugging.  

- **Data Flow**:  
  - Data transitions strictly follow architectural guidelines, ensuring proper routing through MAR, MBR, ALU, and registers.  
  - A bus terminal facilitates data movement across RAM blocks.  

- **ALU**:  
  - Supports 12-16 operations, including addition, subtraction, multiplication, bitwise operations (AND, XOR, NOT), and comparison.  

## Key Design Highlights  
1. **Core Components**:  
   - 8 x 16-bit input registers (organized into two banks).  
   - 1 x 16-bit output register.  
   - 3 buses for efficient data communication.  
   - 1 bus terminus for RAM block intercommunication.  

2. **Execution**:  
   - Automated execution of 12 core operations.  
   - Designed to pass 3 out of 4 predefined assembly test scripts without modifications.  

3. **Mandatory Instructions**:  
   - Matrix Multiplication (Mat Mult).  
   - Block Load and Block Store.  

## Instruction Set Summary  
- Arithmetic: ADD, SUB, MULT.  
- Logic: AND, XOR, NOT.  
- Memory: LOAD, STORE, BLOCK LOAD, BLOCK STORE.  
- Control: NOP (No Operation), HALT.  
- Debugging: DEBUG (adds a delay).  

## Project Goals Achieved  
- Designed a fully functional 16-bit CPU adhering to strict academic guidelines.  
- Implemented a ROM-based PCU instead of a digital circuit-based PCU.  
- Ensured the architecture is extensible for future enhancements.  

