# Signal Modulator

Signal Modulator is an original Adobe After Effects effect plugin project for signal-style image modulation workflows.

## Current milestone

- Environment discovery completed.
- Minimal C++ project architecture created.
- Minimal AE effect skeleton created (entry point, params, passthrough render).
- Core math layer scaffolding created for future modulation/filter/distortion milestones.

## Current implementation status

- **IMPLEMENTED:** project scaffolding, minimal command handling, parameter UI seed, passthrough renderer.
- **TESTED:** CMake configure/build of core library in this environment.
- **UNTESTED:** plugin load in After Effects (AE and AE SDK are not installed in this environment).
- **STUBBED:** demodulation, filtering, distortion, advanced channel logic.

## Build

See `/home/runner/work/Signal-Modulator/Signal-Modulator/DEVELOPMENT.md` for environment details and build/install/debug instructions.
