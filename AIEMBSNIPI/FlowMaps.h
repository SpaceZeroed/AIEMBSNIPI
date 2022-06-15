
/*

*/
#pragma once
#include <iostream>
#include <cstdint>
#include <memory>
using namespace std;

namespace flowmaps
{
    //   ,   PipeSim     Begs-Brill
    enum class FlowPattern
    {
        BubbleMode = 0, //  
        CorkMode, // 
        TransitionalMode, // 
        EmulsionMode, //  
        Unknown
    };



    struct PhaseInfo
    {
        //   ..
        double rho_sc;
        // 
        double rho;
        // 
        double mu;
        //     
        double q;
    };

    struct PhaseInteract
    {
        double lgSurfaceTension;
        //// .  
        //double ogSurfaceTension;
        //// .  
        //double wgSurfaceTension;
        ////        (    )
        //double NoFlowWaterCut;
        ////        

        ///correlation::OilWaterMixingViscosityPtr oil_water_visc;
    };

    ///      
    struct Result
    {
        //  
        FlowPattern flowPattern;
        //   (, ++)
        double pressureGradient;
        //    
        double pressureGradientFriction;
        //    
        double pressureGradientElevation;
        //    
        double pressureGradientAcc;
        //   
        double liquidHoldup;
        //   
        double fluidMeanVelocity;
        //  
        double liquidVelocity;
        //  
        double gasVelocity;
        //   
        double liquidSurfaceTension;
        //    
        double Re_L;
        //    
        double Re_G;
        //   
        double Re;
        //  
        double Mu_L;
        //  
        double Mu_G;
        //  
        double frictionFactor;
        //   ( flow map)
        double N_l;
        //  
        //double N_fr;

        Result() :
            flowPattern(FlowPattern::Unknown),
            pressureGradient(NAN),
            liquidHoldup(NAN),
            N_l(NAN),
            //N_fr(NAN),
            Re(NAN),
            Re_L(NAN),
            Re_G(NAN)
        {
        }
    };

    //
    class IFlowMapLiquidGas
    {
    public:
        virtual Result calc(
            const PhaseInfo& Liquid,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) = 0;
        //virtual void setUseSmoothing(bool Value) = 0;
    };
    using FlowMapLiquidGasPtr = std::shared_ptr<IFlowMapLiquidGas>;

    ///     
    /*class SinglePhaseFriction
    {
      public:
        //   Re  (!)
        virtual double value(double Re, double Rough) = 0;
    };
    using SinglePhaseFrictionPtr = std::shared_ptr<SinglePhaseFriction>;*/

    // ============================================================================

} // namespace flowmaps
