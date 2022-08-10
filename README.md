# Parallelization of graph algorithms

This project works on the parallelization of graph algorithms. THe following graph algorithms are parallelized :-
- Strongly Connected Components
- Minimum Spanning Tree 

```
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B _build/
cd _build/
make
cd Strongly_Connected_Components/
./scc 
cd ../../

```