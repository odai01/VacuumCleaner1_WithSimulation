import pygame
import json
import math

# Load data from the JSON files(steps history and the house matrix)
with open('steps_history.json') as f:
    steps_history = json.load(f)
with open('initial_house.json') as f:
    house_matrix = json.load(f)

# Initialize the Pygame
pygame.init()

rows = len(house_matrix)
cols = len(house_matrix[0])

# display dimensions
block_size = 50 # if we want to enlargen the house we can increase block size
margin = 20
width = cols * block_size + 2 * margin
height = rows * block_size + 2 * margin

# Center the house on the screen
screen_width = max(width, 800)  
screen_height = max(height, 600)
window = pygame.display.set_mode((screen_width, screen_height + 200))  # Extra space for legend and buttons
pygame.display.set_caption("Vacuum Cleaner Simulation")

# Colors we will use
MAROON = (128, 0, 0)
LIGHTYELLOW = (250, 250, 210)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GOLD = (255, 215, 0)
WALL_COLOR = (0, 0, 128)
GREEN = (107, 142, 35)
RED = (255, 99, 71)

docking_station = steps_history[0] if steps_history else None

# Function to calculate gray color based on dirt level
def get_dirt_color(level):
    if level < 0 or level > 9:
        return WHITE
    gray_value = 255 - (level * 25)
    return (gray_value, gray_value, gray_value)

# Function to draw the house grid
def draw_grid(charging):
    start_x = (screen_width - width) // 2
    start_y = (screen_height - height) // 2
    for i in range(rows):
        for j in range(cols):
            if house_matrix[i][j] == -1:
                color = WALL_COLOR
            else:
                color = get_dirt_color(house_matrix[i][j])
            rect = pygame.Rect(start_x + j * block_size, start_y + i * block_size, block_size, block_size)
            pygame.draw.rect(window, color, rect)
            if house_matrix[i][j] > 0:
                font = pygame.font.SysFont(None, 24)
                text = font.render(str(house_matrix[i][j]), True, BLACK)
                window.blit(text, (start_x + j * block_size + block_size // 2 - text.get_width() // 2,
                                   start_y + i * block_size + block_size // 2 - text.get_height() // 2))
            if (i, j) == (docking_station['x'], docking_station['y']):
                draw_docking_station_symbol(start_x + j * block_size, start_y + i * block_size, charging)

# Function to draw the docking station symbol: the Plus sign
def draw_docking_station_symbol(x, y, charging):
    color = GOLD if charging else BLACK #when charging it turns gold
    pygame.draw.line(window, color, (x + 7, y + 5), (x + 14, y + 5), 2) 
    pygame.draw.line(window, color, (x + 10, y + 2), (x + 10, y + 9), 2) 

# Function to draw the legend
def draw_legend():
    font = pygame.font.SysFont(None, 24)
    legend_items = [
        ("Wall", WALL_COLOR),
        ("Vacuum Cleaner", GOLD),
        ("Dirt Levels", BLACK),
        ("Docking Station", WHITE)
    ]
    x, y = margin, screen_height - 180
    for text, color in legend_items:
        if text == "Vacuum Cleaner":
            pygame.draw.circle(window, color, (x + block_size // 2, y + block_size // 2), block_size // 4)
            pygame.draw.line(window, MAROON, 
                             (x + block_size // 2, y + block_size // 2), 
                             (x + block_size // 2 + block_size // 6 * math.cos(math.radians(0)),
                              y + block_size // 2 + block_size // 6 * math.sin(math.radians(0))), 2)
            legend_text = font.render(text, True, BLACK)
            window.blit(legend_text, (x + block_size + 15, y + block_size // 2 - legend_text.get_height() // 2))
        elif text == "Dirt Levels":
            for level in range(10):
                dirt_color = get_dirt_color(level)
                pygame.draw.rect(window, dirt_color, (x + level * (block_size // 10), y, block_size // 10, block_size))
            legend_text = font.render(text, True, BLACK)
            window.blit(legend_text, (x + block_size + 15, y + block_size // 2 - legend_text.get_height() // 2))
        elif text == "Docking Station":
            pygame.draw.rect(window, WHITE, (x, y, block_size, block_size))
            draw_docking_station_symbol(x, y, False)  # Legend doesn't flash
            legend_text = font.render(text, True, BLACK)
            window.blit(legend_text, (x + block_size + 15, y + block_size // 2 - legend_text.get_height() // 2))
        else:
            pygame.draw.rect(window, color, (x, y, block_size, block_size))
            legend_text = font.render(text, True, BLACK)
            window.blit(legend_text, (x + block_size + 15, y + block_size // 2 - legend_text.get_height() // 2))
        y += block_size + 20

# Function to draw buttons
def draw_buttons():
    font = pygame.font.SysFont(None, 24)
    buttons = [
        ("Resume", GREEN, (screen_width // 2 - 100, screen_height + 40, 80, 40)),
        ("Pause", RED, (screen_width // 2 + 20, screen_height + 40, 80, 40))
    ]
    for text, color, rect in buttons:
        pygame.draw.rect(window, color, rect)
        button_text = font.render(text, True, BLACK)
        window.blit(button_text, (rect[0] + rect[2] // 2 - button_text.get_width() // 2,
                                  rect[1] + rect[3] // 2 - button_text.get_height() // 2))

# Run the simulation
running = True
paused = False
step_index = 0
clock = pygame.time.Clock()

prev_x, prev_y = None, None

angle = 0

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouse_pos = pygame.mouse.get_pos()
            if screen_width // 2 - 100 <= mouse_pos[0] <= screen_width // 2 - 20 and screen_height + 40 <= mouse_pos[1] <= screen_height + 80:
                paused = False  # Resume button clicked
            elif screen_width // 2 + 20 <= mouse_pos[0] <= screen_width // 2 + 100 and screen_height + 40 <= mouse_pos[1] <= screen_height + 80:
                paused = True  # Pause button clicked
    
    window.fill(LIGHTYELLOW)
    charging = False
    if step_index < len(steps_history):
        charging = (docking_station['x'] == steps_history[step_index]['x'] and
                    docking_station['y'] == steps_history[step_index]['y'] and
                    prev_x == docking_station['x'] and
                    prev_y == docking_station['y'] and
                    house_matrix[docking_station['x']][docking_station['y']] == 0)
    draw_grid(charging)
    draw_legend()
    draw_buttons()

    if not paused and step_index < len(steps_history):
        # Draw the vacuum cleaner
        x, y = steps_history[step_index]['x'], steps_history[step_index]['y']
        start_x = (screen_width - width) // 2
        start_y = (screen_height - height) // 2
        vacuum_center = (start_x + y * block_size + block_size // 2,
                         start_y + x * block_size + block_size // 2)
        pygame.draw.circle(window, GOLD, vacuum_center, block_size // 4)

        # Draw a line to indicate direction (rotation)
        line_length = block_size // 6
        end_x = vacuum_center[0] + line_length * math.cos(math.radians(angle))
        end_y = vacuum_center[1] + line_length * math.sin(math.radians(angle))
        pygame.draw.line(window, MAROON, vacuum_center, (end_x, end_y), 2)

        # the angle is used for the rotating movement for the vacuum
        angle += 10

        # Decrementing the dirt level when staying in the same place, since this means the vacuum is cleaning
        if ((prev_x == None and prev_y == None and house_matrix[x][y] > 0)
            or (prev_x == x and prev_y == y and house_matrix[x][y] > 0)):
            house_matrix[x][y] -= 1

        prev_x, prev_y = x, y
        step_index += 1

    # Drawing the vacuum cleaner at its last position when paused or finished
    if (paused or step_index >= len(steps_history)) and prev_x is not None and prev_y is not None:
        vacuum_center = (start_x + prev_y * block_size + block_size // 2,
                         start_y + prev_x * block_size + block_size // 2)
        pygame.draw.circle(window, GOLD, vacuum_center, block_size // 4)
        end_x = vacuum_center[0] + line_length * math.cos(math.radians(angle))
        end_y = vacuum_center[1] + line_length * math.sin(math.radians(angle))
        pygame.draw.line(window, MAROON, vacuum_center, (end_x, end_y), 2)

    # Displaying "End of Simulation" when done
    if step_index >= len(steps_history):
        font = pygame.font.SysFont(None, 48)
        end_text = font.render("End of Simulation", True, BLACK)
        window.blit(end_text, (screen_width // 2 - end_text.get_width() // 2, 20))

    pygame.display.flip()
    clock.tick(6)  # incrementing the clock tick will make the vacuum faster

pygame.quit()
