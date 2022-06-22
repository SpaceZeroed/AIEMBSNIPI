#pragma once

#include "FlowMaps.h"
#include <vector>

namespace flowmaps
{
    class FlowMapOrkizhevskiy : public IFlowMapLiquidGas
    {
    public:
        FlowMapOrkizhevskiy();

        Result calc(const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) override;

        FlowPattern modeSelection(
            double N_gv,
            double N_Lv);
        
            std::vector<std::vector<int>> fillMap();        

        Result BubbleMode(const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result CorkMode(const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result EmulsionMode(const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result TransitionalMode(const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        double MethodMarch(
            double Length, 
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

    };
} // namespace flowmaps
