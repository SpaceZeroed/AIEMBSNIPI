#pragma once

#include "FlowMaps.h"
#include <vector>

namespace flowmaps
{
    class FlowMapOrkizhevskiy : public IFlowMapLiquidGas
    {
    public:
        FlowMapOrkizhevskiy();


        void  setArguments(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) ;

        Result calc() override;

        FlowPattern modeSelection(
            double N_gv,
            double N_Lv);
        
            std::vector<std::vector<int>> fillMap();        

        Result BubbleMode();

        Result CorkMode();

        Result EmulsionMode();

        Result TransitionalMode();

        double MethodMarch(
            double Length);

        Result GreyMethod();
        
    };
} // namespace flowmaps
