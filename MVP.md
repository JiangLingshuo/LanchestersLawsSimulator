# One-Week MVP Plan

## MVP Goal

Build a small but real fuzzy Lanchester simulator:

- Configure two factions.
- Choose a blend between linear and square attrition.
- Run, pause, reset, and step the simulation.
- Show the battle evolving at runtime.
- Export the simulation history as CSV.

The MVP should establish an architecture that can later support more than two parties, formations, targeting rules, and richer chart output.

## Non-Goals for Week 1

Do not implement these yet:

- Full formation mechanics.
- Sophisticated spatial movement.
- Polished chart image export.
- Scenario file save/load.
- Complex unit composition.
- AI behavior.
- Networking.

Design the data model so these can be added later, but keep the first version focused.

## Architecture Target

By the end of the week, the project should have this separation:

```text
App / Platform Layer
    GLFW, OpenGL, ImGui setup and frame loop.

UI Layer
    ImGui controls, live readouts, simple visualization.

Simulation Core
    Pure C++ state, rules, stepping, and history.

Export Layer
    CSV export from recorded simulation history.
```

The simulation core must not depend on ImGui, GLFW, or OpenGL.

## Proposed Files

```text
include/
    BattleSimulation.h
    BattleState.h
    CombatRules.h
    SimulationHistory.h
    CsvExporter.h

src/
    BattleSimulation.cpp
    CombatRules.cpp
    SimulationHistory.cpp
    CsvExporter.cpp
```

Existing files can remain, but `Simulator` should become the application/UI coordinator rather than the owner of all simulation math.

## Day 1: Define the Simulation Core

Deliverables:

- Add core data structures.
- Remove assumptions that the simulation is permanently limited to red vs blue.
- Add deterministic fixed-step simulation state.

Suggested model:

```cpp
struct FactionState
{
    int id;
    float strength;
    float effectiveness;
    float morale;
    float rangedWeight;
};

struct BattleParams
{
    float timeStep;
    float linearCoefficient;
    float squareCoefficient;
    float collapseThreshold;
};

struct BattleState
{
    float time;
    std::vector<FactionState> factions;
};
```

Acceptance criteria:

- A `BattleSimulation` object can initialize from parameters and faction states.
- The simulation can reset to its initial state.
- The simulation can advance one fixed step.
- No simulation core file includes ImGui, GLFW, or OpenGL headers.

## Day 2: Implement Linear, Square, and Fuzzy Rules

Deliverables:

- Implement Lanchester Linear attrition.
- Implement Lanchester Square attrition.
- Implement fuzzy interpolation between the two.

Simple first version:

```text
linearDamage = enemyEffectiveness * enemyStrength
squareDamage = enemyEffectiveness * enemyStrength * enemyStrength
damage = lerp(linearDamage, squareDamage, rangedWeight)
```

Then scale by timestep and coefficients.

Acceptance criteria:

- A faction loses strength over time when opposed by an active enemy.
- `rangedWeight = 0.0` behaves like the linear model.
- `rangedWeight = 1.0` behaves like the square model.
- Intermediate values produce intermediate behavior.
- Strength never becomes negative.

## Day 3: Add Runtime Controls

Deliverables:

- Replace the ImGui demo window with simulator controls.
- Add editable faction settings.
- Add rule blend controls.
- Add run, pause, reset, and single-step buttons.

Minimum UI controls:

- Red strength
- Red effectiveness
- Red ranged weight
- Blue strength
- Blue effectiveness
- Blue ranged weight
- timestep
- linear coefficient
- square coefficient
- run/pause
- step
- reset

Acceptance criteria:

- User can configure both factions before running.
- User can run and pause the simulation.
- User can step exactly one tick.
- Reset restores the configured initial state.

## Day 4: Visualize the Simulation

Deliverables:

- Add live state readout.
- Add simple runtime visualization.
- Record history every simulation step.

Minimum visualization:

- Current time.
- Current strength per faction.
- A simple dot or line-style history display in ImGui.

The dot view can be simple:

```text
x-axis = time
y-axis = remaining strength
color = faction
```

Acceptance criteria:

- The user can see both factions changing over time.
- The visualization updates while the simulation runs.
- The history remains visible after pause.

## Day 5: Add CSV Export

Deliverables:

- Store simulation history in a structured format.
- Export history to CSV.

Suggested CSV columns:

```text
time,faction_id,strength,effectiveness,morale,ranged_weight
```

Acceptance criteria:

- User can export the current simulation history.
- CSV opens cleanly in a spreadsheet.
- Each simulation step records one row per faction.

## Day 6: Prepare for Expansion

Deliverables:

- Refactor any remaining red/blue-only logic.
- Make the internal simulation loop support `std::vector<FactionState>`.
- Add a simple target selection placeholder.

Suggested target rule for MVP:

```text
Each active faction attacks the strongest opposing active faction.
```

This keeps two-party behavior simple while making the model ready for more than two factions.

Acceptance criteria:

- Core simulation code can hold more than two factions.
- UI may still expose only two factions for MVP.
- Combat logic does not require hard-coded red and blue variables.

## Day 7: Polish, Test, and Document

Deliverables:

- Add basic deterministic tests or a console test harness.
- Clean up placeholder classes.
- Update documentation.
- Verify a full user flow.

Full user flow:

1. Launch app.
2. Set faction parameters.
3. Select fuzzy blend values.
4. Run simulation.
5. Pause and inspect results.
6. Reset and try different settings.
7. Export CSV.

Acceptance criteria:

- The app builds from a clean checkout.
- The MVP flow works without editing code.
- The simulation core can be tested without opening a window.
- `README.md`, `DESIGN.md`, and `MVP.md` describe the intended architecture.

## Feature Expansion Path

### More Than Two Parties

Add:

- faction IDs and names
- team or alliance IDs
- target selection rules
- per-faction diplomacy state
- multi-party history visualization

The simulation core should already use vectors of factions, so this should not require a rewrite.

### Formations

Add:

- formation type
- frontage
- depth
- cohesion
- contact area
- ranged exposure

Formations can influence the fuzzy interpolation:

```text
tight melee formation -> lower ranged weight
open ranged formation -> higher ranged weight
broken formation -> reduced effectiveness
```

### Spatial Simulation

Add:

- faction positions
- engagement distance
- movement speed
- weapon range
- terrain effects

Distance can drive rule blending:

```text
long distance -> square/ranged behavior
close contact -> linear/melee behavior
```

### Better Output

Add:

- line charts
- stacked strength charts
- win/loss summary
- batch scenario comparison
- chart image export
- scenario JSON export/import

## Definition of Done

The one-week MVP is done when the project has a working interactive loop:

- Configure two factions.
- Blend between melee-like and ranged-like attrition.
- Run deterministic simulation.
- See results live.
- Export results.
- Keep the core architecture ready for multi-party and formation features.

