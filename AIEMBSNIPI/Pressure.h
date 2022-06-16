#pragma once
namespace flowmaps {
	class Pressure
	{
	public:
		Pressure();
		double answer();
	private:
		PhaseInfo Liquid;
		PhaseInfo Gas;
		double Bo, Bg, Rs, D, qo, qw_ny, qo_ny, qg_ny, fo, fw, Bw, qw, mu_o, mu_w, rho_o, rho_w, Rsw, Roughness, Angle, PInflow, TInflow;
		//  начальные условия  

	};
}
