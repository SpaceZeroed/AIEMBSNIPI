#include "FLowMapOrk.h"
#define PI 3.14159265358979323846
#define g 9.806
namespace flowmaps
{

    FlowMapOrkizhevskiy::FlowMapOrkizhevskiy()
    {

    }

    Result FlowMapOrkizhevskiy::BubbleMode()
    {
        Result res;
        double Vs, HL, Ap, Ed;
        res.flowPattern = FlowPattern::BubbleMode;
        Ap = PI * d * d / 4;
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap;
        Vs = 0.244;
        HL = 1 - (1 + res.fluidMeanVelocity / Vs - sqrt(pow(1 + res.fluidMeanVelocity / Vs, 2) - 4 * gas.q / (Ap * Vs))) / 2;//4.60
        res.Re = liquid.rho * (liquid.q / (Ap * HL)) * d / liquid.mu;//4.62
        Ed = roughness / d;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradient = res.frictionFactor * liquid.rho * pow(liquid.q / (Ap * HL), 2) / (2 * d);//4.61
        return res;
    }

    Result FlowMapOrkizhevskiy::CorkMode()
    {
        Result res;
        double Ap, Ed, Vb1, Vb2, Re_B, Vbs, liqDistribCoef, X, rho_s;
        /*
        */
        res.flowPattern = FlowPattern::CorkMode;
        Ap = PI * d * d / 4;
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap;
        res.liquidVelocity = liquid.q / Ap;//Vsl
        res.gasVelocity = gas.q / Ap;
        res.Re_L = liquid.rho * res.fluidMeanVelocity * d / liquid.mu;//4.66
        Vb2 = 0.5 * sqrt(g * d);//4.71
        Re_B = liquid.rho * Vb2 * d / liquid.mu;//4.66

        do
        {
            Vb1 = Vb2;
            if (Re_B <= 3000)
            {
                Vb2 = (0.546 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * d);//4.67
            }
            else if (Re_B >= 8000)
            {
                Vb2 = (0.35 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * d);//4.68
            }
            else
            {
                Vbs = (0.251 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * d);//4.70
                Vb2 = 0.5 * (Vbs + sqrt(pow(Vbs, 2) + 13.59 * liquid.mu / (liquid.rho * sqrt(d))));//4.69
            }
            Re_B = liquid.rho * Vb2 * d / liquid.mu;//4.66
        } while (abs(Vb1 - Vb2) / Vb1 > 0.05);

        if (res.fluidMeanVelocity < 3)//liqDistribCoef = 
        {
            liqDistribCoef = 0.0127 * log10(liquid.mu + 1) / pow(d / 0.3048, 1.415) - 0.284 + 0.167 * log10(res.fluidMeanVelocity / 0.3048) + 0.113 * log10(d / 0.3048);// 4.74          
        }
        else
        {
            X = -log10(res.fluidMeanVelocity) * (0.01 * log10(liquid.mu + 1) / pow(d / 0.3048, 1.571) + 0.397 + 0.63 * log10(d / 0.3048));//4.76
            liqDistribCoef = 0.0274 * log10(liquid.mu + 1) / pow(d / 0.3048, 1.371) + 0.161 + 0.569 * log10(d / 0.3048) + X;//4.75
        }
        rho_s = (liquid.rho * (res.liquidVelocity + Vb2) + gas.rho * res.gasVelocity) / (res.fluidMeanVelocity + Vb2) + liquid.rho * liqDistribCoef;//4.63

        Ed = roughness / d;
        double pn, lambda_L, mu_n; // добавить комменты..
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8
        pn = liquid.rho * lambda_L + gas.rho * (1 - lambda_L);//3.23
        mu_n = liquid.mu * lambda_L + gas.mu * (1 - lambda_L);//3.23
        res.Re = pn * res.fluidMeanVelocity * d / mu_n;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradientFriction = ((res.liquidVelocity + Vb2) / (res.fluidMeanVelocity + Vb2) + liqDistribCoef) * res.frictionFactor * liquid.rho * pow(res.fluidMeanVelocity, 2) / (2 * d);//4.79
        res.pressureGradient = res.pressureGradientFriction + rho_s * g;//4.31

        return res;
    }

    Result FlowMapOrkizhevskiy::EmulsionMode(
        const PhaseInfo& Liquid,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        Result res;
        double Ap, N_we, N_mu, Ed, Vsg, E_k, rho_n, rho_s;
        res.flowPattern = FlowPattern::EmulsionMode;
        Ap = PI * D * D / 4;
        Vsg = gas.q / Ap; //3.11

        N_we = gas.rho * pow(Vsg, 2) * roughness / phaseInteract.lgSurfaceTension; // 4.47
        N_mu = pow(liquid.mu, 2) / (liquid.rho * phaseInteract.lgSurfaceTension * roughness); //4,48

        if (N_mu * N_we <= 0.005)
        {
            Ed = 0.0749 * phaseInteract.lgSurfaceTension / (gas.rho * pow(Vsg, 2) * d);//4.49
        }
        else
        {
            Ed = 0.3713 * phaseInteract.lgSurfaceTension * pow(N_mu * N_we, 0.302) / (gas.rho * pow(Vsg, 2) * d);//4.50
        }

        double pn, lambda_L, mu_n;
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8
        pn = liquid.rho * lambda_L + gas.rho * (1 - lambda_L);//3.23
        mu_n = liquid.mu * lambda_L + gas.mu * (1 - lambda_L);//3.23
        res.Re = pn * res.fluidMeanVelocity * d / mu_n;

        if (Ed > 0.05)
        {
            res.frictionFactor = 4 * (1 / pow(4 * log10(0.27 * Ed), 2) + 0.067 * pow(Ed, 1.73));//4,51
        }
        else
        {
            res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2,19
        }

        res.pressureGradientFriction = res.frictionFactor * gas.rho * pow(Vsg, 2) / (2 * d); //4,45
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap; // fluidMeanVelocity = Vm

        lambda_L = liquid.q / (liquid.q + gas.q);//3.8
        rho_n = lambda_L * liquid.rho + gas.rho * (1 - lambda_L);//3.23
        E_k = res.fluidMeanVelocity * Vsg * rho_n / pInflow;
        // 104
        rho_s = liquid.rho * lambda_L + gas.rho * (1 - lambda_L);//3.22
        //       
        res.pressureGradientElevation = rho_s * g;
        res.pressureGradient = (res.pressureGradientElevation + res.pressureGradientFriction) / (1 - E_k); //4.54
        return res;
    }

    Result FlowMapOrkizhevskiy::TransitionalMode(
        const PhaseInfo& Liquid,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        Result resCork, resEmul, res;
        double Ngvstr, Ngvtrm, Ap, Ngv, Nlv, A;
        res.flowPattern = FlowPattern::TransitionalMode;
        Ap = PI * D * D / 4;
        resCork = CorkMode(
            Liquid,
            Gas,
            PhaseInteract,
            D,
            Roughness,
            Angle,
            PInflow,
            TInflow);
        resEmul = EmulsionMode(
            Liquid,
            Gas,
            PhaseInteract,
            D,
            Roughness,
            Angle,
            PInflow,
            TInflow);
        Ngv = (Gas.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (Liquid.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c
        A = (Ngvtrm - Ngv) / (Ngvtrm - Ngvstr);//4.56
        res.pressureGradient = A * resCork.pressureGradient + (1 - A) * resEmul.pressureGradient;
        return res;
    }

    double FlowMapOrkizhevskiy::MethodMarch(
        double Length, 
        const PhaseInfo& Liquid,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        double deltaP = 0.0;
        double pIn = PInflow;
        for (int i = 0; i < Length; i++)
        {
            double difP = calc(
                Liquid,
                Gas,
                PhaseInteract,
                D,
                Roughness,
                Angle,
                pIn,
                TInflow).pressureGradient;
            pIn -= difP;
            deltaP += difP;            
        }
        cout << "P now " << pIn << "|| delta now = " << deltaP << "\n";
        return deltaP;
    }

    MainFase DefineMainFase(double mu_o, double fw)
    {
        double x, y;
        y = mu_o * 1000;
        x = fw;
        if (y > (x - 0.5) * (0.2 - 0.5) / (1000. - 1) + 1)
        {
            return MainFase::Water;
        }
        else
        {
            return MainFase::Oil;
        }
    }

    Result FlowMapOrkizhevskiy::calc(
        const PhaseInfo& Liquid,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        Result res;
        double Lb, Ap, lambda_L, Ngv, Ngvstr, Nlv, Ngvtrm;
        Ap = PI * D * D/ 4;
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        Lb = 1.071 - 0.2218 * pow(res.fluidMeanVelocity / 0.3048, 2) * 0.3048 / D;//4.59
        if (Lb < 0.13)
        {
            Lb = 0.13;
        }
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8

        Ngv = (Gas.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (Liquid.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c

        if (1 - lambda_L <= Lb)
        {
            res = BubbleMode( 
                Liquid,
                Gas,
                PhaseInteract,
                D,
                Roughness,
                Angle,
                PInflow,
                TInflow);
            res.flowPattern = FlowPattern::BubbleMode;
        }
        else if (Ngv < Ngvstr)
        {
            res = CorkMode(
                Liquid,
                Gas,
                PhaseInteract,
                D,
                Roughness,
                Angle,
                PInflow,
                TInflow);
            res.flowPattern = FlowPattern::CorkMode;
        }

        else if (Ngvstr < Ngv && Ngv < Ngvtrm)
        {
            res = TransitionalMode(
                Liquid,
                Gas,
                PhaseInteract,
                D,
                Roughness,
                Angle,
                PInflow,
                TInflow);
            res.flowPattern = FlowPattern::TransitionalMode;
        }
        else
        {
            res = EmulsionMode(
                Liquid,
                Gas,
                PhaseInteract,
                D,
                Roughness,
                Angle,
                PInflow,
                TInflow);
            res.flowPattern = FlowPattern::EmulsionMode;
        }

        return res;
    } 
} // namespace flowmaps
