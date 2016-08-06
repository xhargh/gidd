### Generate Include Dependency Diagram - gidd

##### Warning: This is currently a quick'n'dirty hack to view dependencies.

The gidd tool will look at c/c++ include file dependencies and generate graphviz .dot file 
and PlantUml diagram to visualize the dependency.

#### Howto

To enable, add -H to gcc/g++. When using cmake, add this to your CMakeLists.txt:
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -H")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H")
```

All includes in a cmake project:
```
cmake .. && make clean && make -j1 &> input.txt
```

All includes for one file in a project
```
touch ../src/file.cpp
cmake .. && make -j1 &> input.txt
```

How to convert the dot file to ps
```
dot -Tps output.dot > output.ps
# or
neato -Tps output.dot > output.ps
```
and then to convert to jpg
```
convert output.ps output.jpg
```

#### Filter include directories
If you are not interested in a set of include folders (normally the system or default),
you can add a filter.txt in the same folder as input.txt where you can filter out sertain folders.
The easiest way of getting a list of include folders is to run gidd once and gidd will print all used folders.
Then you can copy/paste from that list into the filter.txt file and run gidd again.