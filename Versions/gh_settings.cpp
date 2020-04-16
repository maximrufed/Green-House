#include "gh_settings.h"


ControllerSettings GHSettings;


void InitSettings()
{
	//GHSettings.TSensorsUpdateIntervalMinutes = 1;

	GHSettings.TerraAccum.TDiffStart = 15;
	GHSettings.TerraAccum.TDiffStop = 5;
	GHSettings.TerraAccum.TMaxAirAtNightStop = 14;
	GHSettings.TerraAccum.TMinAirAtNightStart = 0;
	GHSettings.TerraAccum.TMaxAirAtDayStart = 35;
	GHSettings.TerraAccum.TMinAirAtDayStop = 10;
}

