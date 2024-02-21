#!/bin/bash
export MallocStackLogging=1
export MallocStackLoggingNoCompact=1
leaks -atExit -- ./bin/Debug/TestBed

