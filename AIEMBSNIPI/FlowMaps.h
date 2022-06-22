

/*
        Карты режимов течения
*/
#pragma once
#include <iostream>
#include <cstdint>
#include <memory>
using namespace std;

namespace flowmaps
{
    // Режимы горизонтального потока, принятые в PipeSim с поправками из исходником Begs-Brill
    enum class FlowPattern
    {
        BubbleMode = 0, // пузырьковый 
        CorkMode, // пробковый
        TransitionalMode, // переходный
        EmulsionMode, //эмульсионный  
        Unknown
    };

    enum class MainFase
    {
        Water,
        Oil
    };

    struct PhaseInfo
    {
        // Плотность при н.у.
        double rho_sc;
        // Плотность
        double rho;
        // Вязкость
        double mu;
        // Объемный расход в текущих условиях
        double q;
    };

    struct PhaseInteract
    {
        double lgSurfaceTension;
        //// Пов. натяжение нефти
        //double ogSurfaceTension;
        //// Пов. натяжение воды
        //double wgSurfaceTension;
        //// Доля воды в смеси в стоячем флюиде (когда массовый расход равен нулю)
        //double NoFlowWaterCut;
        //// Корреляция для расчета вязкости смести нефти и воды

        ///correlation::OilWaterMixingViscosityPtr oil_water_visc;
    };

    /// Результаты расчета свойств потока в точке
    struct Result
    {
        // Режим течения
        FlowPattern flowPattern;
        // Градиент давления (суммарный, потери+гравитация+ускорение)
        double pressureGradient;
        // Градиент за счет трения
        double pressureGradientFriction;
        // Градиент за счет гидростатики
        double pressureGradientElevation;
        // Градиент за счет ускорения
        double pressureGradientAcc;
        // Объемная доля жидкости
        double liquidHoldup;
        // Средняя скорость флюида
        double fluidMeanVelocity;
        // Скорость жидкости
        double liquidVelocity;
        // Скорость газа
        double gasVelocity;
        // Поверхностное натяжение жидкости
        double liquidSurfaceTension;
        // Число Рейнольдса для жидкости
        double Re_L;
        // Число Рейнольдса для газа
        double Re_G;
        // Среднее число Рейнольдса
        double Re;
        // Вязкость жидкости
        double Mu_L;
        // Вязкость газа
        double Mu_G;
        // Коэффициент трения
        double frictionFactor;
        // Доля жидкости (для flow map)
        double N_l;

        Result() :
            flowPattern(FlowPattern::Unknown),
            pressureGradient(NAN),
            liquidHoldup(NAN),
            N_l(NAN),
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
            const PhaseInfo& Water,
            const PhaseInfo& Oil,
            const PhaseInfo& Gas,
            const PhaseInteract& PhaseInteract,
            double D,
            double Roughness,
            double Angle,
            double PInflow,
            double TInflow) = 0;
    };

    using FlowMapLiquidGasPtr = std::shared_ptr<IFlowMapLiquidGas>;


} // namespace flowmaps
