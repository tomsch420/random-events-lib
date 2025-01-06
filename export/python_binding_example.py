from export import random_events_lib as re

si = re.SimpleInterval(3, 5, re.BorderType.CLOSED, re.BorderType.OPEN)
print("simple interval 1: ", si)

si2 = re.SimpleInterval(1, 7, re.BorderType.CLOSED, re.BorderType.CLOSED)
print("simple interval 2: ", si2)

a = re.SetElement(0, 3)
b = re.SetElement(1, 3)
c = re.SetElement(2, 3)
print("set element: ", a)
print("set element element index", a.element_index)
print("set element all elements", a.all_elements_length)

se = re.Set({a, b, c}, 3)
print("set simple sets", se.simple_sets)
print("set elements size", len(se.simple_sets))
print("set elements length", se.all_elements_length)

print("intersect two simple intervals", si.intersection_with(si2))
print("complement of closed interval %s" % si.complement())
print("complement of open interval %s" % si2.complement())

inter = re.Interval({si, si2})
print("interval: ", inter.simple_sets)
print("interval empty", inter.is_empty())

print("closed interval %s" % re.closed(1, 8))
print("open interval %s" % re.open(1, 5))
print("closed_open interval %s" % re.closed_open(1, 12))
print("open_closed interval %s" % re.open_closed(1, 3))
print("singleton %s" % re.singleton(5))
print("empty interval %s" % re.empty())
print("reals %s" % re.reals())


x = re.Continuous("x")
print("continuous variable: ", x)
y = re.Continuous("y")
z = re.Continuous("z")

sa = re.SetElement(0, 3)
sb = re.SetElement(1, 3)
sc = re.SetElement(2, 3)

s = re.Set({sa, sb, sc}, 3)

a = re.Symbolic("a", s)
print("symbolic variable: ", a)
b = re.Symbolic("b", s)

map = {a: s, x: re.Interval(si), y: re.Interval(si2)}


simple_event = re.SimpleEvent(map)

print("simple event: ", simple_event)
# print("simple event elements: ", sev.get_variables())
print("simple event complement: ", simple_event.complement())

event = re.Event(simple_event)
print("event: ", event)
