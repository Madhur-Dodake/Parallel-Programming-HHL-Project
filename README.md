# The HHL Algorithm: Solving Linear Systems Quantumly (From-Scratch C++ Project)

## Overview
This project implements a **from-scratch Quantum State-Vector Simulator in C++** to model the **HHL (Harrow-Hassidim-Lloyd) Quantum Algorithm** for solving linear systems of equations ($Ax = b$). Instead of relying on external black-box quantum libraries, the simulator manually computes complex state amplitudes, superposition, Hamiltonian phase shifts, and conditional ancilla rotations.

## Folder Structure
- `src/hhl_project.cpp` — Main from-scratch C++ quantum state-vector simulator and 4-stage HHL pipeline.
- `src/hhl_numerical_verifier.cpp` — Analytical spectral decomposition and condition number ($\kappa$) verifier in C++.
- `python_reference/hhl_qiskit.py` — Reference circuit implementation using Qiskit for validation.
- `web_visualizer/hhl_dashboard.html` — Interactive browser-based HHL simulator and visualizer.
- `Makefile` — Build script for compiling the C++ executables.

## How to Compile & Run (C++)
```bash
make
./hhl_project
./hhl_verifier
```
Or compile manually with `g++`:
```bash
g++ -std=c++11 -O3 src/hhl_project.cpp -o hhl_project
./hhl_project
```

## Reference Research Papers
1. A. W. Harrow, A. Hassidim, and S. Lloyd, *"Quantum Algorithm for Linear Systems of Equations,"* Physical Review Letters, vol. 103, no. 15, p. 150502, Oct. 2009 (arXiv:0811.3171).
2. Y. Cao, A. Daskin, S. Frankel, and S. Kais, *"Quantum circuit design for solving linear systems of equations,"* Molecular Physics, vol. 110, no. 15–16, pp. 1675–1680, Aug. 2012 (arXiv:1110.2232).
