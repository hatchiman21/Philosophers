# Philosophers

## Personal Introduction
I am thrilled to share this implementation of the Dining Philosophers problem. It took countless hours of careful planning, testing, and deep dives into concurrency pitfalls to ensure a stable and correct solution.
I crafted this project to explore practical concurrency challenges, specifically addressing issues like race conditions, proper thread/process synchronization, and memory safety.

## Project Overview
• Explores threading (in the mandatory part) and processes (in the bonus part).  
• Manages concurrency using mutexes (mandatory) and semaphores (bonus).  
• Demonstrates safe fork usage and resource sharing.
This project delves into both a thread-based (mandatory) and a process-based (bonus) approach to the Dining Philosophers problem, focusing heavily on robust error handling, precise timing constraints, and clean resource deallocation.

## How to Build
1. Navigate to the philo directory:  
   • Run "make" to compile the mandatory version.  
   • The resulting executable is "philo".  
2. Navigate to the philo_bonus directory:  
   • Run "make" to compile the bonus version.  
   • The resulting executable is "philo_bonus".

## Usage
• Mandatory:
  ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]
• Bonus:
  ./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [times_must_eat]

## Rules
• Each philosopher will pick up two forks (left and right) before eating.  
• If a philosopher fails to eat within the specified time_to_die, they starve.  
• If number_of_times_each_philosopher_must_eat is provided, the simulation ends once all have eaten enough times.

## Notes
• All code is in C, and runs on most UNIX-like systems.  
• No global variables are allowed.  
• Mutexes or semaphores are used to avoid race conditions.

Enjoy exploring concurrency with the Philosophers project!
