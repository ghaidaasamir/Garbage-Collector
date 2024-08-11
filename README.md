# Garbage Collector for Smart Pointers

This project implements a garbage collector using smart pointers in C++. The Pointer class manages dynamic memory with reference counting to automatically handle memory deallocation when it's no longer needed. The garbage collector ensures that memory is freed safely and efficiently, avoiding memory leaks.

## Features

Smart Pointer Management: The Pointer class handles memory allocation and deallocation.

Reference Counting: Automatic reference counting to manage object lifetime.

Garbage Collection: Periodic cleanup of unused memory to prevent leaks.

Array and Single Object Handling: Supports both single objects and dynamically allocated arrays.

## Building

1. Clone the project repository: `git clone https://github.com/ghaidaasamir/Garbage-Collector.git`

``` shell
$ cd garbage-collector
$ ./make
$ ./compiled
```
