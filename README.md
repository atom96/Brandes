Brandes - multithread implementation of Brandes algorithm. </br>

Author: Arkadiusz Tomczak</br>
Version: 1.0</br>
Date: 1-12-2016</br>
Language:
 - Program language: N/A
 - Variable names: English
 - Comments: N/A

About algorithm: http://algo.uni-konstanz.de/publications/b-fabc-01.pdf </br>

Usage:</br>
Compile using Cmake.</br>
Run:</br>
./brandes num-of-threads input-file output-file</br>
</br>
num-of-threads - number of threads which should be used in calculations</br>
</br>
Example input file (graph of networks. Each row is edge between two vertex which should be represented as integers):</br>
```
0 2
2 0
2 3
2 4
3 2
3 5
3 6
```
Output (Betweenness Centrality for each vertex sorted by vertex number):
```
0 0
2 6
3 4
```
