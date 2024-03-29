# topology_shorting
This program implements a Topology sorting algorithm, based on Kahn's algorithm [1].
<br>
Two implementations are included, one executing the algorithm in serial, and one using POSIX Threads.
<br>
Directed Acyclic Graph(DAG) is read from an input file created by RandomGraph generator by S.Pettie and V.Ramachandran [2], using the following arguments:
<br>
./RandomGraph directed_grph_<N> <N> 2 1 <N/2>
<br>
where N is the Graph nodes count we want to generate.

## Usage
Both version can be invocted via the Makefile, or by directly compiling and executing.

### Make usage
#### Normal code
```
% make
```
To include a different input file:
```
% make FILE={file_path}
```
To configure a different output file:
```
% make OUTPUT={file_path}
```

#### Parallel code
```
% make parallel
```
To configure different how many threads to use:
```
% make parallel THREADS={threads_count}
```
To include a different input file:
```
% make parallel FILE={file_path}
```
To configure a different output file:
```
% make parallel OUTPUT={file_path}
```

### Direct usage
#### Normal code
Compilation:
```
% gcc -o topology_shorting topology_shorting.c
```
Execution:
```
% ./topology_shorting {input_file} {output_file}
```

#### Parallel code
Compilation:
```
% gcc -o topology_shorting_parallel topology_shorting_parallel.c
```
Execution:
```
% ./topology_shorting_parallel {threads_count} {input_file} {output_file}
```

## Execution examples
### Normal code
```
❯ make
Executing normal code...
gcc -o topology_shorting topology_shorting.c
./topology_shorting test_file output
Calculating Topology sorting of Graph.
Graph will be retrieved from input file: test_file
Topology Matrix will be written in output file: output
Nodes count: 100
Algorithm started, please wait...
Algorithm finished!
Time spend: 0.000043 secs
Writing Topology Matrix to output file.
Program terminates.
```

### Parallel code
```
❯ make parallel
Executing parallel code...
gcc -o topology_shorting_parallel topology_shorting_parallel.c
./topology_shorting_parallel 4 test_file output
Calculating Topology sorting of Graph.
Threads that will be used: 4
Graph will be retrieved from input file: test_file
Topology Matrix will be written in output file: output
Nodes count: 100
Algorithm started, please wait...
Algorithm finished!
Time spend: 0.001094 secs
Writing Topology Matrix to output file.
Program terminates.
```

## References
[1] https://en.wikipedia.org/wiki/Topological_sorting
<br>
[2] http://www.dis.uniroma1.it/challenge9/code/Randgraph.tar.gz
