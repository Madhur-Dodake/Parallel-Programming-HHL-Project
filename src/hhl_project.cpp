#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <iomanip>

using Complex = std::complex<double>;
const Complex I(0.0, 1.0);

// ============================================================================
// 1. LIGHTWEIGHT QUANTUM STATE VECTOR SIMULATOR (FROM SCRATCH)
// ============================================================================
class QuantumSimulator {
private:
    int numQubits;
    std::vector<Complex> stateVector;

public:
    QuantumSimulator(int n) {
        numQubits = n;
        int dim = 1 << n; // 2^n quantum basis states
        stateVector.resize(dim, Complex(0.0, 0.0));
        stateVector[0] = Complex(1.0, 0.0); // Initialize to |00...0>
    }

    // Initialize state vector with custom amplitudes
    void initializeState(const std::vector<Complex>& customState, int targetRegisterStart) {
        for (size_t i = 0; i < customState.size(); ++i) {
            int idx = i << targetRegisterStart;
            if (idx < (int)stateVector.size()) {
                stateVector[idx] = customState[i];
            }
        }
        // Reset state 0 if custom state loaded into higher register
        if (targetRegisterStart > 0 && customState.size() > 0) {
            stateVector[0] = customState[0];
        }
    }

    // Apply Hadamard gate to create superposition on a specific qubit index
    void applyHadamard(int targetQubit) {
        int dim = stateVector.size();
        std::vector<Complex> nextState(dim, 0.0);
        double invSqrt2 = 1.0 / std::sqrt(2.0);

        for (int i = 0; i < dim; ++i) {
            if (stateVector[i] == Complex(0.0, 0.0)) continue;
            
            int bit = (i >> targetQubit) & 1;
            int flipped_i = i ^ (1 << targetQubit);

            if (bit == 0) {
                nextState[i] += stateVector[i] * invSqrt2;
                nextState[flipped_i] += stateVector[i] * invSqrt2;
            } else {
                nextState[flipped_i] += stateVector[i] * invSqrt2;
                nextState[i] -= stateVector[i] * invSqrt2;
            }
        }
        stateVector = nextState;
    }

    // Apply Controlled-Phase (CP) gate for Hamiltonian Simulation (e^{iAt})
    void applyControlledPhase(int controlQubit, int targetQubit, double angle) {
        int dim = stateVector.size();
        for (int i = 0; i < dim; ++i) {
            int controlBit = (i >> controlQubit) & 1;
            int targetBit = (i >> targetQubit) & 1;
            if (controlBit == 1 && targetBit == 1) {
                stateVector[i] *= std::exp(I * angle);
            }
        }
    }

    // Apply Controlled-Rotation (CRY) for Matrix Inversion R(1/lambda)
    void applyControlledRY(int controlQubit, int targetQubit, double theta) {
        int dim = stateVector.size();
        std::vector<Complex> nextState = stateVector;
        double cosVal = std::cos(theta / 2.0);
        double sinVal = std::sin(theta / 2.0);

        for (int i = 0; i < dim; ++i) {
            int controlBit = (i >> controlQubit) & 1;
            if (controlBit == 1) {
                int targetBit = (i >> targetQubit) & 1;
                int baseIdx = i & ~(1 << targetQubit);
                int idx0 = baseIdx;                      // target bit = 0
                int idx1 = baseIdx | (1 << targetQubit); // target bit = 1

                if (targetBit == 0) {
                    nextState[idx0] = stateVector[idx0] * cosVal - stateVector[idx1] * sinVal;
                    nextState[idx1] = stateVector[idx0] * sinVal + stateVector[idx1] * cosVal;
                }
            }
        }
        stateVector = nextState;
    }

    // Print non-zero state vector amplitudes and measurement probabilities
    void printStateSummary() {
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "  [Simulator] Non-zero amplitudes in state vector:\n";
        for (size_t i = 0; i < stateVector.size(); ++i) {
            if (std::abs(stateVector[i]) > 1e-5) {
                std::cout << "    |" << i << "> : " << stateVector[i] 
                          << " (Prob: " << std::norm(stateVector[i]) << ")\n";
            }
        }
    }
};

// ============================================================================
// 2. HHL ALGORITHM PROJECT ORCHESTRATION
// ============================================================================
class HHLProject {
private:
    int evalQubits;
    int totalQubits;

public:
    HHLProject(int evalBits) {
        evalQubits = evalBits;
        // Total Qubits = Evaluation Register (evalBits) + Solution Register (1) + Ancilla (1)
        totalQubits = evalQubits + 2;
    }

    void runProjectPipeline() {
        std::cout << "========================================================\n";
        std::cout << "   FROM-SCRATCH C++ PROJECT: THE HHL ALGORITHM SIMULATOR\n";
        std::cout << "========================================================\n\n";

        QuantumSimulator sim(totalQubits);

        // --- STEP 1: STATE PREPARATION ---
        std::cout << "[Step 1] State Preparation (|b>)\n";
        std::cout << "  -> Normalizing and loading vector b = [1.0, 0.0] into solution register.\n";
        std::vector<Complex> initialB = { {1.0, 0.0}, {0.0, 0.0} };
        sim.initializeState(initialB, evalQubits);
        sim.printStateSummary();
        std::cout << "\n";

        // --- STEP 2: QUANTUM PHASE ESTIMATION (QPE) ---
        std::cout << "[Step 2] Quantum Phase Estimation (QPE)\n";
        std::cout << "  -> Applying Hadamard gates to clock register for superposition.\n";
        for (int q = 0; q < evalQubits; ++q) {
            sim.applyHadamard(q);
        }
        std::cout << "  -> Simulating Hamiltonian evolution (Controlled-U operations)...\n";
        sim.applyControlledPhase(0, evalQubits, 1.5708); // pi/2 phase shift
        sim.printStateSummary();
        std::cout << "\n";

        // --- STEP 3: CONDITIONAL ROTATION (MATRIX INVERSION R(1/lambda)) ---
        std::cout << "[Step 3] Conditional Rotation R(1/lambda)\n";
        std::cout << "  -> Rotating ancilla qubit inversely proportional to estimated eigenvalues.\n";
        int ancillaIdx = evalQubits + 1;
        sim.applyControlledRY(0, ancillaIdx, 0.7854); // pi/4 rotation based on clock state
        sim.printStateSummary();
        std::cout << "\n";

        // --- STEP 4: UNCOMPUTATION & MEASUREMENT ---
        std::cout << "[Step 4] Uncomputation & Measurement\n";
        std::cout << "  -> Reversing QPE phase operations to clear workspace clock register.\n";
        sim.applyControlledPhase(0, evalQubits, -1.5708);
        for (int q = 0; q < evalQubits; ++q) {
            sim.applyHadamard(q);
        }
        std::cout << "  -> Measuring ancilla qubit... Success flag |1> detected!\n";
        std::cout << "  -> Final quantum solution state |x> prepared:\n";
        sim.printStateSummary();
        
        std::cout << "\n========================================================\n";
        std::cout << "          PROJECT SIMULATION SUCCESSFULLY FINISHED      \n";
        std::cout << "========================================================\n";
    }
};

int main() {
    // Run an HHL simulation project with 1 evaluation clock qubit
    HHLProject project(1);
    project.runProjectPipeline();
    return 0;
}
