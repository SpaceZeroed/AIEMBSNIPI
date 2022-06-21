#pragma once

#include "FlowMaps.h"
#include <vector>

namespace flowmaps
{
    class FlowMapOrkizhevskiy : public IFlowMapLiquidGas
    {
    public:
        FlowMapOrkizhevskiy();
        FlowPattern mainFlowPattern;
        PhaseInfo liquid;
        PhaseInfo gas;
        PhaseInteract phaseInteract;
        MainFase mainFase;
        double d;
        double roughness;
        double angle;
        double pInflow;
        double tInflow;
        void setD(double d);
        void setRoughness(double D);
        void setAngle(double D);
        void setLiquid(double qo_ny, double qw_ny, double Bo, double Bw, double mu_o, double mu_w, double rho_o, double rho_w);
        void setGas(double qg_ny, double qo_ny, double qw_ny, double mu_g, double Rs, double Rsw, double Bg, double rho_g);
        void setPhaseInteract(double SurfaceTension);

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
