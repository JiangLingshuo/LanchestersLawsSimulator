#include "BattleSimulation.h"

#include <algorithm>

void CohortStorage::Clear()
{
    ids.clear();
    factionIds.clear();
    unitCounts.clear();
    initialUnitCounts.clear();
    averageHealths.clear();
    totalFirepowers.clear();
    morales.clear();
    cohesions.clear();
    positionsX.clear();
    positionsY.clear();
    frontages.clear();
    rangedWeights.clear();
}

void CohortStorage::Reserve(std::size_t capacity)
{
    ids.reserve(capacity);
    factionIds.reserve(capacity);
    unitCounts.reserve(capacity);
    initialUnitCounts.reserve(capacity);
    averageHealths.reserve(capacity);
    totalFirepowers.reserve(capacity);
    morales.reserve(capacity);
    cohesions.reserve(capacity);
    positionsX.reserve(capacity);
    positionsY.reserve(capacity);
    frontages.reserve(capacity);
    rangedWeights.reserve(capacity);
}

std::size_t CohortStorage::Size() const
{
    return ids.size();
}

void BattleSimulation::Clear()
{
    m_Params = BattleParams{};
    m_Time = 0.0f;
    m_Factions.clear();
    m_Cohorts.Clear();
    m_PendingUnitLosses.clear();
    m_History.clear();
}

void BattleSimulation::SetParams(const BattleParams& params)
{
    m_Params = params;
    m_Params.timeStep = std::max(0.0f, m_Params.timeStep);
    m_Params.linearCoefficient = std::max(0.0f, m_Params.linearCoefficient);
    m_Params.squareCoefficient = std::max(0.0f, m_Params.squareCoefficient);
    m_Params.collapseThreshold = std::max(0.0f, m_Params.collapseThreshold);
}

const BattleParams& BattleSimulation::GetParams() const
{
    return m_Params;
}

void BattleSimulation::AddFaction(const FactionDefinition& faction)
{
    m_Factions.push_back(faction);
}

void BattleSimulation::AddCohort(const CohortDefinition& cohort)
{
    const float unitCount = std::max(0.0f, cohort.unitCount);

    m_Cohorts.ids.push_back(cohort.id);
    m_Cohorts.factionIds.push_back(cohort.factionId);
    m_Cohorts.unitCounts.push_back(unitCount);
    m_Cohorts.initialUnitCounts.push_back(unitCount);
    m_Cohorts.averageHealths.push_back(std::max(0.0f, cohort.averageHealth));
    m_Cohorts.totalFirepowers.push_back(std::max(0.0f, cohort.totalFirepower));
    m_Cohorts.morales.push_back(std::max(0.0f, cohort.morale));
    m_Cohorts.cohesions.push_back(std::max(0.0f, cohort.cohesion));
    m_Cohorts.positionsX.push_back(cohort.positionX);
    m_Cohorts.positionsY.push_back(cohort.positionY);
    m_Cohorts.frontages.push_back(std::max(0.0f, cohort.frontage));
    m_Cohorts.rangedWeights.push_back(std::clamp(cohort.rangedWeight, 0.0f, 1.0f));

    m_PendingUnitLosses.push_back(0.0f);
}

void BattleSimulation::Reset()
{
    m_Time = 0.0f;
    m_Cohorts.unitCounts = m_Cohorts.initialUnitCounts;
    std::fill(m_PendingUnitLosses.begin(), m_PendingUnitLosses.end(), 0.0f);

    m_History.clear();
    RecordHistory();
}

void BattleSimulation::Step()
{
    std::fill(m_PendingUnitLosses.begin(), m_PendingUnitLosses.end(), 0.0f);
    m_Time += m_Params.timeStep;

    RecordHistory();
}

float BattleSimulation::GetTime() const
{
    return m_Time;
}

const std::vector<FactionDefinition>& BattleSimulation::GetFactions() const
{
    return m_Factions;
}

const CohortStorage& BattleSimulation::GetCohorts() const
{
    return m_Cohorts;
}

const std::vector<SimulationSample>& BattleSimulation::GetHistory() const
{
    return m_History;
}

void BattleSimulation::RecordHistory()
{
    for (const FactionDefinition& faction : m_Factions)
    {
        float totalUnits = 0.0f;
        float totalFirepower = 0.0f;
        float weightedMorale = 0.0f;
        float weightedCohesion = 0.0f;

        for (std::size_t cohortIndex = 0; cohortIndex < m_Cohorts.Size(); ++cohortIndex)
        {
            if (m_Cohorts.factionIds[cohortIndex] != faction.id)
            {
                continue;
            }

            const float units = m_Cohorts.unitCounts[cohortIndex];
            const float initialUnits = m_Cohorts.initialUnitCounts[cohortIndex];
            const float remainingRatio = initialUnits > 0.0f ? units / initialUnits : 0.0f;

            totalUnits += units;
            totalFirepower += m_Cohorts.totalFirepowers[cohortIndex] * remainingRatio;
            weightedMorale += m_Cohorts.morales[cohortIndex] * units;
            weightedCohesion += m_Cohorts.cohesions[cohortIndex] * units;
        }

        SimulationSample sample;
        sample.time = m_Time;
        sample.factionId = faction.id;
        sample.totalUnits = totalUnits;
        sample.totalFirepower = totalFirepower;
        sample.averageMorale = totalUnits > 0.0f ? weightedMorale / totalUnits : 0.0f;
        sample.averageCohesion = totalUnits > 0.0f ? weightedCohesion / totalUnits : 0.0f;

        m_History.push_back(sample);
    }
}
