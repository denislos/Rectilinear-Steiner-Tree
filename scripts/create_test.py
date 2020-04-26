#!

import random
import itertools

def main():
    N = 150
    num_terminals = 100

    begin_string = '<root>\n<grid min_x="0" max_x="150" min_y="0" max_y="150" />\n<net>\n'
    end_string = '\n</net>\n</root>'

    choices = random.sample(list(itertools.permutations(range(N), 2)), num_terminals)

    with open('generated_test.xml', 'w') as f:
        f.write(begin_string)
        output_points = '\n'.join([f'<point x="{x}" y="{y}" layer="pins"/>' for (x, y) in choices])
        f.write(output_points)
        f.write(end_string)


if __name__ == '__main__':
    main()
