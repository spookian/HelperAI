patch1 = b'\x00\x0C'
patch2 = b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF'

file = "./build/Helper.dol"
fileHandler = open(file, "rb+")
fileHandler.seek(0xAE)
fileHandler.write(patch1)

fileHandler.seek(0x1C)
stroffset = fileHandler.read(4)
offset = int.from_bytes(stroffset, "big")
fileHandler.seek(offset)
fileHandler.write(patch2)

fileHandler.close()





