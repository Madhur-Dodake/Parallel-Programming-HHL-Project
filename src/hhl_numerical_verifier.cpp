#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

struct Matrix2x2 {
    double m[2][2];
};

struct Vector2D {
    double x;
    double y;
};

class InDepthHHLNumericalVerifier {
private:
    Matrix2x2 A;
    Vector2D b;

public:
    InDepthHHLNumericalVerifier(Matrix2x2 matrixA, Vector2D vectorB) : A(matrixA), b(vectorB) {}

    void runVerification() {
        std::cout << "==================================================\n";
        std::cout << "      HHL NUMERICAL SPECTRAL VERIFIER (C++)       \n";
        std::cout << "==================================================\n\n";

        // Step 1: State Preparation
        double normB = std::sqrt(b.x * b.x + b.y * b.y);
        std::cout << "[Step 1] Normalized State |b>: [" << b.x / normB << ", " << b.y / normB << "]\n\n";

        // Step 2: Spectral Decomposition (Eigenvalues)
        double trace = A.m[0][0] + A.m[1][1];
        double det = (A.m[0][0] * A.m[1][1]) - (A.m[0][1] * A.m[1][0]);
        double discriminant = std::sqrt((trace * trace / 4.0) - det);
        
        double lambda1 = (trace / 2.0) + discriminant;
        double lambda2 = (trace / 2.0) - discriminant;
        double kappa = std::abs(lambda1 / lambda2);

        std::cout << "[Step 2] Extracted Eigenvalues (QPE Target):\n";
        std::cout << "  -> Lambda 1: " << lambda1 << "\n";
        std::cout << "  -> Lambda 2: " << lambda2 << "\n";
        std::cout << "  -> Condition Number (kappa): " << kappa << "\n\n";

        // Step 3: Matrix Inversion R(1/lambda)
        std::cout << "[Step 3] Inverse Scaling Factors (1/lambda):\n";
        std::cout << "  -> 1 / Lambda 1: " << (1.0 / lambda1) << "\n";
        std::cout << "  -> 1 / Lambda 2: " << (1.0 / lambda2) << "\n\n";

        // Step 4: Solution State |x>
        double invA[2][2] = {
            { A.m[1][1] / det, -A.m[0][1] / det },
            { -A.m[1][0] / det, A.m[0][0] / det }
        };
        double x1 = invA[0][0] * b.x + invA[0][1] * b.y;
        double x2 = invA[1][0] * b.x + invA[1][1] * b.y;
        double normX = std::sqrt(x1 * x1 + x2 * x2);

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "[Step 4] Target Quantum Solution State |x>:\n";
        std::cout << "  -> |x> = [" << (x1 / normX) << ", " << (x2 / normX) << "]\n";
    }
};

int main() {
    Matrix2x2 A = {{{1.5, 0.5}, {0.5, 1.5}}};
    Vector2D b = {1.0, 0.0};
    InDepthHHLNumericalVerifier verifier(A, b);
    verifier.runVerification();
    return 0;
}
