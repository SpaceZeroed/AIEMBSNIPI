#pragma once

#include "FlowMaps.h"
#include <vector>

namespace flowmaps
{
    class FlowMapOrkizhevskiy : public IFlowMapLiquidGas
    {
    public:
        FlowMapOrkizhevskiy();

        Result calc(
            const PhaseInfo& Water,
            const PhaseInfo& Oil,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) override;

       
        Result BubbleMode(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result CorkMode(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow,
            MainFase mainFase);

        Result EmulsionMode(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result TransitionalMode(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow,
            MainFase mainFase);

        double MethodMarch(
            double Length, 
            const PhaseInfo& Water,
            const PhaseInfo& Oil,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

    };
} // namespace flowmaps
