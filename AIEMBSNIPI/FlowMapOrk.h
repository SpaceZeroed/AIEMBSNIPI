#pragma once

#include "FlowMaps.h"

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
        void setLiquid(double qo_ny, double qw_ny, double Bo, double Bw, double mu_o, double mu_w, double rho_o, double rho_w);
        void setGas(double qg_ny, double qo_ny, double qw_ny, double mu_g, double Rs, double Rsw, double Bg, double rho_g); 
        void setPhaseInteract(double SurfaceTension);
        void defineTypeOfFlow();// �� ���������� 
                
        Result calc(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) override;
    };
} // namespace flowmaps
