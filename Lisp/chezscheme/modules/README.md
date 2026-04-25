# Chez Scheme R6RS Example

## Project Structure

```
chez-example/
├── main.sls          — top-level script
├── test-greet.sls    — test script
└── modules/
    └── greet.sls     — (modules greet) library
```

## Requirements

- [Chez Scheme](https://cisco.github.io/ChezScheme/) 9.x or later
- The `scheme` (or `chez`) binary must be on your PATH

## Running the Main Script

```bash
cd chez-example/
scheme --libdirs "." --script main.sls
```

Expected output:
```
Hello, World!
Hello, Chez Scheme!
```

## Running the Tests

```bash
cd chez-example/
scheme --libdirs "." --script test-greet.sls
```

Expected output:
```
Running tests...
  PASS: basic greeting
  PASS: custom name
  PASS: empty name
  PASS: greet! produces output

Results: 4 passed, 0 failed.
```

## Notes

- `--libdirs "."` tells Chez to look for libraries relative to the current
  directory. The library name `(modules greet)` maps to `./modules/greet.sls`.
- Both files use `(import (chezscheme))` rather than `(import (rnrs))`.
  Either works in Chez, but `(chezscheme)` gives access to the full Chez
  standard environment including useful extensions.
- Library files use the `.sls` extension (Scheme Library Source), which is
  the R6RS convention. Chez also accepts `.ss` and `.scm`.
- Always run from the project root (`chez-example/`) so that relative library
  resolution works correctly.
