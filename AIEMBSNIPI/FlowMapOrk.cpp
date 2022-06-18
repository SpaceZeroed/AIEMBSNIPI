#include "FLowMapOrk.h"
#define PI 3.14159265358979323846
#define g 9.806
namespace flowmaps
{

    FlowMapOrkizhevskiy::FlowMapOrkizhevskiy()
    {

    }

    Result BubbleMode(
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
        double Vs, HL, Ap, Ed;
        res.flowPattern = FlowPattern::BubbleMode;
        Ap = PI * D * D / 4;
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        Vs = 0.244;
        HL = 1 - (1 + res.fluidMeanVelocity / Vs - sqrt(pow(1 + res.fluidMeanVelocity / Vs, 2) - 4 * Gas.q / (Ap * Vs))) / 2;//4.60
        res.Re = Liquid.rho * (Liquid.q / (Ap * HL)) * D / Liquid.mu;//4.62
        Ed = Roughness / D;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradient = res.frictionFactor * Liquid.rho * pow(Liquid.q / (Ap * HL), 2) / (2 * D);//4.61
        return res;
    }

    Result CorkMode(
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
        double Ap, Ed, Vb1, Vb2, Re_B, Vbs, liqDistribCoef, X, rho_s;
        res.flowPattern = FlowPattern::CorkMode;
        Ap = PI * D * D / 4;
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        res.liquidVelocity = Liquid.q / Ap;//Vsl
        res.gasVelocity = Gas.q / Ap;
        res.Re_L = Liquid.rho * res.fluidMeanVelocity * D / Liquid.mu;//4.66
        Vb2 = 0.5 * sqrt(g * D);//4.71
        Re_B = Liquid.rho * Vb2 * D / Liquid.mu;//4.66

        do
        {
            Vb1 = Vb2;
            if (Re_B <= 3000)
            {
                Vb2 = (0.546 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.67
            }
            else if (Re_B >= 8000)
            {
                Vb2 = (0.35 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.68
            }
            else
            {
                Vbs = (0.251 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.70
                Vb2 = 0.5 * (Vbs + sqrt(pow(Vbs, 2) + 13.59 * Liquid.mu / (Liquid.rho * sqrt(D))));//4.69
            }
            Re_B = Liquid.rho * Vb2 * D / Liquid.mu;//4.66
        } while (abs(Vb1 - Vb2) / Vb1 > 0.05);

        if (res.fluidMeanVelocity < 3)//liqDistribCoef = 
        {
            liqDistribCoef = 0.0127 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.415) - 0.284 + 0.167 * log10(res.fluidMeanVelocity / 0.3048) + 0.113 * log10(D / 0.3048);// 4.74          
        }
        else
        {
            X = -log10(res.fluidMeanVelocity) * (0.01 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.571) + 0.397 + 0.63 * log10(D / 0.3048));//4.76
            liqDistribCoef = 0.0274 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.371) + 0.161 + 0.569 * log10(D / 0.3048) + X;//4.75
        }
        rho_s = (Liquid.rho * (res.liquidVelocity + Vb2) + Gas.rho * res.gasVelocity) / (res.fluidMeanVelocity + Vb2) + Liquid.rho * liqDistribCoef;//4.63

        Ed = Roughness / D;
        double pn, lambda_L, mu_n;
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        pn = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.23
        mu_n = Liquid.mu * lambda_L + Gas.mu * (1 - lambda_L);//3.23
        res.Re = pn * res.fluidMeanVelocity * D / mu_n;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradientFriction = ((res.liquidVelocity + Vb2) / (res.fluidMeanVelocity + Vb2) + liqDistribCoef) * res.frictionFactor * Liquid.rho * pow(res.fluidMeanVelocity, 2) / (2 * D);//4.79
        res.pressureGradient = res.pressureGradientFriction + rho_s * g;//4.31

        return res;
    }

    Result EmulsionMode(
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
        double Ap, N_we, N_mu, Ed, Vsg, f, E_k, rho_n, rho_s;
        res.flowPattern = FlowPattern::EmulsionMode;
        Ap = PI * D * D / 4;
        Vsg = Gas.q / Ap; //3.11

        N_we = Gas.rho * pow(Vsg, 2) * Roughness / PhaseInteract.lgSurfaceTension; // 4.47
        N_mu = pow(Liquid.mu, 2) / (Liquid.rho * PhaseInteract.lgSurfaceTension * Roughness); //4,48

        if (N_mu * N_we <= 0.005)
        {
            Ed = 0.0749 * PhaseInteract.lgSurfaceTension / (Gas.rho * pow(Vsg, 2) * D);//4.49
        }
        else
        {
            Ed = 0.3713 * PhaseInteract.lgSurfaceTension * pow(N_mu * N_we, 0.302) / (Gas.rho * pow(Vsg, 2) * D);//4.50
        }

        double pn, lambda_L, mu_n;
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        pn = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.23
        mu_n = Liquid.mu * lambda_L + Gas.mu * (1 - lambda_L);//3.23
        res.Re = pn * res.fluidMeanVelocity * D / mu_n;

        if (Ed > 0.05)
        {
            res.frictionFactor = 4 * (1 / pow(4 * log10(0.27 * Ed), 2) + 0.067 * pow(Ed, 1.73));//4,51
        }
        else
        {
            res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2,19
        }

        res.pressureGradientFriction = res.frictionFactor * Gas.rho * pow(Vsg, 2) / (2 * D); //4,45
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap; // fluidMeanVelocity = Vm

        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        rho_n = lambda_L * Liquid.rho + Gas.rho * (1 - lambda_L);//3.23
        E_k = res.fluidMeanVelocity * Vsg * rho_n / PInflow;
        // 104
        rho_s = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.22
        //       
        res.pressureGradientElevation = rho_s * g;
        res.pressureGradient = (res.pressureGradientElevation + res.pressureGradientFriction) / (1 - E_k); //4.54
        return res;
    }

    Result TransitionalMode(
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
        resCork = CorkMode(Liquid, Gas, PhaseInteract, D, Roughness, Angle, PInflow, TInflow);
        resEmul = EmulsionMode(Liquid, Gas, PhaseInteract, D, Roughness, Angle, PInflow, TInflow);
        Ngv = (Gas.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (Liquid.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c
        A = (Ngvtrm - Ngv) / (Ngvtrm - Ngvstr);//4.56
        res.pressureGradient = A * resCork.pressureGradient + (1 - A) * resEmul.pressureGradient;
        return res;
    }

    Result GreyMethod(
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
        double R, Ap, Vsg, Vsl, rho_n, lambda_L, N_v, N_D, B, HL, rho_s, epsilon_palka, epsilon, Ed;
        Ap = PI * D * D / 4;
        Vsg = Gas.q / Ap; //3.11
        Vsl = Liquid.q / Ap;
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        R = Vsl / Vsg;//4.14
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        rho_n = lambda_L * Liquid.rho + Gas.rho * (1 - lambda_L);//3.23
        N_v = (pow(rho_n, 2) * pow(res.fluidMeanVelocity, 4)) / (g * PhaseInteract.lgSurfaceTension * (Liquid.rho - Gas.rho));//4.12
        N_D = g * (Liquid.rho - Gas.rho) * pow(D, 2) / PhaseInteract.lgSurfaceTension;//4.13
        B = 0.0814 * (1 - 0.0554 * log(1 + 730 * R / (R + 1)));//4.16
        HL = 1 - (1 - exp(-2.314 * (N_v * pow(1 + 205 / N_D, B)))) / (R + 1);//4.15
        rho_s = Liquid.rho * HL + Gas.rho * (1 - HL);//3.22
        epsilon_palka = 28.5 * PhaseInteract.lgSurfaceTension / (rho_n * pow(res.fluidMeanVelocity, 2));//4.18
        if (R >= 0.007)
        {
            epsilon = epsilon_palka;
        }
        else
        {
            epsilon = Roughness + R * (epsilon_palka - Roughness) / 0.007;
        }
        Ed = Roughness / D;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradient = res.frictionFactor * rho_n * pow(res.fluidMeanVelocity, 2) / (2 * D) + rho_s * g - pow(rho_n, 2) / rho_n;//4.11
        return res;
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

    void FlowMapOrkizhevskiy::setLiquid(double qo_ny, double qw_ny, double Bo, double Bw, double mu_o, double mu_w, double rho_o, double rho_w)
    {
        double qo = qo_ny * Bo;
        double qw = qw_ny * Bw;

        double fo = qo / (qw + qo);
        double fw = 1 - fo;

        mainFase = DefineMainFase(mu_o, fw);
        liquid.q = qo + qw;
        liquid.mu = mu_o * fo + mu_w * fw;
        liquid.rho = rho_o * fo + rho_w * fw;
        liquid.rho_sc = liquid.rho;
    }

    void FlowMapOrkizhevskiy::setGas(double qg_ny, double qo_ny, double qw_ny, double mu_g, double Rs, double Rsw, double Bg, double rho_g)
    {
        gas.mu = mu_g;
        gas.q = (qg_ny - qo_ny * Rs - qw_ny * Rsw) * Bg;
        gas.rho = rho_g;
        gas.rho_sc = gas.rho;
    }


    void FlowMapOrkizhevskiy::setPhaseInteract(double SurfaceTension)
    {
        phaseInteract.lgSurfaceTension = SurfaceTension;
    }

    void FlowMapOrkizhevskiy::defineTypeOfFlow()
    {
    }

    Result FlowMapOrkizhevskiy::calc(
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        Result res;
        double Lb, Ap, lambda_L, Ngv, Ngvstr, Nlv, Ngvtrm;
        Ap = PI * D * D / 4;
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap;
        Lb = 1.071 - 0.2218 * pow(res.fluidMeanVelocity / 0.3048, 2) * 0.3048 / D;//4.59
        if (Lb < 0.13)
        {
            Lb = 0.13;
        }
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8

        Ngv = (gas.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (liquid.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c
        //res = GreyMethod(Liquid, Gas, PhaseInteract, D, Roughness, Angle, PInflow, TInflow);
        if (1 - lambda_L <= Lb)
        {
            res = BubbleMode(liquid, gas, phaseInteract, D, Roughness, Angle, PInflow, TInflow);

            std::cout << "BubbleMode\n";
        }
        else if (Ngv < Ngvstr)
        {
            res = CorkMode(liquid, gas, phaseInteract, D, Roughness, Angle, PInflow, TInflow);
            res.flowPattern = FlowPattern::CorkMode;
            std::cout << "CorkMode\n";
        }

        else if (Ngvstr < Ngv && Ngv < Ngvtrm)
        {
            res = TransitionalMode(liquid, gas, phaseInteract, D, Roughness, Angle, PInflow, TInflow);
            res.flowPattern = FlowPattern::TransitionalMode;
            std::cout << "TransitionalMode\n";
        }
        else
        {
            res = EmulsionMode(liquid, gas, phaseInteract, D, Roughness, Angle, PInflow, TInflow);
            res.flowPattern = FlowPattern::EmulsionMode;
            std::cout << "EmulsionMode\n";
        }

        return res;
    }

   
} // namespace flowmaps
