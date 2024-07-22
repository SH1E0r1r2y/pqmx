# pqmx: Post-Quantum Cryptography on Arm v8.1-M + MVE
This repository is a fork of the [pqmx](https://gitlab.com/arm-research/security/pqmx) repository, adding further
examples from public-key cryptography (both classical and post-quantum). Along with
[pqax](https://github.com/slothy-optimizer/pqax) and [SLOTHY](https://github.com/slothy-optimizer/slothy), it
accompanies the paper [Fast and Clean: Auditable high-performance assembly via
constraint-solving](https://eprint.iacr.org/2022/1303) by Abdulrahman, Becker, Kannwischer and Klein.
## Overview
### Scope

It is intended as a complement to the well-known [`pqm4`](https://github.com/mupq/pqm4/), which collects implementations
of post-quantum cryptography targeting Cortex-M4, with a focus on CPUs implementing the [M-Profile Vector Extension
(MVE)](https://www.arm.com/why-arm/technologies/helium) to the Armv8.1-M architecture (also known as Arm<sup>&reg;</sup>
Helium&trade; Technology), such as the [Arm<sup>&reg;</sup>
Cortex&trade;-M55](https://www.arm.com/products/silicon-ip-cpu/cortex-m/cortex-m55) processor.

### SLOTHY

This repository also contains the source code for the SLOTHY assembly superoptimizer, discussed in the paper [Fast and Clean: Auditable high-performance assembly via constraint solving](https://eprint.iacr.org/2022/1303). See [slothy/README.md](slothy/README.md) for more information.

### M-Profile Vector Extension (MVE)

The M-Profile Vector Extension (MVE), or Arm Helium Technology, is a Single Instruction Multiple Data (SIMD) extension for the Armv8.1-M architecture, complementing the
Arm<sup>&reg;</sup> Neon&trade; Advanced SIMD and Arm Scalable Vector Extension (SVE) for the Cortex-R and Cortex-A processor series.

We refer to the following resources for further information:
* [Arm Helium: Enhancing the Capabilities of the Smallest Devices](https://www.arm.com/why-arm/technologies/helium)
* [Making Helium: Why not just add
  NEON?](https://community.arm.com/developer/research/b/articles/posts/making-helium-why-not-just-add-neon): A blog
  series explaining the design of MVE and comparing it to Arm NEON.
* [Arm v8-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0553/latest)
* [MVE Reference Book](https://www.arm.com/resources/education/books/mve-reference-book)

## Structure

The main components of the repository are the following:
* [`asm`](asm): Core primitives in optimized assembly, mostly auto-generated.
* [`tests`](tests): C-based tests for core primitives using a minimal hardware abstraction layer (HAL).
* [`envs`](envs): Test environments implementing the HAL.
* [`slothy`](slothy): The SLOTHY assembly superoptimizer. See the [README](slothy/README.md) for more information.

The following sections explain each component in greater detail.

### Optimized assembly routines

The heart of the repository are optimized assembly routines for core components of the post quantum primitives under
consideration, such as the NTT. All optimized assembly is contained in the [`asm`](asm) directory, which is structured
as follows:

* [`asm/manual`](asm/manual) contains assembly that has been written by hand.
* [`asm/gen/`](asm/gen/) contains a small Python 3 code generation framework, offering various helper classes for
  register management, loading/storing (contiguous, non-contiguous, scattered) buffers, and common assembly snippets.
* [`asm/scripts`](asm/scripts) contains code generation scripts for various
  algorithms around polynomial multiplication or the PQC schemes they're relevant for, as well as other tests and
  examples. Those scripts build on the generic framework provided by [`asm/gen`](asm/gen).
* [`asm/auto/`](asm/auto/) contains the assembly auto-generated by the examples in
  [`asm/scripts`](asm/scripts/). Its structure mirrors that of [`asm/scripts`](asm/scripts/).

See [`asm/`](asm/) for more information.

### Tests

Each code generation example is accompanied by an example C-program contained in [`tests/`](tests/). For example, the Toom4
multiplication code generators from [`asm/scripts/toom4`](asm/scripts/toom4/) are tested in
[`tests/toom/`](tests/toom/).

The test files platform-independent and only rely on a small hardware abstraction layer
[`tests/inc/hal.h`](tests/inc/hal.h) which declares stubs for debugging, measuring, and random sources. As long as the tests get ported into an environment which defines this
hardware
through a separate translation unit in the test environment, or via `hal_env.h` in case some or all of the HAL
functionality shall be implemented through macros. Note `hal_env.h` must currently always be present in the test environment, even
if the entire HAL is implemented in a separate translation unit.

For convenience, there is also a [HelloWorld test](tests/helloworld/) with a minimal MVE assembly snippet, which can be used to test the tool setup or a new test environment.

### Test environments

As mentioned above, the tests from [`tests/`](tests/) can be run in any environment defining the hardware abstraction layer
interface [`tests/inc/hal.h`](tests/inc/hal.h). This flexibility is useful in order to test the MVE assembly in different models or
simulators of MVE-implementations.

The supported test environments are located in [`envs`](envs/).
As of now, we are supporting two platforms:
 - [Arm® Corstone™ SSE-300 with Cortex®-M55 and Ethos™-U55 (AN547)](https://developer.arm.com/downloads/view/AN547)
 - [Arm® Corstone™ SSE-310 with Cortex®-M85 and Ethos™-U55 (AN555)](https://developer.arm.com/downloads/view/AN555)

The former can be emulated using qemu (>=6.0).
Previously, the freely available FVPs for the Arm<sup>&reg;</sup> Corstone&trade;-300 MPS2 and Arm<sup>&reg;</sup> Corstone&trade;-300 MPS3 were also supported.
However, these are currently no longer maintained (see https://github.com/slothy-optimizer/pqmx/issues/7).

Writing a new test environment requires the provisioning of build, run and debug scripts, plus an implementation of the
test HAL [`tests/inc/hal.h`](tests/inc/hal.h).
If you have added a new test
environment, you can test that it works against the HelloWorld test in [`tests/helloworld`](tests/helloworld/).

To run the tests in qemu, the target `run-m55-an547_{test_name}` can be used. It will build the executable from the sources and run it using `qemu-system-arm -M mps3-an547  -nographic -semihosting -kernel`.

## License

The software is provided under an MIT license. Contributions to this project are accepted under the same license.

## Getting started

### Prerequisites

#### Compilation

Compilation requires a toolchain supporting Armv8.1-M, such as Arm Compiler 6.14 or GNU Arm Embedded Toolchain 10-2020-q4-major or
higher.

### Usage flow


The code in this repository can then be generated, compiled and run via `make`:
* `make {build,run}-{m55-an547,m85-an555}-{helloworld,ntt-kyber,ntt-dilithium}` builds/runs the chosen
  test in the chosen test environment.

We recommend trying

```
make run-m55-an547_helloworld
```

after setting up the required tooling, to check that the tools are in the
right place and working as expected.

#### Autocompletion

On MacOS+zshrc, add the following to your `.zshrc` to support autocompletion with `make`:

```
zstyle ':completion::complete:make:*:targets' call-command true
autoload -U compinit && compinit
```
