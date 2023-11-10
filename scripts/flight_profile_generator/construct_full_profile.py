accel = open("outputs/acceleration_Full_metric.csv")
hscm = open("outputs/hscm_pressure.csv")
pvc = open("outputs/pvc_pressure.csv")
time = open("outputs/time.csv")

hscm = list(map(float,hscm.read().split(",")))
pvc = list(map(float,pvc.read().split(",")))
time = list(map(float,time.read().split(",")))

output = open("outputs/profile_full_metric.csv", "w")

for i in range(len(time)):
    a = float(accel.readline().split(",")[1])
    t = time[i]
    p = pvc[i]
    h = hscm[i]
    output.write(f"{t},{h},{p},{a},0,0\n")

output.close()
