#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct FactionDefinition
{
    std::uint32_t id = 0;
    std::string name;
};

struct BattleParams
{
    float timeStep = 0.1f;
    float linearCoefficient = 0.01f;
    float squareCoefficient = 0.00001f;
    float collapseThreshold = 0.001f;
};

struct CohortStorage
{
    std::vector<std::uint32_t> ids;
    std::vector<std::uint32_t> factionIds;
    std::vector<float> unitCounts;
    std::vector<float> initialUnitCounts;
    std::vector<float> averageHealths;
    std::vector<float> totalFirepowers;
    std::vector<float> morales;
    std::vector<float> cohesions;
    std::vector<float> positionsX;
    std::vector<float> positionsY;
    std::vector<float> frontages;
    std::vector<float> rangedWeights;

    void Clear();
    void Reserve(std::size_t capacity);
    std::size_t Size() const;
};

struct CohortDefinition
{
    std::uint32_t id = 0;
    std::uint32_t factionId = 0;
    float unitCount = 0.0f;
    float averageHealth = 1.0f;
    float totalFirepower = 0.0f;
    float morale = 1.0f;
    float cohesion = 1.0f;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float frontage = 1.0f;
    float rangedWeight = 0.5f;
};

struct SimulationSample
{
    float time = 0.0f;
    std::uint32_t factionId = 0;
    float totalUnits = 0.0f;
    float totalFirepower = 0.0f;
    float averageMorale = 0.0f;
    float averageCohesion = 0.0f;
};

class BattleSimulation
{
public:
    void Clear();
    void SetParams(const BattleParams& params);
    const BattleParams& GetParams() const;

    void AddFaction(const FactionDefinition& faction);
    void AddCohort(const CohortDefinition& cohort);

    void Reset();
    void Step();

    float GetTime() const;
    const std::vector<FactionDefinition>& GetFactions() const;
    const CohortStorage& GetCohorts() const;
    const std::vector<SimulationSample>& GetHistory() const;

private:
    void RecordHistory();

    BattleParams m_Params;
    float m_Time = 0.0f;
    std::vector<FactionDefinition> m_Factions;
    CohortStorage m_Cohorts;
    std::vector<float> m_PendingUnitLosses;
    std::vector<SimulationSample> m_History;
};
