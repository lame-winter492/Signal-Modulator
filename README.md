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

## Install on Windows

After building the plugin, run this from any PowerShell directory. Update `$repo` and
the After Effects version before running it:

```powershell
$repo = "C:\path\to\Signal-Modulator"
$plugin = Get-ChildItem -Path (Join-Path $repo "build") -Filter "SignalModulator.aex" -Recurse -File | Select-Object -First 1
if ($null -eq $plugin) {
    throw "SignalModulator.aex was not found under $repo\build. Build the AE plugin first."
}
$destination = Join-Path ${env:ProgramFiles} "Adobe\Adobe After Effects 2025\Support Files\Plug-ins"
New-Item -ItemType Directory -Path $destination -Force | Out-Null
Copy-Item -Path $plugin.FullName -Destination $destination -Force
```

The plugin target is only produced when CMake is configured with
`-DSIGNAL_MODULATOR_AE_SDK_ROOT` pointing to a local After Effects SDK.
Restart After Effects after copying the plugin.
