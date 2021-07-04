def find_level(line):
    level = 0
    for c in line:
        if c == '.':
            level = level + 1
        else:
            break
    return level


def find_file_name(line):
    level = find_level(line)
    return line[level:].strip()


def find_base_name(line):  # do something fancier
    return find_file_name(line)


def is_base(line):
    return line[0] == '['


class IncFile:
    def __init__(self, name, parent=None, level=0):
        self.name = name
        self.level = level
        self.includes = []
        self.parent = parent

    def add_include(self, includee):
        self.includes.append(includee)

    def get_parent(self):
        return self.parent

    def __repr__(self):
        return '{} {}'.format(self.level, self.name)

    def print(self):
        print(self.__repr__())
        for i in self.includes:
            i.print()

    def dot(self):
        for i in self.includes:
            print('"{}" -> "{}";'.format(self.name, i.name))
            i.dot()

def main():
    # parse arguments
    # TODO: parse program arguments
    # TODO: read from stdin
    # TODO: export using jinja
    # TODO: group folders (optional)
    # TODO: filter out files or folders that aren't interesting
    input_file_name = './bld/input.txt'

    # read file
    input_file = open(input_file_name, 'r')
    base = IncFile(input_file_name)
    x = base
    for line in input_file:
        line = line.strip()

        if is_base(line):
            name = find_base_name(line)
            ff = IncFile(name, x, 0)
            x.add_include(ff)
            x = ff
        else:
            level = find_level(line)
            name = find_file_name(line)

            if level <= 0:
                # ignore
                pass
            elif x.level >= level:
                # new include should go to parent until level match
                while x.level >= level:
                    x = x.get_parent()
                ff = IncFile(name, x, level)
                x.add_include(ff)
                x = ff
            else:
                ff = IncFile(name, x, level)
                x.add_include(ff)
                x = ff

    input_file.close()

    # generate output
    print("digraph G {")
    base.dot()
    print("}")

    pass


if __name__ == '__main__':
    main()
