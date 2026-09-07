# Development Environment

## Detected environment

- Repository path: `/home/runner/work/Signal-Modulator/Signal-Modulator`
- OS: Ubuntu 24.04.4 LTS (Linux x86_64)
- Compiler toolchain:
  - clang/clang++ 18.1.3
  - gcc/g++ 13.3.0
- Build tools:
  - CMake 3.31.6
  - Ninja 1.13.2

## Adobe After Effects and SDK availability

- Adobe After Effects installation: **not detected** in this environment.
- After Effects SDK headers/examples: **not detected** in this environment.
- Key headers searched (not found): `AE_Effect.h`, `entry.h`, `PF_Suite_Helper.h`.

## SDK/plugin architecture decision

- Intended plugin type: After Effects Effect plugin (`.aex` on Windows, `.plugin` bundle on macOS).
- Integration model: AE host integration layer (`Plugin.cpp`, `Params.cpp`, `Render.cpp`) separated from core math modules (`Modulation.cpp`, `Filtering.cpp`, `Distortion.cpp`, `Channels.cpp`, `Demodulation.cpp`).
- Current architecture status: scaffolded for AE Effect API but not yet validated against a concrete SDK version on this machine.

## Feature support status (environment dependent)

Because AE and the SDK are not present locally, these are **not verified** yet:

- SmartFX: unverified
- 16-bit rendering: unverified
- 32-bit float rendering: unverified
- Multi-Frame Rendering: unverified
- GPU acceleration: unverified

## Target AE versions

- Pending SDK discovery.
- Final target matrix will be set after SDK path/version is available.

## Expected plugin output location

When building with a valid SDK/toolchain on target host OS:

- Windows: build output `SignalModulator.aex`
- macOS: build output `SignalModulator.plugin`

Installation location examples:

- Windows: `C:\Program Files\Adobe\Adobe After Effects <version>\Support Files\Plug-ins\`
- macOS: `/Applications/Adobe After Effects <version>/Plug-ins/`

## Build instructions

1. Obtain/install the Adobe After Effects SDK locally.
2. Configure with SDK root:
   ```bash
   cmake -S /home/runner/work/Signal-Modulator/Signal-Modulator \
         -B /home/runner/work/Signal-Modulator/Signal-Modulator/build \
         -G Ninja \
         -DSIGNAL_MODULATOR_AE_SDK_ROOT=/absolute/path/to/AE_SDK
   ```
3. Build:
   ```bash
   cmake --build /home/runner/work/Signal-Modulator/Signal-Modulator/build
   ```

If `SIGNAL_MODULATOR_AE_SDK_ROOT` is not set, only the core static library target is built.

## Install instructions

- Copy built plugin binary into the After Effects Plug-ins folder for your platform.
- Launch/restart After Effects.
- Effect should appear under effects menu once PiPL/resource packaging is finalized for the selected SDK setup.

## Debugging

- Build in Debug configuration:
  ```bash
  cmake -S /home/runner/work/Signal-Modulator/Signal-Modulator \
        -B /home/runner/work/Signal-Modulator/Signal-Modulator/build-debug \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug \
        -DSIGNAL_MODULATOR_AE_SDK_ROOT=/absolute/path/to/AE_SDK
  cmake --build /home/runner/work/Signal-Modulator/Signal-Modulator/build-debug
  ```
- Attach debugger to the After Effects process:
  - Windows: Visual Studio
  - macOS: Xcode/lldb
- Set breakpoints in `src/Plugin.cpp` and `src/Render.cpp`.
- Validate command flow: `PF_Cmd_GLOBAL_SETUP`, `PF_Cmd_PARAMS_SETUP`, `PF_Cmd_RENDER`.
