from gpiozero import OutputDevice
import time

# The GPIO pins for connecting a motor
OUTPUT_PIN1_1 = 6
OUTPUT_PIN1_2 = 13
OUTPUT_PIN1_3 = 19
OUTPUT_PIN1_4 = 26

# Interval for switching a phase to be excited
TIME_SLEEP = 0.002

# Create output devices to rotate a motor
motor_pins = [
    OutputDevice(OUTPUT_PIN1_1),
    OutputDevice(OUTPUT_PIN1_2),
    OutputDevice(OUTPUT_PIN1_3),
    OutputDevice(OUTPUT_PIN1_4),
]

# A function for exciting a specified phase
def out_motor_pin(pin_num):
    for i in range(0, 4):
        if i == pin_num:
            motor_pins[i].on()
        else:
            motor_pins[i].off()

# Rotate a stepper motor with 1-phase excitation
i = 0
while True:
    # Excite phase `i` and go to the next phase
    out_motor_pin(i)
    i += 1
    if i >= 4:
        i = 0 # Return to phase 0 after one lap

    # Wait a while to prevent a motor from losing steps
    time.sleep(TIME_SLEEP)
