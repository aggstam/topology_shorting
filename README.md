This program implements a Topology sorting algorithm, based on Kahn's algorithm [1].
<br>
Two implementations are included, one executing the algorithm in serial, and one using POSIX Threads.
<br>
Directed Acyclic Graph(DAG) is read from an input file created by RandomGraph generator by S.Pettie and V.Ramachandran [2], using the following arguments:
<br>
./RandomGraph directed_grph_<N> <N> 2 1 <N/2>
<br>
where N is the Graph nodes count we want to generate.
<br>
<br>
References:
<br>
[1] https://en.wikipedia.org/wiki/Topological_sorting
<br>
[2] http://www.dis.uniroma1.it/challenge9/code/Randgraph.tar.gz
