from os import path

# Tile types.

TILE_MAPPING = {
    ' ': 0,  # Ground
    'S': 0,  # Start
    'F': 0,  # Finish
    'D': 0,  # Dynamite
    '1': 1,  # Stone
    '2': 2,  # Wood
    '3': 3,  # Dynamite
    '#': 4   # Outer wall
}

# Main script.

def main():

    # Enter the level's name (or default to "output").

    while True:
        name = input('Name:   ') or 'output'
        name = name.lower().replace(' ', '_')

        if len(name) > 24:
            print(' (!) Name must be up to 24 characters long.')
            continue

        if not all(ord(char) < 128 for char in name):
            print(' (!) Name must only use ASCII characters.')
            continue

        break

    # Enter the level's biome (or default to "grass").

    while True:
        biome = input('Biome:  ') or 'grass'
        biome = biome.lower().replace(' ', '_')

        if len(biome) > 24:
            print(' (!) Biome must be up to 24 characters long.')
            continue

        if not all(ord(char) < 128 for char in biome):
            print(' (!) Biome must only use ASCII characters.')
            continue

        break

    # Enter the level's width.

    while True:
        try:
            width = int(input('Width:  '))

        except ValueError:
            print(' (!) Width must be an integer between 4 and 32.')
            continue

        if width < 4 or width > 32:
            print(' (!) Width must be between 4 and 32.')
            continue

        break

    # Enter the level's height.

    while True:
        try:
            height = int(input('Height: '))

        except ValueError:
            print(' (!) Height must be an integer between 4 and 32.')
            continue

        if height < 4 or height > 32:
            print(' (!) Height must be between 4 and 32.')
            continue

        break

    # Enter the tilemap.

    print('\nTilemap:')

    tiles = [0] * (width * height)
    start = None
    finish = None
    dynamites = []

    for y in reversed(range(height)):
        row = input(' ')[:width].ljust(width, '#')

        for x, tile in enumerate(row):
            tile = tile.upper()

            # Validate tile and add to the tiles list.

            if tile not in TILE_MAPPING:
                print(f'\n(!) Invalid tile symbol "{tile}".')
                return

            tiles[y * width + x] = TILE_MAPPING[tile]

            # Set the start position.

            if tile == 'S':
                start = (x, y)

            # Set the finish position.

            if tile == 'F':
                finish = (x, y)

            # Add a dynamite position.

            if tile == 'D':
                dynamites.append((x, y))

    # Validate that the level is enclosed.

    border = tiles[:width] + tiles[-width:]

    for y in range(1, height - 1):
        row = tiles[y * width:(y + 1) * width]

        border.append(row[0])
        border.append(row[-1])

    if border.count(4) != len(border):
        print('\n(!) The level is not enclosed.')
        return

    # Validate that the start and finish are set.

    if start is None:
        print('\n(!) Starting tile is not set.')
        return

    if finish is None:
        print('\n(!) Finish tile is not set.')
        return

    # Write level data to a file.

    directory = path.join(path.split(path.dirname(__file__))[0], 'levels')

    with open(f'{directory}/{name}.level', 'wb+') as file:
        file.write(len(biome).to_bytes(4, 'little'))
        file.write(bytes(biome, 'ASCII'))

        file.write(width.to_bytes(4, 'little'))
        file.write(height.to_bytes(4, 'little'))

        file.write(start[0].to_bytes(4, 'little'))
        file.write(start[1].to_bytes(4, 'little'))

        file.write(finish[0].to_bytes(4, 'little'))
        file.write(finish[1].to_bytes(4, 'little'))

        file.write(len(dynamites).to_bytes(4, 'little'))

        for dynamite in dynamites:
            file.write(dynamite[0].to_bytes(4, 'little'))
            file.write(dynamite[1].to_bytes(4, 'little'))

        file.write(bytes(tiles))

        print(f'\nSaved as "{name}.level"!')

# Entry point.

if __name__ == '__main__':
    main()