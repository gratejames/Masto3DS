
width = 4
height = 4
OpUnit = 4
# for i in range(width*height):
# 	x = i%width
# 	y = i//height
# 	print(f"{hex(i)}: ({x}, {y}) => ({x%2}, {y%2})")

for i in range(width*height):
	x = i%width
	y = i//height
# 	print(f"{hex(i)}: ({x}, {y}) => ({x//4*4}, {y//4*4})")


	cornerX = x//OpUnit*OpUnit
	cornerY = y//OpUnit*OpUnit
	mx = x%OpUnit
	my = y%OpUnit
	i2 = (my*width + mx) + cornerY*width + cornerX
	print(f"{hex(i)}: ({x}, {y}) => ({cornerX}, {cornerY}) + ({mx}, {my}) ({mx + cornerX == x and my + cornerY == y})")