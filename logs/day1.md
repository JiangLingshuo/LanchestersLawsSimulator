# Day 1 Log

## Refactor: Split ImGui Responsibilities

Refactored the application shell so `Simulator` no longer owns most Dear ImGui setup and rendering details.

### Added

- `include/ImGuiLayer.h`
- `src/ImGuiLayer.cpp`

`ImGuiLayer` now owns:

- ImGui context creation.
- ImGui style and input configuration.
- GLFW/OpenGL ImGui backend initialization.
- Per-frame ImGui begin logic.
- Current demo UI drawing.
- OpenGL clear and ImGui draw submission.
- ImGui backend shutdown.

### Updated

- `include/Simulator.h`
- `src/Simulator.cpp`
- `LanchestersLawsSimulator/LanchestersLawsSimulator.vcxproj`
- `LanchestersLawsSimulator/LanchestersLawsSimulator.vcxproj.filters`

`Simulator` now remains responsible for:

- GLFW initialization.
- Window creation.
- Main loop ownership.
- Simulation update placeholder.
- Calling the UI layer.
- Window and GLFW shutdown.

### Notes

- Replaced the previous ImGui backend content-scale helper in `Simulator.cpp` with GLFW's `glfwGetMonitorContentScale`, keeping ImGui headers out of the simulator implementation.
- Preserved the existing demo UI behavior for now. The next step is to replace `ImGuiLayer::DrawDemoContent()` with simulator-specific controls.

## Verification

Ran:

```text
MSBuild.exe LanchestersLawsSimulator.sln /p:Configuration=Debug /p:Platform=x64 /m
```

Result:

- Build succeeded.
- 0 errors.
- 1 linker warning remains: `LNK4098`, default runtime library conflict with `MSVCRT`. This appears unrelated to the refactor.

## Implementation: Core Simulation State

Added the Day 1 simulation core for a large-scale-ready, data-oriented model.

### Added

- `include/BattleSimulation.h`
- `src/BattleSimulation.cpp`

The new simulation core includes:

- `FactionDefinition`: vector-based faction metadata, with no red/blue limit.
- `BattleParams`: deterministic timestep and future attrition parameters.
- `CohortDefinition`: input data for aggregate unit groups.
- `CohortStorage`: structure-of-arrays storage for cache-friendly cohort simulation.
- `SimulationSample`: faction-level history output.
- `BattleSimulation`: owns simulation time, factions, cohorts, reset, fixed-step advance, and history recording.

### Design Notes

- Day 1 intentionally implements deterministic state stepping, not combat damage. Fuzzy Lanchester attrition belongs in the next rule implementation pass.
- Cohorts are the core scale unit. A battle with more than one million units should be represented as many aggregate cohorts instead of one rich object per unit.
- The simulation core uses `std::vector` for factions and cohorts, so the model is not permanently limited to two parties.

### Cleanup

- Removed unused `m_RedArmy` and `m_BlueArmy` members from `Simulator`.
- Removed the old fixed `FACTION_COUNT` and `MILITARY_COUNT` macros from `Simulator.h`.

### Verification

Ran:

```text
MSBuild.exe LanchestersLawsSimulator.sln /p:Configuration=Debug /p:Platform=x64 /m
```

Result:

- Build succeeded.
- 0 errors.
- 1 linker warning remains: `LNK4098`, default runtime library conflict with `MSVCRT`. This is the same warning observed before the simulation-core change.

## Refactor: Simplify Simulator Initialization

Moved GLFW initialization, OpenGL hint selection, window creation, content scale lookup, context binding, and vsync setup into a dedicated `GlfwWindow` wrapper.

### Added

- `include/GlfwWindow.h`
- `src/GlfwWindow.cpp`

### Updated

- `Simulator::Initialize()` now only coordinates high-level startup:
  - initialize the application window
  - initialize the ImGui layer using the native window handle and display settings
- `Simulator::ShouldQuit()`, `Simulator::Render()`, and `Simulator::Shutdown()` now delegate window-specific work to `GlfwWindow`.

### Reasoning

This keeps `Simulator` as the app coordinator instead of a platform setup class. GLFW details are isolated behind a small abstraction, matching the earlier split where ImGui details moved into `ImGuiLayer`.

### Verification

Ran:

```text
MSBuild.exe LanchestersLawsSimulator.sln /p:Configuration=Debug /p:Platform=x64 /m
```

Result:

- Build succeeded.
- 0 errors.
- 1 linker warning remains: `LNK4098`, default runtime library conflict with `MSVCRT`. This is unchanged from earlier builds.

## UI: Replace ImGui Demo With Simulation Parameters

Removed the Dear ImGui demo/sample windows from the application UI.

### Updated

- `ImGuiLayer::DrawDemoContent()` was replaced with `ImGuiLayer::DrawSimulationParameters()`.
- `Simulator::Render()` now calls the simulation parameter UI.

### Current Parameters Shown

- Melee to ranged combat blend slider.
- Fixed timestep.
- Collapse threshold.
- Linear attrition coefficient.
- Square attrition coefficient.
- Red and blue initial unit counts.
- Red and blue effectiveness.
- Background color.

### Notes

The slider values are currently UI state only. The next integration step is to bind these controls to `BattleSimulation` setup and stepping.

### Verification

Ran:

```text
MSBuild.exe LanchestersLawsSimulator.sln /p:Configuration=Debug /p:Platform=x64 /m
```

Result:

- Build succeeded.
- 0 errors.
- 0 warnings.
