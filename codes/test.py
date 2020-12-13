from ctypes import *
 
p = (c_char * 10)(*list(range(10)))
 
m = cast(p, c_char_p)

print(m)

print(dir(m))
