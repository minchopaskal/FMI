#/usr/bin/python3

def get_defs():
    defs = []
    with open("ast_definitions.fmt", "r") as f:
        l = f.readline()
        while l != '':
            if l.startswith("#"):
                l = f.readline()
                continue
            l = l.rstrip()
            defs.append(l.split(" "))
            l = f.readline()
    return defs
