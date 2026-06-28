# Lanchester's Laws Simulator

A C++ desktop simulator for exploring Lanchester's Laws in a fuzzy, extensible way.

The goal is not only to reproduce the classic linear and square laws, but to build a framework where different combat rules can be mixed, interpolated, visualized, and expanded. The simulator should help answer questions like:

- How does a melee-heavy battle differ from a ranged battle?
- What happens when combat shifts gradually between linear and square attrition?
- How do more than two factions behave when they target, maneuver, or form alliances?
- How do formations, terrain, morale, or command rules change the outcome?

## Project Vision

Classic Lanchester models are useful but simplified:

- Linear Law often describes melee-like combat, where local pairing and contact limits matter.
- Square Law often describes ranged combat, where many units can concentrate fire on a target.

This project treats those laws as endpoints. The simulator should support fuzzy interpolation between them by extracting combat rules into configurable systems.

Instead of hard-coding "melee" or "ranged" as separate modes, the simulator should let a scenario blend rule weights:

```text
0.0 = pure melee-like linear attrition
1.0 = pure ranged-like square attrition
0.5 = mixed combat behavior
```

Over time, this can grow into a sandbox for force modeling, combat rules, formations, and visualization.

## Current Status

The repository currently contains a Visual Studio C++ application with:

- Dear ImGui for UI.
- GLFW for windowing and input.
- OpenGL for rendering.
- Early placeholder domain types for armies, combat rules, health, firepower, and war simulation.

The application shell runs, but the actual fuzzy Lanchester simulation model is still under development.

## Planned Core Concepts

### Battle Simulation

The simulation core should be independent from rendering and UI. It should own:

- simulation time
- factions
- force state
- combat rule evaluation
- target selection
- attrition updates
- battle history

The UI should observe and control the simulation, not contain the simulation logic.

### Factions

A battle should support two or more parties.

Each faction can eventually include:

- unit count or strength
- combat effectiveness
- ranged capability
- melee capability
- morale
- cohesion
- formation state
- targeting preference
- position or spatial footprint

The first MVP can start with two factions, but the data model should avoid assuming only red and blue armies.

### Combat Rules

Combat rules should be modular. Early rules may include:

- Linear attrition rule
- Square attrition rule
- Fuzzy interpolation between linear and square rules
- Target selection rule
- Damage distribution rule
- Victory or collapse rule

Later rules can model:

- formations
- range bands
- terrain modifiers
- morale collapse
- suppression
- reinforcement
- command delay
- alliances and multi-party targeting

### Fuzzy Lanchester Model

The main design idea is to represent combat as a blend of rule influences.

For example:

```text
damage = lerp(linearDamage, squareDamage, rangedWeight)
```

Where `rangedWeight` can be controlled by scenario settings, unit behavior, formation, distance, terrain, or time.

This makes it possible to simulate battles that are not cleanly melee or ranged.

### Output Modes

The simulator should support multiple ways to inspect results:

- Runtime dot simulation showing faction state over time.
- Live graphs inside the ImGui UI.
- Exported CSV data for analysis.
- Exported charts for reports or comparison.

The MVP should prioritize a simple runtime view and CSV export. More polished chart export can come later.

## Suggested Architecture

```text
Application / ImGui Shell
    Owns window, renderer, input, frame loop.

Simulation UI
    Owns controls, scenario editing, graphs, and runtime visualization.

BattleSimulation
    Owns deterministic simulation state and stepping.

Combat Rules
    Pure functions or strategies that calculate attrition.

Scenario Data
    Serializable configuration for factions, rule weights, and output settings.
```

The most important rule: simulation code should not depend on ImGui, GLFW, or OpenGL.

## Possible Data Model

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

This is intentionally simple. The framework can later add positions, formations, unit types, and target selection behavior without rewriting the whole simulator.

## Build

This project is currently organized as a Visual Studio solution:

- `LanchestersLawsSimulator.sln`
- `LanchestersLawsSimulator/LanchestersLawsSimulator.vcxproj`

Open the solution in Visual Studio 2022 and build the `x64` configuration.

## Roadmap

Near-term:

- Replace the ImGui demo UI with simulator controls.
- Implement deterministic simulation stepping.
- Add linear, square, and fuzzy blended attrition rules.
- Show runtime results as simple dots or line plots.
- Export simulation history as CSV.

Future:

- Support more than two factions.
- Add formations and spatial relationships.
- Add richer target selection.
- Add scenario save/load.
- Add chart/image export.
- Add tests for combat math and deterministic simulation behavior.

