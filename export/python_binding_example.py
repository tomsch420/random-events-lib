from export import random_events as re

si = re.SimpleInterval(3, 5, re.BorderType.CLOSED, re.BorderType.OPEN)
print("simple interval 1: ", si)

si2 = re.SimpleInterval(1, 7, re.BorderType.CLOSED, re.BorderType.CLOSED)
print("simple interval 2: ", si2)

a = re.SetElement({"a", "b", "c"}, 1)
print("set element element index", a.element_index)
print("set element all elements", a.all_elements)

se = re.Set({"a", "b", "c", "d"})
print("set: ", se)
print("set elements", se.all_elements)

print("intersect two simple intervals", si.intersect_with(si2))
print("complement of closed interval %s" % si.complement())
print("complement of open interval %s" % si2.complement())

x = re.Interval({si, si2})

for e in si2.complement().simple_sets:
    print(e)

print("closed interval %s" % re.closed(1, 8))
print("open interval %s" % re.open(1, 5))
print("closed_open interval %s" % re.closed_open(1, 12))
print("open_closed interval %s" % re.open_closed(1, 3))
print("singleton %s" % re.singleton(5))
print("empty interval %s" % re.empty())
print("reals %s" % re.reals())

sev = re.SimpleEvent()
print("simple event: ", sev)
print("simple event elements: ", sev.get_variables())
print("simple event complement: ", sev.complement())
