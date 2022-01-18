from point import Point, norm, dot

p = Point(3, 4)
for x in range(3):
    for y in range(3):
        q = -Point(x, y)
        print(f'norm{q} = {norm(q)}')
        print(f'{p}・{q} = {dot(p, q)}')


p = Point(2, 3)
q = Point(4, 5)
p *= q
print(p)
print(q)
