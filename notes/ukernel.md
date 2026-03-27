# Micro-kernel principles

Micro-kernel is the ultimate computational core of a software/application. It has _all_ the computational functionality of the application, but in pure and basic form (in a style very close to C99 or FORTRAN2008). It merely defines the mathematics, logic and basic data structures for the application. It aims for correctness, accuracy, robustness and scalabitiy above all.

Micro-kernel adheres to the Unix philosophy of software development; see e.g., [Basics of the Unix Philosophy](http://www.catb.org/esr/writings/taoup/html/ch01s06.html) in E. S. Raymond, “The Art of Unix Programming” (2003).


## Micro-Kernel Principles:
- [Basics of the Unix Philosophy](http://www.catb.org/esr/writings/taoup/html/ch01s06.html)
- KISS
- YAGNI
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
