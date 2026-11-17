import numpy as np
from qiskit import QuantumCircuit, QuantumRegister, ClassicalRegister
from qiskit.circuit.library import QFT
from qiskit_aer import AerSimulator

# Define Hermitian Matrix A and Vector b
A = np.array([[1.25, 0.25], 
              [0.25, 1.25]])
b = np.array([1.0, 0.0])
b_normalized = b / np.linalg.norm(b)

def build_hhl_algorithm(num_eval_qubits=2, time_scaling=np.pi):
    eval_reg = QuantumRegister(num_eval_qubits, 'clock-eval')
    sol_reg = QuantumRegister(1, 'solution-b')
    ancilla_reg = QuantumRegister(1, 'ancilla')
    c_reg = ClassicalRegister(1, 'measurement')
    
    qc = QuantumCircuit(eval_reg, sol_reg, ancilla_reg, c_reg)
    
    # Stage 1: State Preparation (|b>)
    qc.initialize(b_normalized, sol_reg)
    qc.barrier()
    
    # Stage 2: Quantum Phase Estimation (QPE)
    qc.h(eval_reg)
    for idx, qubit in enumerate(eval_reg):
        power = 2 ** idx
        angle = time_scaling * power * 0.5 
        qc.cp(angle, qubit, sol_reg[0])
    qc.barrier()
    
    iqft = QFT(num_eval_qubits, inverse=True, name="IQFT")
    qc.compose(iqft, eval_reg, inplace=True)
    qc.barrier()
    
    # Stage 3: Conditional Rotation R(1/lambda)
    for idx, qubit in enumerate(eval_reg):
        rotation_angle = np.arcsin(1.0 / (2 ** idx + 1)) 
        qc.cry(rotation_angle, qubit, ancilla_reg[0])
    qc.barrier()
    
    # Stage 4: Uncomputation
    qc.compose(QFT(num_eval_qubits, inverse=False, name="QFT"), eval_reg, inplace=True)
    for idx, qubit in reversed(list(enumerate(eval_reg))):
        power = 2 ** idx
        angle = -1 * time_scaling * power * 0.5
        qc.cp(angle, qubit, sol_reg[0])
    qc.h(eval_reg)
    qc.barrier()
    
    # Stage 5: Measurement
    qc.measure(ancilla_reg, c_reg)
    return qc

if __name__ == "__main__":
    circuit = build_hhl_algorithm(num_eval_qubits=2)
    print(f"HHL Qiskit Reference Circuit Built! Qubits: {circuit.num_qubits}, Depth: {circuit.depth()}")
    simulator = AerSimulator()
    result = simulator.run(circuit, shots=1024).result()
    print("Measurement Counts:", result.get_counts())
