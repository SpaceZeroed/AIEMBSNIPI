#include "Pressure.h"
#include "FlowMapOrk.h"
namespace flowmaps {
	Pressure::Pressure()
	{
        //������ �� ���� � ����� ��� ���
        qo_ny = 1590.0 / 86400;
        //������ �� ���� � ���� 
        qw_ny = 0;
        //������� 
        D = 0.1524;

        Bo = 1.197;
        Bg = 0.0091;
        Bw = 0.0;

        // �������� �����
        mu_o = 0.00097;
        // �������������
        Roughness = 0.000018288;
        PInflow = 117.13 * 100000;
        TInflow = 82;
        qo = qo_ny * Bo;
        qw = qw_ny * Bw;

        rho_o = 762.64;
        Rs = 50.6;

        fo = qo / (qw + qo);
        fw = 1 - fo;

        // �������� ������   
        Rsw = 0;
        rho_w = 1000;
        mu_w = 1;

        //������ � ���������
        Liquid.q = qo + qw;
        Liquid.mu = mu_o * fo + mu_w * fw;
        Liquid.rho = rho_o * fo + rho_w * fw;
        Liquid.rho_sc = Liquid.rho;

        //�������� � ����� 
        qg_ny = 283000.0 / 86400;
        Gas.mu = 0.000016;
        Gas.q = (qg_ny - qo_ny * Rs - qw_ny * Rsw) * Bg;
        Gas.rho = 94.19;
        Gas.rho_sc = Gas.rho;
        Angle = 90;
	}
    double Pressure::answer()
    {
        //������ � ��������������� ��� 
        PhaseInteract PhaseInteract;
        PhaseInteract.lgSurfaceTension = 0.00841;
        Result grad;
        // ����������
        FlowMapOrkizhevskiy myex = FlowMapOrkizhevskiy();
        grad = myex.calc(Liquid, Gas, PhaseInteract, D, 0.000018288, 90, 117.13 * 100000, 82);
        return grad.pressureGradient;
    }
}