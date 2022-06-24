#include "FLowMapOrk.h"
#define PI 3.14159265358979323846
#define g 9.806
namespace flowmaps
{

    FlowMapOrkizhevskiy::FlowMapOrkizhevskiy()
    {

    }

    Result FlowMapOrkizhevskiy::BubbleMode(
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
        double Vs, 
            HL, // ОбЪемное содержание жидкости c учетом эффекта проскальзования
            Ap, // Площадь сечения
            Ed; // Шероховатость на диаметр 
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

    Result FlowMapOrkizhevskiy::CorkMode(
        const PhaseInfo& Liquid,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow,
        MainFase mainFase)
    {
        Result res;
        double Ap, // Площадь сечения, м^2
            Ed, // Шероховатость на диаметр
            Vb1, // Скорость подъема пузырька, м/c
            Vb2, // Скорость подъема пузырька, м/c
            Vbs, // Скорость пузыря в статической жидкости, м/c
            liqDistribCoef, // Коэффициент распределения жидкости Г
            X, 
            rho_s, // Плотность потока, кг/м^3
            rho_n, // Плотность без учета эффекта проскальзования, кг/м^3
            lambda_L, // Объемное содержание жидкости без учета эффекта проскальзования
            mu_n,// Вязкость без учета эффекта проскальзования, Па*с
            Re_B; 
        
        res.flowPattern = FlowPattern::CorkMode;
        Ap = PI * D * D / 4;
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        res.liquidVelocity = Liquid.q / Ap;//Vsl
        res.gasVelocity = Gas.q / Ap;
        res.Re_L = Liquid.rho * res.fluidMeanVelocity * D / Liquid.mu;//4.66
        Vb2 = 0.5 * sqrt(g * D);//4.71
        Re_B = Liquid.rho * Vb2 * D / Liquid.mu;//4.66
        res.Re_G = Liquid.rho * Vb2 * D / Liquid.mu;//4.66

        do
        {
            Vb1 = Vb2;
            if (res.Re_G <= 3000)
            {
                Vb2 = (0.546 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.67
            }
            else if (res.Re_G >= 8000)
            {
                Vb2 = (0.35 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.68
            }
            else
            {
                Vbs = (0.251 + 8.74 * 0.000001 * res.Re_L) * sqrt(g * D);//4.70
                Vb2 = 0.5 * (Vbs + sqrt(pow(Vbs, 2) + 13.59 * Liquid.mu / (Liquid.rho * sqrt(D))));//4.69
            }
            res.Re_G = Liquid.rho * Vb2 * D / Liquid.mu;//4.66
        } while (abs(Vb1 - Vb2) / Vb1 > 0.05);

        if (mainFase == MainFase::Oil)
        {
            if (res.fluidMeanVelocity < 3)//liqDistribCoef = 
            {
                liqDistribCoef = 0.0127 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.415) - 0.284 + 0.167 * log10(res.fluidMeanVelocity / 0.3048) + 0.113 * log10(D / 0.3048);// 4.74          
            }
            else
            {
                X = -log10(res.fluidMeanVelocity) * (0.01 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.571) + 0.397 + 0.63 * log10(D / 0.3048));//4.76
                liqDistribCoef = 0.0274 * log10(Liquid.mu + 1) / pow(D / 0.3048, 1.371) + 0.161 + 0.569 * log10(D / 0.3048) + X;//4.75
            }
        }
        else if (mainFase == MainFase::Water)
        {
            if (res.fluidMeanVelocity < 3)//liqDistribCoef = 
            {
                liqDistribCoef = 0.013 * log10(Liquid.mu) / pow(D / 0.3048, 1.38) - 0.681 + 0.232 * log10(res.fluidMeanVelocity / 0.3048) + 0.428 * log10(D / 0.3048);// 4.72       
            }
            else
            {
                liqDistribCoef = 0.045 * log10(Liquid.mu) / pow(D / 0.3048, 0.799) - 0.709 + 0.162 * log10(res.fluidMeanVelocity / 0.3048) + 0.888 * log10(D / 0.3048);// 4.73    
            }
        }

        rho_s = (Liquid.rho * (res.liquidVelocity + Vb2) + Gas.rho * res.gasVelocity) / (res.fluidMeanVelocity + Vb2) + Liquid.rho * liqDistribCoef;//4.63

        Ed = Roughness / D;
        
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        rho_n = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.23
        mu_n = Liquid.mu * lambda_L + Gas.mu * (1 - lambda_L);//3.21
        res.Re = rho_n * res.fluidMeanVelocity * D / mu_n;
        res.frictionFactor = pow(-2 * log10(2 * Ed / 3.7 - (5.02 / res.Re) * log10(2 * Ed / 3.7 + 13 / res.Re)), -2);//2.19
        res.pressureGradientFriction = ((res.liquidVelocity + Vb2) / (res.fluidMeanVelocity + Vb2) + liqDistribCoef) * res.frictionFactor * Liquid.rho * pow(res.fluidMeanVelocity, 2) / (2 * D);//4.79
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
        double Ap, // Площадь сечения, м^2
            N_we, // Число Вебера
            N_mu, // Безразмерный показатель, зависящий от вязкости
            Ed, // Шероховатость на диаметр
            Vsg, // Приведенная скорость газа
            E_k, // Кинетическая энергия, Дж
            rho_n, // Плотность без учета эффекта проскальзования, кг/м^3
            rho_s, // Плотность без учета эффекта проскальзования, кг/м^3
            lambda_L, // Объемное  содержание жидкости без учета эффекта проскальзования
            mu_n; // Вязкость без учета эффекта проскальзования, Па*с
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

        
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8
        rho_n = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.23
        mu_n = Liquid.mu * lambda_L + Gas.mu * (1 - lambda_L);//3.23
        res.Re = rho_n * res.fluidMeanVelocity * D / mu_n;

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

   
        E_k = res.fluidMeanVelocity * Vsg * rho_n / PInflow;// 4.53
        rho_s = Liquid.rho * lambda_L + Gas.rho * (1 - lambda_L);//3.22
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
        double TInflow,
        MainFase mainFase)
    {
        Result resCork, resEmul, res;
        double Ngvstr, // Граница между пробковым и переходным 
            Ngvtrm, // Граница между переходным и эмульсионным
            Ap, // Площадь сечения, м^2
            Ngv, // Показатель скорости газа
            Nlv, // Показатель скорости газа 
            A; // Коэффициент линейной интерполяции
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
            TInflow,
            mainFase);
        resEmul = EmulsionMode(
            Liquid,
            Gas,
            PhaseInteract,
            D,
            Roughness,
            Angle,
            PInflow,
            TInflow);
        Ngv = (Gas.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1. / 4);//4.4
        Nlv = (Liquid.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1. / 4);//4.3
        Ngvstr = 50 + 36 * Nlv;//4.32b
        Ngvtrm = 75 + 84 * pow(Nlv, 0.75);//4.32c
        A = (Ngvtrm - Ngv) / (Ngvtrm - Ngvstr);//4.56
        res.pressureGradient = A * resCork.pressureGradient + (1 - A) * resEmul.pressureGradient;
        return res;
    }

    double FlowMapOrkizhevskiy::MethodMarch(
        double Length, 
        const PhaseInfo& Water,
        const PhaseInfo& Oil,
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
                Water,
                Oil,
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

    MainFase DefineMainFase(double mu_o, double fw) // Рис. 3.3
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
        const PhaseInfo& Water,
        const PhaseInfo& Oil,
        const PhaseInfo& Gas,
        const PhaseInteract& PhaseInteract,
        double D,
        double Roughness,
        double Angle,
        double PInflow,
        double TInflow)
    {
        Result res;
        double Lb, // Параметр уравнения
            Ap,  // Площадь сечения, м^2
            lambda_L, // Объемное  содержание жидкости без учета эффекта проскальзования
            Ngv, // Показатель скорости газа
            Ngvstr, // Граница между пробковым и переходным 
            Nlv, // Показатель скорости жидкость
            Ngvtrm; // Граница между переходным и эмульсионным
        Ap = PI * D * D/ 4;

        double fo = Oil.q / (Water.q + Oil.q);//доля нефти
        double fw = 1 - fo;//доля воды

        PhaseInfo Liquid;
        Liquid.q = Oil.q + Water.q;
        Liquid.mu = Oil.mu * fo + Water.mu * fw;
        Liquid.rho = Oil.rho * fo + Water.rho * fw;
        Liquid.rho_sc = Liquid.rho;

        MainFase mainFase = DefineMainFase(Oil.mu, fw);
        res.fluidMeanVelocity = (Liquid.q + Gas.q) / Ap;
        Lb = 1.071 - 0.2218 * pow(res.fluidMeanVelocity / 0.3048, 2) * 0.3048 / D;//4.59
        if (Lb < 0.13)
        {
            Lb = 0.13;
        }
        lambda_L = Liquid.q / (Liquid.q + Gas.q);//3.8

        Ngv = (Gas.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1. / 4);//4.4
        Nlv = (Liquid.q / Ap) * pow(Liquid.rho / (g * PhaseInteract.lgSurfaceTension), 1./ 4);//4.3
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
            cout << "BubbleMode";
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
                TInflow,
                mainFase);
            res.flowPattern = FlowPattern::CorkMode;
            cout << "CorkMode";
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
                TInflow,
                mainFase);
            res.flowPattern = FlowPattern::TransitionalMode;
            cout << "TransitionalMode";
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
            cout << "EmulsionMode";
        }

        return res;
    } 
} // namespace flowmaps
