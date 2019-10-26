#pragma once
class GBMProcess
{
public:
	GBMProcess() {}
	GBMProcess(double spot, double rf, double div, double vol) :
		spot_(spot), rf_(rf), div_(div), vol_(vol) {}
	double getSpot() { return spot_; }
	double getRf() { return rf_; }
	double getDiv() { return div_; }
	double getVol() { return vol_; }

	// Assignment.2
	void setSpot(double spot) { spot_ = spot; }
	void setRf(double rf) { rf_ = rf; }
	void setDiv(double div) { div_ = div; }
	void setVol(double vol) { vol_ = vol; }

	~GBMProcess() {};
private:
	double spot_, rf_, div_, vol_;
};
