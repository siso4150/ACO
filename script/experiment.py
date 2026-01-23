import subprocess

config_file = ["rho0.1.json", "rho0.5.json", "rho0.9.json"]

for conf in config_file:
    subprocess.run(["./aco","config_json/" + conf])
print("batch experiments completed")