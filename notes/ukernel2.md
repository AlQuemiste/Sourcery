The micro-kernel (µKernel) is the irreducible computational core of any software application. It encapsulates *all* computational functionality of the application in its purest, most basic form — expressed in a style deliberately close to C99 or Fortran 2008. The uKernel defines only the mathematics, logic, and fundamental data structures required by the application. Its paramount objectives are **correctness**, **numerical accuracy**, **robustness**, **reproducibility**, and **scalability**.



* The uKernel strictly adheres to the Unix philosophy of software development (see E. S. Raymond, *The Art of Unix Programming*, 2003, “[Basics of the Unix Philosophy](http://www.catb.org/esr/writings/taoup/html/ch01s06.html)”).
Unix Philosophy

The µKernel adheres to the Unix philosophy as codified in E. S. Raymond, *The Art of Unix Programming* (2003), especially:

- **Rule of Modularity:** Write simple parts connected by clean interfaces.
- **Rule of Clarity:** Clarity is better than cleverness.
- **Rule of Parsimony:** Write a big program only when nothing else will do.
- **Rule of Transparency:** Design for visibility to make inspection and debugging easier.
- **Rule of Robustness:** Robustness is the child of transparency and simplicity.
- **Rule of Separation:** Separate policy from mechanism; separate interfaces from engines.
- **Rule of Silence:** When a program has nothing surprising to say, it should say nothing.

## Core Design Philosophy

- **KISS** — Keep It Simple, Stupid. Complexity is the enemy of correctness and performance.
- **YAGNI** — You Ain’t Gonna Need It. Every feature, abstraction, or optimization must be justified by a concrete, present need.
- Prefers the fastest, numerically stable algorithms available while maintaining clarity.
- **Data-Oriented Design** — Data layout and access patterns are designed first for cache efficiency, vectorization, and parallel scalability; algorithms follow the data.
- **Extreme memory discipline**: zero heap allocations inside computational routines; all memory is allocated by the caller and passed in (caller-owned buffers, pre-allocated workspaces).
- Designed for **large-scale data throughput** — optimized for cache locality, contiguous memory access, and vectorization (SIMD-friendly layouts such as Structure-of-Arrays where beneficial).
- **Zero-overhead abstractions** — compile-time guarantees eliminate runtime cost.
- Compile-time switches control optional features (e.g., verbose logging, bounds checking) so production builds remain maximal performance.
- **Strong static type safety**.
- **Pure functions preferred** — no side effects, referential transparency, easy reasoning and parallelization. 
- **Total functions** — Functions are defined for all possible inputs in their domain. Edge cases (e.g., division by zero, singularities) are handled gracefully and predictably without crashing. Computational routines never throw; errors are reported via return codes, status flags, or output parameters.

- **Free Functions over Methods:** Prefers free functions (often grouped in namespaces) to class methods, ensuring functions are stateless and data structures remain pure.
- **Segregation of Data and Logic (functions)**: Emphasizes that data structures are strictly separated from the functions that transform them. All logic resides in free (non-member) functions.

- **Prefers composition over inheritance** for all computational types.
- **Avoids proxy / wrapper / handle classes** — direct data access is favored for zero-overhead and transparency.
- **Design by contract** — dedicated verification modules provide runtime (or compile-time) checks for preconditions, postconditions, and structural invariants. Assertions can be disabled in release builds.
- **Prefers safety without sacrificing performance** — safety checks are either compile-time or optional (via macros / `constexpr` flags).
- **No code or data duplication** (DRY principle enforced at design and review time).

- **Modularity & Orthogonality:** Modules are weakly coupled, cleanly segregated, and do not overlap in domain responsibility, making the kernel highly extensible and testable.
Dependencies between modules are a **directed acyclic graph**: no circular dependencies are permitted.

- **Self-contained** — minimal external dependencies; only standard C++ headers required (no external libraries inside the uKernel).
- **Easily extensible** — new algorithms or data layouts can be added as new free functions or modules without touching existing code.

- **Evaluation logic is completely separated from execution logic** — The logic that determines *what to compute* (the evaluation plan) is strictly separated from the logic that *performs the computation* (execution). Evaluation logic (the mathematics) is strictly separated from execution logic (the scheduler). This allows identical computational routines to be seamlessly dispatched in serial, concurrent, parallel, vectorized (SIMD), or asynchronous modes.

- **Low-level API only** — no high-level convenience wrappers inside the kernel; higher-level layers build on top.
- **Separate thread-safe API** — a distinct, explicitly thread-safe interface (lock-free where possible) coexists with the single-threaded high-performance path.
- **Tiered API Accessibility:** Exposes a raw, low-level API for maximum control, alongside a separate, explicitly thread-safe API layer if orchestration primitives (like atomic counters or barriers) are mathematically required for a specific domain model.
- **No mutable global state**, **No Shared Mutable State** and **no non-constant global variables** — the entire kernel is reentrant and thread-safe by design.
- Optional compile-time verbose logging for debugging without performance penalty in production.

- Dedicated **serialization / deserialization modules** for binary (platform-independent) persistence.
- Dedicated **human-readable text representation modules** for debugging, logging, and interoperability.
- All resource acquisition (memory, files, handles) occurs outside computational routines; the kernel operates exclusively on caller-provided buffers.
- External comprehensive unit tests, property-based tests, and regression tests are maintained alongside the kernel.
**No Undefined Behavior (UB):** Strictly prohibits practices that invoke UB. Relies on compiler warnings, sanitizers, and static analysis to enforce absolute language-level correctness.

-  **Compile-Time Guarantees:** Prefers shifting computational burdens, bounds checking, and configuration to compile-time wherever feasible, maximizing runtime throughput.

- **Correctness Before Optimisation**: Correct and slow is always fixable. Fast and wrong is a bug that may not be caught. Every optimisation must preserve the observable behaviour of the unoptimised version, verified by a formal argument or a comprehensive test.
- Preferrs a clear separation of pure and impure code.

## Language and Coding Style

- Uses **modern C++ (C++17 and later)** in a *highly conservative, curated subset*:
  - Plain `struct`s with public data members + free functions are the default.
  - No classes with private data, no member functions for core logic, no inheritance hierarchies, no virtual functions, no RTTI, and no exceptions inside computational paths.
  - Emphasizes value semantics, `constexpr`, `static_assert`, templates for generic precision, and compile-time computation.
  - Strictly avoids undefined behavior; all operations are well-defined or explicitly checked.


*   **Conservative Modern C++ (C++17/20):** Utilizes a highly curated subset of modern C++. It leverages features like `constexpr`, `templates`, and `concepts` for compile-time guarantees, while outright rejecting complex object-oriented overhead (e.g., RTTI, deep inheritance, virtual dispatch in hot loops).

### 10.1 Unit Tests

Every function in the public API has at least one unit test. Tests are:

- **Self-contained:** they do not share mutable state with other tests.
- **Deterministic:** the same test always produces the same result.
- **Fast:** the full unit test suite runs in under one minute.
- **Structured as black-box tests:** they test the interface contract, not the implementation.

- **Fully documented**, tends towards literate programming.
- Respect **semantic versioning**: breaking changes increment the major version.

# Changelog

A **changelog** records every public interface change, every algorithm substitution, and every numerical behaviour change. Entries cite the motivation (bug report, accuracy improvement, performance measurement) and the commit or review that approved the change.

---
