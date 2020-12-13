from mip import Model, INTEGER, minimize

bus_input = "1789,37,47,1889"
buses = bus_input.split(",")

model = Model()
t = model.add_var(var_type=INTEGER)  # TIME - result

x = []
n_buses = 0
prod_buses = 1
for i in range(len(buses)):
    if buses[i] != "x":
        buses[i] = int(buses[i])
        x.append(model.add_var(var_type=INTEGER))
        model.add_constr(t == buses[i] * x[n_buses] - i)
        n_buses += 1
        prod_buses *= buses[i]

model.objective = minimize(t)

for i in range(len(x)):
    model.add_constr(x[i] >= 1)
model.add_constr(t >= 1)
model.add_constr(t <= prod_buses)

model.optimize()

print('')
print('Objective value: {model.objective_value:.3}'.format(**locals()))
print('Solution: ', end='')
for v in model.vars:
    if v.x > 1e-5:
        print('{v.name} = {v.x}'.format(**locals()))
        print('          ', end='')
