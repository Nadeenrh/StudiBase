<div align="center">

<pre>
  __  ___       __     __        __   ___
/__`  |  |  | |  \ | |__)  /\  /__` |__
 .__/  |  \__/ |__/ | |__) /~~\ .__/ |___
                                 Nadeen H
</pre>

<p><strong>A terminal-based student record manager written in C</strong></p>

</div>


StudiBase is a terminal-based student record manager written in C. It stores student names, IDs, and GPAs in plain-text files and lets the user view, search, add, and remove records through an interactive menu.

The project began as a group of smaller experiments with structures, sorting, dynamic memory, and file input/output. Those early trials are preserved in the [`experiments`](experiments) folder, while `studiBase.c` contains the complete program.

## Features

- Open the default `StudentRecords.txt` file or choose another record file
- Create a record file when it does not already exist
- View every student in the selected record
- Search for a student using a unique ID
- Add students with a name, ID, and GPA
- Prevent duplicate student IDs
- Remove a student after confirmation
- Validate menu choices, IDs, names, and GPAs
- Load records into a dynamically allocated array

StudiBase uses York University's 9-point GPA scale, so accepted GPAs range from `0.00` to `9.00`.

## Record format

Each student is saved on one line:

```text
name,id,gpa
```

For example:

```text
Ada Lovelace,1001,8.75
```

A complete fictional record is available in [`examples/sampleRecords.txt`](examples/sampleRecords.txt). Names cannot contain commas because commas separate the fields.

## Build and run

StudiBase requires a C11-compatible compiler such as GCC or Clang.

Using Make:

```sh
make
./studibase
```

Compile manually with GCC:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic studiBase.c -o studibase
./studibase
```

On Windows, run `studibase.exe` instead of `./studibase`.

To remove the compiled program:

```sh
make clean
```

## Project structure

```text
StudiBase/
|-- studiBase.c              # Complete program
|-- Makefile                 # Build commands
|-- examples/
|   `-- sampleRecords.txt    # Fictional sample data
`-- experiments/             # Earlier learning trials and prototype
```

## Technical details

The program uses:

- C structures for student data
- `malloc`, `realloc`, and `free` for dynamic memory management
- Standard file I/O for persistent text records
- Line-based input with numeric and length validation
- Linear search by student ID
- Array shifting and file rewriting for record removal

Malformed rows are skipped when a record file is loaded, which prevents one bad line from stopping access to the remaining valid records.
