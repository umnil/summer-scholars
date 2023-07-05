# import modules
import pygame
from serial import Serial
from serial.tools.list_ports import comports
from scanf import scanf

# Find our joystick controller using the product and vendor id of the board
vendor_id = 9025
product_id = 66
ports = comports()
port = [p for p in ports if (p.vid == vendor_id) and (p.pid == product_id)][0]

# Initalize our serial port to communicate with the board
com_port = Serial(port.device, 19200, timeout=0.01)

# Initialize PyGame library
pygame.init()

# Create a window to play in
win = pygame.display.set_mode((500, 500))
pygame.display.set_caption("Moving")

# Set our player position
x = 200
y = 200

# set our player size
w = 20
h = 20

# velocity
v_slow = 5
v_fast = 10
v = v_slow

# Game state. Once this is true we stop the game
done = False

# Clear any data collected from the joystick
com_port.reset_input_buffer()
while not done:

    # Provide some time for GUI processing
    pygame.time.delay(1)

    # Process any OS events, like pressing the exit button on the window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True

    # Read data from the joystick
    # Because the joystick sends data faster than we receive it, we basically just
    # throw everything away and just focus on the most recent data
    com_port.reset_input_buffer()  # Throw it all away
    com_port.readline()  # This cleans the buffer to start on a new line
    data = com_port.readline()  # Read the next most available data

    if len(data) < 1:
        print("No data received")
        continue

    print(data)
    xi, yi, si = scanf("%f, %f, %d\r\n", data.decode())
    v = v_slow if not si else v_fast
    x += xi * v
    y -= yi * v  # Axis is inverted

    # Handle Edge Problems
    if y < 0:
        y = 0
    elif y > 500 - h:
        y = 500 - h
    if x < 0:
        x = 0
    elif x > 500 - w:
        x = 500 - w

    # Fill the window with color
    win.fill((1, 1, 1))

    # Draw our player in their position
    pygame.draw.rect(win, (255, 0, 0), (x, y, w, h))
    pygame.display.update()

pygame.quit()
