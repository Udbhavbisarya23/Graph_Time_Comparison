# Parallelization of graph algorithms

This project works on the parallelization of graph algorithms. THe following graph algorithms are parallelized :-
- Strongly Connected Components
- Minimum Spanning Tree 

```
cmake -S . -B _build/
cd _build/
make
cd Minimum_Spanning_Tree/
./parallel_kruskal 
cd ../../

```