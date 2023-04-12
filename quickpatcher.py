patch = b'\x80\x8D\x12\x0C'

file = "main_new.dol"
fileHandler = open(file, "rb+")
fileHandler.seek(0xE0)
fileHandler.write(patch)
fileHandler.close()





