from api import VacuumCleaner
import inspect

lines = inspect.getsource(VacuumCleaner)
print(lines)
