Context "ODE"

Expect dGetVersion()=1.0,"gets version"
Expect dDemoTime()=1000,"gets fixed demo time"

world=dWorldCreate()
Expect world<>0,"can create world"

dWorldDestroy world
