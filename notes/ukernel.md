# Principles of Lean Software

Micro-kernel is the ultimate computational core of a software/application. It has _all_ the computational functionality of the application, but in its purest and most basic form, expressed in a style deliberately close to C99 or Fortran 2008. It merely defines the mathematics, logic and basic data structures for the application. It aims for correctness, accuracy, robustness, reproducibility and scalabitiy above all.

* Lean software adheres to the Unix philosophy of software development; see e.g., [Basics of the Unix Philosophy](http://www.catb.org/esr/writings/taoup/html/ch01s06.html) in E. S. Raymond, “The Art of Unix Programming” (2003).

- Rule of Modularity: Write simple parts connected by clean interfaces.
- Rule of Clarity: Clarity is better than cleverness.
- Rule of Parsimony: Write a big program only when nothing else will do.
- Rule of Transparency: Design for visibility to make inspection and debugging easier.
- Rule of Robustness: Robustness is the child of transparency and simplicity.
- Rule of Separation: Separate policy from mechanism; separate interfaces from engines.

## Core Design Philosophy
- **KISS**: Keep It Simple, Stupid. Complexity is the enemy of correctness and performance.
- **YAGNI**: You Ain’t Gonna Need It. Every feature, abstraction, or optimization must be justified by a concrete, present need.
- Prefers the fastest, numerically stable algorithms available while maintaining clarity.
- **Data-Oriented Design**: Data layout and access patterns are designed first for cache efficiency, vectorization, and parallel scalability; algorithms follow the data.
- **Segregation of Data and Logic (functions)**: Emphasizes that data structures are strictly separated from the functions that transform them. All logic resides in free (non-member) functions.
- **Evaluation logic is completely separated from execution logic**: The logic that determines *what to compute* (the evaluation plan) is strictly separated from the logic that *performs the computation* (execution). Evaluation logic (the mathematics) is strictly separated from execution logic (the scheduler). This allows identical computational routines to be seamlessly dispatched in serial, concurrent, parallel, vectorized (SIMD), or asynchronous modes.
- **Extreme memory discipline**: zero heap allocations inside computational routines; all memory is allocated by the caller and passed in (caller-owned buffers, pre-allocated workspaces).
- Designed for **large-scale data throughput**: optimized for cache locality, contiguous memory access, and vectorization (SIMD-friendly layouts such as Structure-of-Arrays where beneficial).
- **Zero-overhead abstractions**: compile-time guarantees eliminate runtime cost.
- **Strong static type safety**.
-  **Compile-Time Guarantees**: Prefers shifting computational burdens, bounds checking, and configuration to compile-time wherever feasible, maximizing runtime throughput.
- Compile-time switches control optional features (e.g., verbose logging, bounds checking) so production builds remain maximal performance.
- **Free Functions over Methods**: Prefers free functions (often grouped in namespaces) to class methods, ensuring functions are stateless and data structures remain pure.
- **Correctness Before Optimisation**: Correct and slow is always fixable. Fast and wrong is a bug that may not be caught. Every optimisation must preserve the observable behaviour of the unoptimised version, verified by a formal argument or a comprehensive test.
- Prefers a **clear separation of pure and impure code**.
- **Pure functions preferred**: no side effects, referential transparency, easy reasoning and parallelization.
- **Total functions**: Functions are defined for all possible inputs in their domain. Edge cases (e.g., division by zero, singularities) are handled gracefully and predictably without crashing. Computational routines never throw; errors are reported via return codes, status flags, or output parameters.
- **Prefers composition over inheritance** for all computational types.
- **Avoids proxy / wrapper / handle classes**: direct data access is favored for zero-overhead and transparency.
- **Design by contract**: dedicated verification modules provide runtime (or compile-time) checks for preconditions, postconditions, and structural invariants. Assertions can be disabled in release builds.
- **Avoid software rot**: Build software to be flexible and easy to refactor, not rigid, with attention to code quality, to avoid "software rot" over time.
- **Prefers safety without sacrificing performance**: safety checks are either compile-time or optional (via macros / `constexpr` flags).
- **Modularity & Orthogonality**: Modules are weakly coupled, cleanly segregated, and do not overlap in domain responsibility, making the kernel highly extensible and testable.
Dependencies between modules are a **directed acyclic graph**: no circular dependencies are permitted.
- **No code or data duplication** (DRY principle enforced at design and review time).
- **Easily extensible**: new algorithms or data layouts can be added as new free functions or modules without touching existing code.
- **Self-contained**: minimal external dependencies.
- **No mutable global state**, **No Shared Mutable State** and **no non-constant global variables**: the entire kernel is reentrant and thread-safe by design.
- Optional compile-time verbose logging for debugging without performance penalty in production.
- **Low-level API only**: no high-level convenience wrappers inside the kernel; higher-level layers build on top.
- **Separate thread-safe API**: a distinct, explicitly thread-safe interface (lock-free where possible) coexists with the single-threaded high-performance path.
- **Tiered API Accessibility**: Exposes a raw, low-level API for maximum control, alongside a separate, explicitly thread-safe API layer if orchestration primitives (like atomic counters or barriers) are mathematically required for a specific domain model.
- Dedicated **serialization / deserialization modules** for binary (platform-independent) persistence.
- Dedicated **human-readable text representation modules** for debugging, logging, and interoperability.
**No Undefined Behavior (UB)**: Strictly prohibits practices that invoke UB. Relies on compiler warnings, sanitizers, and static analysis to enforce absolute language-level correctness.
- External comprehensive unit tests, property-based tests, and regression tests are maintained alongside the kernel.

*   **Conservative Modern C++ (C++17/20)**: Utilizes a highly curated subset of modern C++. It leverages features like `constexpr`, `templates`, and `concepts` for compile-time guarantees, while outright rejecting complex object-oriented overhead (e.g., RTTI, deep inheritance, virtual dispatch in hot loops).
- Uses **modern C++ (C++17 and later)** in a *highly conservative, curated subset*:
  - Plain `struct`s with public data members + free functions are the default.
  - Emphasizes value semantics, `constexpr`, `static_assert`, templates for generic precision, and compile-time computation.

- **Fully documented**, tends towards literate programming.
- Respect **semantic versioning**: breaking changes increment the major version.


- Uses modern C++ (17+) but in a very conservative manner. Micro-kernel uses only a curated subset of C++. For example, it emphasizes plain old data (POD) + functions over classes and does not allow undefined errors.
- Micro-kernel is extremely fast thanks to its simplicity and curation.
- Prefers fast optimized algorithms, and time- and memory-efficiency.
- Optimizes memory use: Strives for minimum heap allocations and low memory footprint.
- Prefers compile-time guarantees.
- Is type-safe (e.g., uses explicit type casts and enum classes).
- Prefers safety, but avoids compromising performance.
- Prefers pure functions (with no side-effects).
- Prefers simple data structures (POD, like `structs`) instead of full-blown classes. Allows for inter-operation with a C-FFI.
- Segregates functions and data.
- Prefers free functions to class methods.
- Prefers composition to inheritance for the computational data structures.
- Avoids proxy classes (prefers direct access).
- Allows no data or code duplication.
- Avoids allocating/releasing resources in the computational routines, as far as possible (resources must be allocated by the caller).
- Has very good explanative comments for the code, and has extensive documentation.
- Is modular, weakly coupled and therefore, easily extensible.
- Does not throw exceptions in its computational routines; instead sets flags to explain the situation; hence, prefers 'total' functions. The algorithms are fault-tolerant.
- Has modules to verify the consistency of its data structures ("design by contract").
- Has I/O modules to serialize its data
- Has modules to represent its data structures as plain text.
- Is appropriate for _large_ data throughput and is scalabe.
- Allows optional verbose logging with a compile-time switch.
- Has only a low-level API. The high-level API (for instance Python wrapping) is segregated from the low-level API.
- Has a separate thread-safe low-level API. Allows a parallel data flow (prefers lock-free parallelism when possible), with concurrency transparency, without race conditions.
- Separates the implementation of the algorithm from its execution method; hence the algorithm can be executed in serial, parallel or asynchronous mode.
- Has no global non-constant variables or mutable shared state (to allow multi-threading).
- Is self-contained with minimal dependencies, and avoids inclusion of extra header files.
- Has slow change cycles and is maintainable for long term.
