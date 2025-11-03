# AI Coding Assistant Instructions for "The Boys" Project

## Project Overview

This is a C-based simulation system modeling a world of superheroes, bases, and missions. The project implements discrete event simulation using priority queues and generic data structures.

## Core Architecture

### Key Components

1. **World Structure** (`mundo.h`)
   - Central data structure containing heroes, bases, missions
   - Fixed-size arrays for entities (MAXH=10 heroes, MAXB=4 bases, MAXM=10 missions)
   - Coordinates system for spatial positioning
   - Global clock for simulation timing

2. **Event System** (`eventos.h`)
   - Uses Lista de Eventos Futuros (LEF) - a priority queue for event scheduling
   - Events include: hero arrivals, waiting, entering/exiting bases, missions, deaths
   - Each event has: time, hero, base, and function pointer to handler

3. **Data Structures**
   - Priority Queue (`fprio.h`): Generic implementation for event scheduling
   - Queue (`fila.h`): Used for base waiting lines
   - Set (`conjunto.h`): Used for hero abilities and mission requirements

## Development Workflows

### Build System
```bash
make        # Build the project
make run    # Build and run
make valgrind # Run with memory checker
make tgz    # Package for submission
```

### Memory Management Conventions
- All data structures have create/destroy functions
- Structures use void pointers for generic item storage
- Manual memory management required - no garbage collection

## Key Patterns

1. **Event Handling**
   - Events are scheduled using `fprio_insere` into LEF
   - Event handlers follow pattern: `void handler(int time, struct heroi* h, struct base* b)`

2. **Entity Management**
   - Heroes have: ID, patience, speed, experience, abilities
   - Bases have: capacity, present heroes set, waiting queue
   - Missions have: requirements set, location, status

## Integration Points

1. **Event-World Interaction**
   - Events modify world state through passed entity pointers
   - World clock advances based on event timestamps

2. **Spatial System**
   - All entities (bases, heroes, missions) use coordinate system
   - Movement/travel times calculated based on hero speed and distances

## Common Pitfalls

1. **Memory Safety**
   - Always check return values from create/insert operations
   - Free all allocated memory in correct order during cleanup
   - Use Valgrind regularly to check for leaks

2. **Event Ordering**
   - Events must be processed in strict time order
   - Multiple events can occur at same timestamp - handle ordering carefully