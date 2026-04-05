# uab-fp2-robot-simulator
Event-driven robot simulator written in C. Practical project 1 for the Fundamentals of Programming 2 subject (AI Degree) at Universitat Autònoma de Barcelona.

# Overview
This repository contains a simple, event-based simulator designed to automate logistics and household tasks. It is built entirely in C to practice fundamental data structures, program arguments, and dynamic memory allocation. The simulation is strictly driven by command-line arguments that dictate the number of events to process.

# Key Features
* **Package Management**: Iterates through a maintained list to sort incoming packages by their supplier.
* **Package Classification**: Segregates small, medium, and large packages, pushing them into separate stacks until maximum capacity is reached.
* **Shopping Automation**: Manages a queue of robots that wait for their turn to process a shopping list, with time passing relative to consumed simulation events.

# Technical Details
* **Language**: C
* **Memory**: Dynamic memory management using malloc and free to prevent memory leaks.
* **Data Structures**: Custom implementation of linked lists, stacks, and queues.

# Usage
Compile the program and pass the number of events as an argument.

'''c
./program <number_of_events>
'''

### author note
This README was mostly redacted by an LLM. The code is entirely human-made.