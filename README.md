# Rectilinear-Steiner-Tree

This is a university project on Rectilinear Steiner Trees (RST) 

An implemented algorithm uses **Parallel Batched 1-Steiner** heuristic to construct RST. Required Minimum Spanning Trees (MST) are found using either [Prim's algorithm](https://en.wikipedia.org/wiki/Prim%27s_algorithm) or **Linear-time Dynamic MST Maintenance**.

Check the [Literature](#literature) for more details on algorithms.

## Requirements
*   [Cmake](https://cmake.org/) >= 3.12
*   [Boost](https://www.boost.org/) >= 1.67.0
*   C++ Compiler supporting C++17 standard

## Workflow
#### Clone
*   Check that your environment meets all the requirements
*   Clone the repository: `git clone https://github.com/denislos/Rectilinear-Steiner-Tree.git`

#### Build

```bash
mkdir build && cd build
cmake .. && make
```

#### Run
*   Run all the tests via `scripts/run_tests.sh` on Linux machine
*   Run for a single XML input file: `./steiner <input file>.xml`. Output file `<input_file>_out.xml` will be created in the same directory as the input file.

## File formats
Examples of input and output files can be found [here](examples/).

#### Input file format
XML input file should have the following format:

```XML
<root>
<grid min_x="<Min X>" max_x="<Max X>" min_y="<Min Y>" max_y="<Max X>" />
<net>
<point x="<x1 coordinate>" y="<y1 coordinate>" layer="pins" />
<point x="<x2 coordinate>" y="<y2 coordinate>" layer="pins" />
...
<point x="<xN coordinate>" y="<yN coordinate>" layer="pins" />
</net>
</root>
```

#### Output file format
XML output file has the following format:

```XML
<root>
<grid min_x="<Min X>" max_x="<Max X>" min_y="<Min Y>" max_y="<Max X>" />
<net>
  
# Pins
<point x="<x1 coordinate>" y="<y1 coordinate>" layer="pins" />
<point x="<x2 coordinate>" y="<y2 coordinate>" layer="pins" />
...
<point x="<xN coordinate>" y="<yN coordinate>" layer="pins" />

# Vias points having the following layers: pins_m2, m2, m2_m3
<point x="<vx coordinate>" y="<vy coordinate>" layer="m2" />
<point x="<vx coordinate>" y="<vy coordinate>" layer="pins_m2" />
...
<point x="<vxK coordinate>" y="<vyK coordinate>" layer="m2_m3" />

# Horizontal segments (layer m2)
<segment x1="<x1 coordinate>" y1="<y1 coordinate>" x2="<x2 coordinate>" y2="<y2 coordinate>" layer="m2" />
...
  
# Vertical segments (layer m3)
<segment x1="<x1 coordinate>" y1="<y1 coordinate>" x2="<x2 coordinate>" y2="<y2 coordinate>" layer="m3" />
...
  
</net>
</root>
```

## Literature
Griffith, J.; Robins, G.; Salowe, J.S.; Tongtong Zhang (1994-11). ["Closing the gap: near-optimal Steiner trees in polynomial time"](https://ieeexplore.ieee.org/document/329264). IEEE Transactions on Computer-Aided Design of Integrated Circuits and Systems. 13 (11): 1351–1365. [doi:10.1109/43.329264](https://ieeexplore.ieee.org/document/329264). [ISSN 1937-4151](https://www.worldcat.org/title/ieee-transactions-on-computer-aided-design-of-integrated-circuits-and-systems/oclc/1076422777)
