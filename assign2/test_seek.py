#test mmap
#!/usr/bin/python

import shutil

#copy file
shutil.copyfile("lorem.txt", "lorem_copy.txt")

f = open("lorem_copy.txt", "rw+")
f.seek(-39000, 2)
f.write("tacostues1")

f.seek(5005, 1)
f.write("tacostues2")

f.seek(10000, 0)
f.write("tacostues3")

f.seek(15000, 0)
f.write("tacostues3")

f.seek(25500, 0)
f.write("tacostues2")

f.seek(48000, 0)
f.write("tacostues3")

f.seek(5100, 0)
f.write("tacostues3")

f.seek(-2700, 2)
f.write("tacostues4")

f.seek(0, 0)
f.write("tacostues5")

f.seek(0, 2)
f.write("tacostues5")

f.close()                              