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
        double pn, lambda_L, mu_n;
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8
        pn = liquid.rho * lambda_L + gas.rho * (1 - lambda_L);//3.23
        mu_n = liquid.mu * lambda_L + gas.mu * (1 - lambda_L);//3.23
        res.Re = pn * res.fluidMeanVelocity * d / mu_n;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradientFriction = ((res.liquidVelocity + Vb2) / (res.fluidMeanVelocity + Vb2) + liqDistribCoef) * res.frictionFactor * liquid.rho * pow(res.fluidMeanVelocity, 2) / (2 * d);//4.79
        res.pressureGradient = res.pressureGradientFriction + rho_s * g;//4.31

        return res;
    }

    Result FlowMapOrkizhevskiy::EmulsionMode()
    {
        Result res;
        double Ap, N_we, N_mu, Ed, Vsg, E_k, rho_n, rho_s;
        res.flowPattern = FlowPattern::EmulsionMode;
        Ap = PI * d * d / 4;
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

    Result FlowMapOrkizhevskiy::TransitionalMode()
    {
        Result resCork, resEmul, res;
        double Ngvstr, Ngvtrm, Ap, Ngv, Nlv, A;
        res.flowPattern = FlowPattern::TransitionalMode;
        Ap = PI * d * d / 4;
        resCork = CorkMode();
        resEmul = EmulsionMode();
        Ngv = (gas.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (liquid.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c
        A = (Ngvtrm - Ngv) / (Ngvtrm - Ngvstr);//4.56
        res.pressureGradient = A * resCork.pressureGradient + (1 - A) * resEmul.pressureGradient;
        return res;
    }

    double FlowMapOrkizhevskiy::MethodMarch(double Length)
    {
        double deltaP = 0.0;
        double pIn = pInflow;
        for (int i = 0; i < Length; i++)
        {
            double difP = calc().pressureGradient;
            pIn -= difP;
            pInflow = pIn;
            deltaP += difP;            
        }
        cout << "P now " << pIn << "|| delta now = " << deltaP << "\n";
        return deltaP;
    }

    Result FlowMapOrkizhevskiy::GreyMethod()
    {
        Result res;
        double R, Ap, Vsg, Vsl, rho_n, lambda_L, N_v, N_D, B, HL, rho_s, epsilon_palka, epsilon, Ed;
        Ap = PI * d * d / 4;
        Vsg = gas.q / Ap; //3.11
        Vsl = liquid.q / Ap;
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap;
        R = Vsl / Vsg;//4.14
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8
        rho_n = lambda_L * liquid.rho + gas.rho * (1 - lambda_L);//3.23
        N_v = (pow(rho_n, 2) * pow(res.fluidMeanVelocity, 4)) / (g * phaseInteract.lgSurfaceTension * (liquid.rho - gas.rho));//4.12
        N_D = g * (liquid.rho - gas.rho) * pow(d, 2) / phaseInteract.lgSurfaceTension;//4.13
        B = 0.0814 * (1 - 0.0554 * log(1 + 730 * R / (R + 1)));//4.16
        HL = 1 - (1 - exp(-2.314 * (N_v * pow(1 + 205 / N_D, B)))) / (R + 1);//4.15
        rho_s = liquid.rho * HL + gas.rho * (1 - HL);//3.22
        epsilon_palka = 28.5 * phaseInteract.lgSurfaceTension / (rho_n * pow(res.fluidMeanVelocity, 2));//4.18
        if (R >= 0.007)
        {
            epsilon = epsilon_palka;
        }
        else
        {
            epsilon = roughness + R * (epsilon_palka - roughness) / 0.007;
        }
        Ed = roughness / d;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradient = res.frictionFactor * rho_n * pow(res.fluidMeanVelocity, 2) / (2 * d) + rho_s * g - pow(rho_n, 2) / rho_n;//4.11
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

    void FlowMapOrkizhevskiy::setD(double D)
    {
        d = D;
    }

    void FlowMapOrkizhevskiy::setRoughness(double r)
    {
        roughness = r;
    }

    void FlowMapOrkizhevskiy::setAngle(double An)
    {
        angle = An;
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



    void FlowMapOrkizhevskiy::setArguments(double D, double Roughness, double Angle, double PInflow, double TInflow)
    {
        d = D;
        roughness = Roughness;
        angle = Angle;
        pInflow = PInflow;
        tInflow = TInflow;
    }

    Result FlowMapOrkizhevskiy::calc()
    {
        Result res;
        double Lb, Ap, lambda_L, Ngv, Ngvstr, Nlv, Ngvtrm;
        Ap = PI * d * d / 4;
        res.fluidMeanVelocity = (liquid.q + gas.q) / Ap;
        Lb = 1.071 - 0.2218 * pow(res.fluidMeanVelocity / 0.3048, 2) * 0.3048 / d;//4.59
        if (Lb < 0.13)
        {
            Lb = 0.13;
        }
        lambda_L = liquid.q / (liquid.q + gas.q);//3.8

        Ngv = (gas.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.4
        Nlv = (liquid.q / Ap) * pow(liquid.rho / (g * phaseInteract.lgSurfaceTension), 1 / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c

        if (1 - lambda_L <= Lb)
        {
            res = BubbleMode();
            res.flowPattern = FlowPattern::BubbleMode;
        }
        else if (Ngv < Ngvstr)
        {
            res = CorkMode();
            res.flowPattern = FlowPattern::CorkMode;
        }

        else if (Ngvstr < Ngv && Ngv < Ngvtrm)
        {
            res = TransitionalMode();
            res.flowPattern = FlowPattern::TransitionalMode;
        }
        else
        {
            res = EmulsionMode();
            res.flowPattern = FlowPattern::EmulsionMode;
            cout << "EmulsionMode";
        }

        return res;
    }

        vector<vector<int>>  FlowMapOrkizhevskiy::fillMap()
    {
        std::vector<std::vector<int>> Array;
        double t, N_gv,N_Lv;
        FlowPattern flowPattern;
        t = pow(liquid.rho / (phaseInteract.lgSurfaceTension * g), 1. / 4);
        //cout << t << "\n"; //debug?
        for (N_Lv = 0.1; N_Lv <= 100; N_Lv += 0.1) // Ngv �� �, � Nlv �� �
        {
            std::vector<int> Temp;
            for (N_gv = 0.5; N_gv <= 100; N_gv += 0.5) 
            {
                flowPattern = modeSelection( N_gv, N_Lv);
                switch (flowPattern)
                {
                case flowmaps::FlowPattern::BubbleMode:
                    Temp.push_back(0);
                    break;
                case flowmaps::FlowPattern::CorkMode:
                    Temp.push_back(1);
                    break;
                case flowmaps::FlowPattern::TransitionalMode:
                    Temp.push_back(2);
                    break;
                case flowmaps::FlowPattern::EmulsionMode:
                    Temp.push_back(3);
                    break;
                default:
                    Temp.push_back(4);
                    break;
                }
               
            }
            for (N_gv = 101; N_gv <= 1000; N_gv += 1) 
            {
                flowPattern = modeSelection( N_gv, N_Lv);
                switch (flowPattern)
                {
                case flowmaps::FlowPattern::BubbleMode:
                    Temp.push_back(0);
                    break;
                case flowmaps::FlowPattern::CorkMode:
                    Temp.push_back(1);
                    break;
                case flowmaps::FlowPattern::TransitionalMode:
                    Temp.push_back(2);
                    break;
                case flowmaps::FlowPattern::EmulsionMode:
                    Temp.push_back(3);
                    break;
                default:
                    Temp.push_back(4);
                    break;
                }
            }
            Array.push_back(Temp);
        }
        return Array;
        //������������� � ���� � ����������� �� ������
    }

    FlowPattern FlowMapOrkizhevskiy::modeSelection(
        double N_gv,
        double N_Lv
        )
    {
        Result res;
        double lambda_B, v_m, lambda_L, Ngvstr, Ngvtrm,t;
        FlowPattern flowPattern;

        t = pow(liquid.rho / (phaseInteract.lgSurfaceTension*g), 1. / 4);
        v_m = t/ N_gv + t/ N_Lv;
        lambda_B = 1.071 - 0.2218 * pow(v_m / 0.3048, 2) * 0.3048 / d;//4.59
        if (lambda_B < 0.13)
        {
            lambda_B = 0.13;
        }
        //lambda_L = N_gv / t;
        lambda_L = N_Lv / (N_gv + N_Lv);

        Ngvstr = 50 + 36 * N_Lv;//4.32b
        Ngvtrm = 75 + 84 * pow(N_Lv, 0.75);//4.32c
       
        if (1 - lambda_L <= lambda_B)
        {
            flowPattern = FlowPattern::BubbleMode;
        }
        else if (N_gv < Ngvstr)
        {
            flowPattern = FlowPattern::CorkMode;
        }
        else if (Ngvstr < N_gv && N_gv < Ngvtrm)
        {
            flowPattern = FlowPattern::TransitionalMode;
        }
        else
        {
            flowPattern = FlowPattern::EmulsionMode;
        }

        return flowPattern;
    }
   
} // namespace flowmaps
