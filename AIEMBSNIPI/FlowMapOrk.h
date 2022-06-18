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
        double D;
        double Roughness;
        double Angle;
        void setD(double d);
        void setRoughness(double D);
        void setAngle(double D);
        void setLiquid(double qo_ny, double qw_ny, double Bo, double Bw, double mu_o, double mu_w, double rho_o, double rho_w);
        void setGas(double qg_ny, double qo_ny, double qw_ny, double mu_g, double Rs, double Rsw, double Bg, double rho_g);
        void setPhaseInteract(double SurfaceTension);
        void defineTypeOfFlow();//   

        Result calc(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) override;

        FlowPattern modeSelection(
            double D,
            double N_gv,
            double N_Lv);
        

        void fillMap(

            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);        

        Result BubbleMode(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result CorkMode(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result EmulsionMode(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        Result TransitionalMode(
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);

        double MethodMarch(
            double Length,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow);
        
    };
} // namespace flowmaps
