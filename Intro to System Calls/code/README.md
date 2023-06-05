# INTERNATIONAL INSTITUTE OF INFORMATION TECHNOLOGY HYDERABAD
## ASSIGNMENT - 1
## Intro To System Calls
## Rhythm Aggarwal, 2021101081

Operating System Used - MacOS

Shell Used - zsh

### QUESTION - 1
#### Assumptions
* it is assumed that the input format will consist of file path in the command line, otherwise the program would terminate giving error
#### Compilation and Execution 
```
$ gcc q1.c
$ ./a.out <input_file_path>
```
---
### QUESTION - 2
#### Assumptions
* If the input file doesn't exist, the program will terminate showing an error message.
* The output directory is being made in the directory in which the input file is being present.
* Zero-indexing has been assumed and the offsets are assumed to be part of the middle portion
* The starting and the ending offsets must be distinct
* the ending index must be less than the file size
#### Compilation and Execution
```
$ gcc q2.c
$ ./a.out <input_file_path> <offset for the mid-portion start> <offset for the mid-portion end>
```
---
### QUESTION - 3
#### Assumptions
* If any of the three, the newfile, the oldfile or the directory doesn't exist, the program will terminate immediately at the point till it has run, displaying an error message
* first it will check if the directory exists, then the program will tell if the old and the newfile are reverse of each other, afterwards the permissions will be displayed.
* Firstly, the permissions of the oldfile will be displayed, then for the newfile, and later for the directory. 

#### Compilation and Execution
``` 
$ gcc q3.c
$ ./a.out <newfile_path> <oldfile_path> <directory_path>
```
---
