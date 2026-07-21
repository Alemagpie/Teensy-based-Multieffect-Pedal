import sys
import math

def convert_coeff(time_ms : float):
    return round((1.0 - math.exp(-1.0 / (44100 * time_ms / 1000))) * 32767.0)

print(f"Q15 coefficient for {sys.argv[1]} ms is: ", convert_coeff(float(sys.argv[1])))