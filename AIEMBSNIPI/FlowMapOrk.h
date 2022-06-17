#pragma once

#include "FlowMaps.h"

namespace flowmaps
{
    class FlowMapOrkizhevskiy : public IFlowMapLiquidGas
    {
    public:
        FlowMapOrkizhevskiy();


        Result calc(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow,
            double mu_o,
            double fw) override;

    };
} // namespace flowmaps
