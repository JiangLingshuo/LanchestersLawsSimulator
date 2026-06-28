# Lanchester's Laws Simulator Design Notes

## Project Intent

This project is a C++ desktop simulator for exploring Lanchester's Laws of combat attrition. It appears intended to let a user configure two opposing forces, choose between Lanchester's Linear Law and Square Law, run a battle simulation, and inspect how force strength changes over time.

The current application is mostly a working Dear ImGui, GLFW, and OpenGL shell. The domain simulation model is only lightly sketched out.

## Current Structure

### Application Entry Point

`src/LanchestersLawsSimulator.cpp` contains `main()`. It creates a `Simulator` instance and calls `Run()`.

### Simulator

`include/Simulator.h` and `src/Simulator.cpp` define the main runtime object.

Current responsibilities:

- Initialize GLFW, OpenGL, and Dear ImGui.
- Create the main application window.
- Run the main loop.
- Poll input and window events.
- Render ImGui UI.
- Shut down rendering and window resources.
- Own two military data sets: red army and blue army.

At present, `Simulator` is both the application shell and the likely owner of future battle simulation state. This is useful for early scaffolding, but it should be split before the simulation grows.

### MilitaryData

`include/Military.h` defines:

```cpp
struct MilitaryData
{
    std::vector<float> firePowers;
    std::vector<float> healths;
};
```

This represents one force as parallel arrays of per-unit firepower and health.

The current structure does not enforce important invariants, such as both arrays having the same length, health values being non-negative, or dead units being excluded from combat power.

### RulesEnum

`include/RulesEnum.h` defines the intended combat rules:

```cpp
enum class RulesEnum
{
    Lanchesters_Linear,
    Lanchesters_Square,
    COUNT
};
```

The enum exists, but the rule implementations are not wired into the simulation yet.

### Placeholder Domain Classes

The following classes currently exist but do not contain behavior:

- `War`
- `Health`
- `FirePower`

These likely represent future battle orchestration and unit attribute concepts. Until they have concrete responsibilities, they add architectural noise.

### External Dependencies

The `external/` directory vendors:

- Dear ImGui for immediate-mode UI.
- GLFW for windowing and input.
- GLAD for OpenGL loading.

### Build Output

The `build/` directory contains generated Visual Studio artifacts, including object files, logs, debug symbols, and the debug executable.

## Architecture Review

### Main Issue: `Simulator` Owns Too Many Responsibilities

`Simulator` currently combines platform setup, UI rendering, app lifecycle, simulation ownership, and future simulation stepping. This makes the simulation difficult to test and likely to become coupled to ImGui and GLFW.

Before implementing more features, separate application code from gameplay simulation code.

Recommended direction:

- `Application` or `ImGuiApp`: owns window creation, renderer setup, frame lifecycle, and shutdown.
- `BattleSimulation`: owns pure simulation state, parameters, stepping, reset, and result generation.
- `SimulatorView` or `BattleView`: owns ImGui controls and visualization for the simulation.

The simulation core should compile and run without creating a window.

### Establish the Domain Model First

The current `MilitaryData` uses parallel arrays. That can be valid for data-oriented simulation, but the invariants need to be explicit.

Before adding features, decide whether the simulator models combat as aggregate forces or individual units.

Aggregate-force model:

```cpp
struct ForceState
{
    float strength;
    float effectiveness;
};
```

This is closer to the usual Lanchester equations and easier to graph, tune, and explain.

Per-unit model:

```cpp
struct Unit
{
    float health;
    float firePower;
};
```

This supports more detailed behavior, but it is more complex and may be unnecessary for a Lanchester-focused simulator.

For this project, an aggregate-force model is likely the better first implementation.

### Implement Rules as Pure Gameplay Code

The Lanchester rule math should not live inside ImGui rendering code. It should be implemented as deterministic, testable logic.

Recommended shape:

```cpp
BattleState StepBattle(const BattleState& state, const BattleParams& params, float dt);
```

Where:

- `BattleState` contains the current red and blue force states.
- `BattleParams` contains selected rule, coefficients, timestep settings, and stopping thresholds.
- `dt` controls simulation advancement.

This allows the same simulation to be used by UI, tests, command-line tools, or future visualizers.

### Add a Simulation Clock

The current `Update()` function is empty and has no timestep model.

Before adding feature work, define how simulation time advances:

- Fixed timestep for deterministic simulation.
- Pause and resume.
- Single-step.
- Reset.
- Optional fast-forward over many steps.

For a simulator, fixed-step deterministic updates are the best default.

### Replace Demo UI Early

`Simulator::Render()` still contains the Dear ImGui sample windows. Replace them with a minimal simulator UI soon:

- Red force inputs.
- Blue force inputs.
- Rule selection.
- Run, pause, reset, and step controls.
- Current state readout.
- Basic history graph or table.

The UI does not need to be final, but it should reflect the real product loop.

### Remove or Fill Placeholder Classes

`War`, `Health`, and `FirePower` should either gain concrete responsibilities or be removed until needed.

Suggested simplification:

- Remove `Health` and `FirePower` classes for now.
- Use plain numeric fields in the simulation model.
- Reintroduce stronger types only if there is a real need for validation, units, modifiers, editor metadata, or reusable behavior.

`War` could become `BattleSimulation` if it owns the simulation state and stepping.

## Recommended Next Steps

1. Define `ForceState`, `BattleState`, `BattleParams`, and `BattleResult`.
2. Implement Linear and Square Lanchester stepping as pure functions.
3. Add a small test or console harness for simulation math.
4. Split window/UI lifecycle out of the simulation core.
5. Replace the ImGui demo window with simulator controls.
6. Remove placeholder classes or give them specific responsibilities.

The highest-value improvement is keeping the simulation independent from rendering and UI. That will make future features easier to add, including graphs, presets, save/load, multiple factions, testing, and alternate frontends.
