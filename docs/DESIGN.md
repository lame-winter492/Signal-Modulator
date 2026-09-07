# Signal Modulator Design (Initial)

## Architecture

- AE integration layer handles parameter retrieval and render dispatch.
- Core signal-processing layer is isolated in standalone C++ modules for testability and future GPU parity checks.

## Current core modules

- `Modulation`: waveform and modulation primitives.
- `Demodulation`: placeholder for reconstruction models.
- `Filtering`: placeholder for low/high/band-pass framework.
- `Distortion`: placeholder for nonlinear signal transforms.
- `Channels`: placeholder for RGB/independent channel routing.

## Milestone focus

This initial milestone delivers project structure and a minimal passthrough renderer. Mathematical subsystems are intentionally scaffolded and will be filled incrementally.
