## Ab16 - Design
The design is written by ChatGPT (mostly) as an experiment!

### Memory Layout

| Address Range  | Size    | Description              |
|----------------|---------|--------------------------|
| 0x0000 - 0x0FFF| 4 KB    | BIOS                     |
| 0x1000 - 0x1FFF| 4 KB    | Interrupt Vectors        |
| 0x2000 - 0x2FFF| 4 KB    | I/O Ports                |
| 0x3000 - 0xFFFF| 52 KB   | Program Code and Data    |

### Detailed Description

- **BIOS (0x0000 - 0x0FFF, 4 KB):**
  - This region is reserved for the Basic Input/Output System (BIOS). The BIOS contains the firmware needed to initialize and test hardware components at startup and to provide runtime services for operating systems and programs.

- **Interrupt Vectors (0x1000 - 0x1FFF, 4 KB):**
  - This segment is reserved for storing the addresses of interrupt service routines. Each vector points to a handler routine that manages specific hardware or software interrupts.

- **I/O Ports (0x2000 - 0x2FFF, 4 KB):**
  - This area is dedicated to input/output ports. It allows the CPU to communicate with peripheral devices like keyboards, displays, and other hardware components.

- **Program Code and Data (0x3000 - 0xFFFF, 52 KB):**
  - The remaining memory is allocated for program code and data. This large segment is used for storing the executable code of programs, as well as any associated data needed during execution.

## Ab16 Instruction Set

### Instruction Format

#### Basic Instruction Format

| Bits       | Description                  |
|------------|------------------------------|
| 15 - 12    | Opcode                       |
| 11 - 8     | Destination Register (Rd)    |
| 7 - 4      | Source Register 1 (Rs)       |
| 3 - 0      | Source Register 2/Immediate  |

- **Opcode (4 bits):** Determines the operation to be performed.
- **Destination Register (4 bits):** The register where the result is stored.
- **Source Register 1 (4 bits):** The first operand register.
- **Source Register 2/Immediate (4 bits):** The second operand register or an immediate value.

#### Example Instruction Set

| Opcode | Mnemonic | Description               |
|--------|----------|---------------------------|
| 0000   | NOP      | No Operation              |
| 0001   | MOV      | Move data                 |
| 0010   | ADD      | Add                       |
| 0011   | SUB      | Subtract                  |
| 0100   | AND      | Logical AND               |
| 0101   | OR       | Logical OR                |
| 0110   | XOR      | Logical XOR               |
| 0111   | NOT      | Logical NOT               |
| 1000   | JMP      | Jump to address           |
| 1001   | JZ       | Jump if zero              |
| 1010   | JNZ      | Jump if not zero          |
| 1011   | CALL     | Call subroutine           |
| 1100   | RET      | Return from subroutine    |
| 1101   | PUSH     | Push register onto stack  |
| 1110   | POP      | Pop register from stack   |
| 1111   | LDI      | Load immediate            |

### Instruction Decoding Example

Consider the instruction `ADD R1, R2`.

- Binary representation: `0010 0001 0010 0000`
  - Opcode: `0010` (ADD)
  - Destination Register: `0001` (R1)
  - Source Register 1: `0010` (R2)
  - Source Register 2/Immediate: `0000` (not used in this instruction)

To decode:
1. Extract the opcode (bits 15-12): `0010`
2. Identify the operation: `ADD`
3. Extract the destination register (bits 11-8): `0001` (R1)
4. Extract the source register 1 (bits 7-4): `0010` (R2)

### Interrupt Vector Table (IVT)

#### IVT Entry Format

| Address    | Description                  |
|------------|------------------------------|
| 0x1000     | Interrupt 0 Vector (ISR0)    |
| 0x1002     | Interrupt 1 Vector (ISR1)    |
| 0x1004     | Interrupt 2 Vector (ISR2)    |
| ...        | ...                          |
| 0x1FFE     | Interrupt 1023 Vector (ISR1023) |

#### IVT Entry Format

| Bits       | Description                  |
|------------|------------------------------|
| 15 - 0     | Address of Interrupt Service Routine (ISR) |

### Example Assembly Code

#### BIOS Initialization

```x86asm
ORG 0x0000
START:
    ; Initialize interrupt vectors
    LDI R0, 0x1010       ; Address of Timer ISR
    STM 0x1000, R0       ; Set interrupt vector for Interrupt 0

    LDI R0, 0x1020       ; Address of Keyboard ISR
    STM 0x1002, R0       ; Set interrupt vector for Interrupt 1

    LDI R0, 0x1030       ; Address of I/O ISR
    STM 0x1004, R0       ; Set interrupt vector for Interrupt 2

    JMP MAIN

; Timer Interrupt Service Routine
ORG 0x1010
TIMER_ISR:
    RETI

; Keyboard Interrupt Service Routine
ORG 0x1020
KEYBOARD_ISR:
    RETI

; I/O Interrupt Service Routine
ORG 0x1030
IO_ISR:
    RETI

ORG 0x3000
MAIN:
    JMP MAIN
```

### General Purpose Registers

- **General Purpose Registers**
  - **R0 - R7**: 8 general-purpose 16-bit registers

- **Special Purpose Registers**
  - **PC** (Program Counter): Holds the address of the next instruction
  - **SP** (Stack Pointer): Points to the top of the stack
  - **FLAGS**: Holds the status flags (Zero, Carry, Sign, Overflow)

### Status Flags (in FLAGS register)

| Bit | Flag | Description                               |
|-----|------|-------------------------------------------|
| 0   | Z    | Zero Flag: Set if the result is zero      |
| 1   | C    | Carry Flag: Set if there is a carry       |
| 2   | S    | Sign Flag: Set if the result is negative  |
| 3   | O    | Overflow Flag: Set if there is overflow   |

### Detailed Example: Adding Two Numbers

Let's write an example program to add two numbers and store the result in a memory location.

#### Assembly Code

```x86asm
ORG 0x3000        ; Start of program code

MAIN:
    LDI R0, 0x0030   ; Load immediate value 0x0030 into R0
    LDI R1, 0x0040   ; Load immediate value 0x0040 into R1
    ADD R2, R0, R1   ; Add R0 and R1, store the result in R2
    STM 0x3002, R2   ; Store the result from R2 to memory location 0x3002
    JMP MAIN         ; Infinite loop
```

#### Decoding the Instructions

1. **LDI R0, 0x0030**
   - Binary: `1111 0000 0000 0011 0000`
   - Opcode: `1111` (LDI)
   - Destination Register: `0000` (R0)
   - Immediate: `0000 0011 0000`

2. **LDI R1, 0x0040**
   - Binary: `1111 0001 0000 0100 0000`
   - Opcode: `1111` (LDI)
   - Destination Register: `0001` (R1)
   - Immediate: `0000 0100 0000`

3. **ADD R2, R0, R1**
   - Binary: `0010 0010 0000 0001`
   - Opcode: `0010` (ADD)
   - Destination Register: `0010` (R2)
   - Source Register 1: `0000` (R0)
   - Source Register 2: `0001` (R1)

4. **STM 0x3002, R2**
   - Binary: `MOV` opcode with memory addressing (assuming similar format for memory ops)
   - Opcode: `MOV` 
   - Destination: Memory Address `0x3002`
   - Source Register: `R2`