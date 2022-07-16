# Parallelization of graph algorithms

This project works on the parallelization of graph algorithms. THe following graph algorithms are parallelized :-
- Strongly Connected Components
- Minimum Spanning Tree 

The code to run the serial SCC code is as follows :- 

```
cmake -S . -B _build/
cd _build/
make
cd Strongly_Connected_Components/
./scc 
cd ../../

```

The code to run the serial MST code is as follows :- 

```
cmake -S . -B _build/
cd _build/
make
cd Minimum_Spanning_Tree/
./kruskal 
cd ../../

```
To run the parallel MST code, change the executable from ./kruskal to ./parallel_kruskal
